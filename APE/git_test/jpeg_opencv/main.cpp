#include "Jpeg2dimg.h"

using namespace std;
using namespace cv;

int main() 
{
	printf("start\n");
	Mat input_image = imread("qie.jpg");;
	FILE *fp1=fopen("xxx.bin","wb");

	uint8_t *out_data = (uint8_t *)malloc(401408);
	if (!out_data) {
		printf("allocate out buffer fail\n");		
	}
	memset(out_data , 0, 401408);	

	JPEG2DIMG(input_image,out_data);

	uint8_t *pBuffer=(uint8_t *)malloc(401408);  //写入文件
	memcpy(pBuffer,out_data, 401408);
	fwrite(pBuffer,1,401408,fp1);
	fclose(fp1);
	printf("END !\n");


    
}
