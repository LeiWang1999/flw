#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <typeinfo>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/*
 *  @Description : Realize the last layer of yolov2 target detection ----- region_layer
 *  @Version : V1.0
 *  @Author : Wangxu
 *  @Date : 2020-5-25 
*/

string jpg_dir = "/home/xu/YOLO/yolo_demo1/image_jpg/"; //项目根目录
string txt_dir = "/home/xu/YOLO/yolo_demo1/txt_trans/";
string pro_dir = "/home/xu/YOLO/yolo_demo1/";
string clsNames = pro_dir + "voc.names";

// float confidenceThreshold = 0.25;
float confidenceThreshold = 0.3;
float NMSThreshold = 0.3;

//image size
int img_size_w = 416;
int img_size_h = 416;

//voc anchors
float VOC[10]={1.08, 3.42, 6.63, 9.42, 16.62 , 1.19, 4.41, 11.38, 5.11, 10.52};

typedef struct{
	float x;
	float y;
	float width;
	float height;
}RectNMS;

typedef struct {
	RectNMS box;
	float prob;
	int index;
}BOX_b;

//从大到小的顺序排列
bool comp(BOX_b a, BOX_b b) {
	if(a.prob>b.prob) 
		return true;
	else 
		return false;
}

static float get_boxs_iou(RectNMS rect1, RectNMS rect2)
{
	int xx1, yy1, xx2, yy2;
 
	xx1 = max(rect1.x, rect2.x);
	yy1 = max(rect1.y, rect2.y);
	xx2 = min(rect1.x + rect1.width - 1, rect2.x + rect2.width - 1);
	yy2 = min(rect1.y + rect1.height - 1, rect2.y + rect2.height - 1);
 
	int insection_width, insection_height;
	insection_width = max(0, xx2 - xx1 + 1);
	insection_height = max(0, yy2 - yy1 + 1);
 
	float insection_area, union_area, iou;
	insection_area = float(insection_width) * insection_height;
	union_area = float(rect1.width*rect1.height + rect2.width*rect2.height - insection_area);
	iou = insection_area / union_area;
	return iou;
}
//input:  boxes: 原始检测框集合;
//input:  confidences：原始检测框对应的置信度值集合
//input:  confThreshold 和 nmsThreshold 分别是 检测框置信度阈值以及做nms时的阈值
//output:  indices  经过上面两个阈值过滤后剩下的检测框的index 
vector<BOX_b> nms_boxes(float *boxes,float *scores,vector<int> idx)
{
	BOX_b b_;
	RectNMS box;
	vector<BOX_b> b_s;
	int i,j,k;
	//b_s包括 [box坐标,box score,box index]，保留index是为了下面根据前两项获取当前index
	for(k=0;k<720;k++){
		box.x = boxes[4*k];		
		box.y = boxes[4*k+1];
		box.width = boxes[4*k+2];
		box.height = boxes[4*k+3];
		b_.box=box;		
		b_.prob=scores[k];	
		b_.index=idx[k];
		b_s.push_back(b_);
	}
	sort(b_s.begin(),b_s.end(),comp);//从大到小排序

	int b_s_size = b_s.size();
	for(i = 0 ; i < b_s_size ; i++)
	{		
		//如果当前box的iou值小于置信度，那么说明之后的都小于置信度，则退出
		if(b_s[i].prob<confidenceThreshold)
		{
			b_s.erase(b_s.begin()+i);
			b_s_size=b_s.size();
			i--;
			continue;				
		} 		
		for(j = i+1; j < b_s_size ; j++) //计算与当前box后面box 与 当前box 的iou，若iou较大则删除
		{   
			float iou = get_boxs_iou(b_s[i].box,b_s[j].box);
			if(iou > NMSThreshold && b_s[i].index==b_s[j].index)
			{
				b_s.erase(b_s.begin()+j);
				b_s_size=b_s.size();
				j--;
			}
		}
	}	
	return b_s;
}

float softmax(float* src, float* dst, int length)
{
    //为了避免溢出，需要减去最大值
    const float max_value = *std::max_element(src, src + length);
    float denominator = 0.0;
 	//printf("[log] Enter softmax\n");
    for (int i = 0; i < length; ++i) {
        dst[i] = std::exp(src[i] - max_value);
        denominator += dst[i];
    }
    for (int i = 0; i < length; ++i) {
        dst[i] /= denominator;
    }
	//printf("[log] exit softmax\n");
    return 0;
}

float logistic(float val)
{
	return 1.0 / (1.0 + std::exp(-val));
}


float get_region_box(float *output, int offsetx , int offsety , float *box , float biasw , float biash)
{
    int w = 12, h = 12;

	box[0] = ((float)offsetx -0.5 + logistic(output[0])) / (float)w;
    box[1] = ((float)offsety -0.5 + logistic(output[1])) / (float)h;
    box[2] = std::exp(output[2]) * biasw / (float)w;
    box[3] = std::exp(output[3]) * biash / (float)h;

	box[0] = box[0]*(float)img_size_w; 
	box[1] = box[1]*(float)img_size_h;
	box[2] = box[2]*(float)img_size_w;
	box[3] = box[3]*(float)img_size_h;
}


vector<int> parse_yolo_output_v2(float *output, float *prob , float *boxes)
{
	int max_idx=0;
	vector<int> index;
	float *com_prob = (float*)malloc(720*20*4);
	float* in;
	float* out = new float[20];
	for(int i=0 ; i < 720; i++ )
	{
		in = output+i*25+5;
		softmax(in, out , 20);
		for (int j = 0 ; j < 20; j++ )
		{			
			com_prob[i*20+j] = logistic(output[i*25+4]) * out[j];  //综合概率					
		}
		for(int k = 0; k < 20; k++)//循环20次找出每一组prob中最大值
		{
			if(com_prob[i*20+k] > com_prob[i*20+max_idx])
				max_idx = k;				
		}
		prob[i] = com_prob[i*20+max_idx];
		index.push_back(max_idx);

		int l = i % 5;
		int n = i * 25 / 125;
		int offset_x = n % 12;
		int offset_y = n / 12;

		get_region_box(output + i * 25, offset_x, offset_y, boxes + i * 4, VOC[l], VOC[l+5]);
	}
	return index;
	free(com_prob);
	delete [] out;
}

void image_detection(string image_path,string loadtxt_file) //loadtxt_file 720*25
{
	float *Bboxes = (float*)malloc(720*4*4);;
	float *Pprobs = (float*)malloc(720*4);
	int size = 720*25*4;
	float number;
	vector<int> index;
	vector<string> classNamesVec;
	vector<BOX_b> Bbox;
	RectNMS box;

	printf("[log] Enter image_detection\n");

	ifstream file(loadtxt_file.c_str());
	if ( !file )
	{
		perror("error");
		exit(1);
	}
	float *region_layer_input = (float*)malloc(size);
	for(int i = 0;i < 18000;i++)
	{
		file >> number;
		region_layer_input[i] = number;
	}
	//加载图像
	Mat frame=imread(image_path.c_str());
	Mat detectionMat(720,25,CV_32FC1);    //reshap	
    detectionMat.data=(uchar*)malloc(size);
	memcpy(detectionMat.data,region_layer_input,size);
	float *arr=(float*)detectionMat.data;
	// 加载分类信息	
	ifstream classNamesFile(clsNames.c_str());
	if (classNamesFile.is_open())
	{
		string className = "";
		while (std::getline(classNamesFile, className))
			classNamesVec.push_back(className);
	}
 	index = parse_yolo_output_v2(arr,Pprobs,Bboxes);
	 
	Bbox = nms_boxes(Bboxes,Pprobs,index);

	for (int i = 0; i < Bbox.size(); i++)
	{
		box = Bbox[i].box;

		cout << "********************" << endl;
		cout << "prob  : " << Bbox[i].prob << endl;
		cout << "index : " << Bbox[i].index << endl;
		cout << "x     : " << std::fixed << Bbox[i].box.x << endl;
		cout << "y     : " << std::fixed << Bbox[i].box.y << endl;
		cout << "width : " << std::fixed << Bbox[i].box.width << endl;
		cout << "height: " << std::fixed << Bbox[i].box.height << endl;
		cout << "********************" << endl;

		float x = box.x;
		float y = box.y;
		float width = box.width;
		float height = box.height;				

		int xLeftBottom = static_cast<int>(x - width / 2);
		int yLeftBottom = static_cast<int>(y - height / 2);
		int xRightTop = static_cast<int>(x + width / 2);
		int yRightTop = static_cast<int>(y + height / 2);

		Rect object(xLeftBottom, yLeftBottom,
					xRightTop - xLeftBottom,
					yRightTop - yLeftBottom);
		rectangle(frame, object, Scalar(0, 0, 255), 2, 8);

		if (Bbox[i].index < classNamesVec.size())
		{
			ostringstream ss;
			ss.str("");
			ss << Bbox[i].prob;
			String conf(ss.str());
			String label = String(classNamesVec[Bbox[i].index]) + ": " + conf;
			int baseLine = 0;
			Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
			rectangle(frame, Rect(Point(xLeftBottom, yLeftBottom),
				Size(labelSize.width, labelSize.height + baseLine)),
				Scalar(0, 0, 255), CV_FILLED);
			putText(frame, label, Point(xLeftBottom, yLeftBottom + labelSize.height),
				FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0));
		}
	}
	imshow("YOLO-Detections", frame);
	imwrite("result.jpg", frame);
	waitKey(0);

	free(Pprobs);
	free(Bboxes);
	free(region_layer_input);

	printf("[log] Exit image_detection\n");
}


/*
*  @param image_path  图像文件
*  @param loadtxt_file  TXT文件
*/
int main(int argc, char** argv)
{	
	string image_path = jpg_dir + "6.jpg";
	string loadtxt_file = txt_dir + "out_6.txt";
	image_detection(image_path,loadtxt_file);//图片测试
}

