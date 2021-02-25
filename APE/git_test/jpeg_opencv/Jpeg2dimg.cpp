#include "Jpeg2dimg.h"

using namespace std;
using namespace cv;


void JPEG2DIMG(Mat Image,uint8_t *out)
{
	uint8_t* ibuf;
	uint8_t* obuf = 0;	
	int height = 224;
	int width = 224;
	int n = 0,m = 0;
	uint8_t* pImageData = (uint8_t*)malloc(width*height* 3 * sizeof(int));
	uint8_t* repImageData = (uint8_t*)malloc(width*height* 8 * sizeof(int));

	if(!Image.data){
			cout<< "open image fail ! " << endl;
	}
	for(int i = 0; i < height; i++)
	{
		const uchar* current = Image.ptr<uchar>(i);
		for(int j = 0; j < width; j++)
		{
			pImageData[i*width* 3 + 3 * j + 0] = current[3 * j + 0];
			pImageData[i*width * 3+ 3 * j + 1] = current[3 * j + 1];
			pImageData[i*width * 3+ 3 * j + 2] = current[3 * j + 2];
		}

	}	
	for(int i=0;i<3*224;i++){
		for(int k=0;k<224;k++){
		repImageData[n++]=pImageData[m++];
		}		
		if((i+1)%3==0){
			for(int j=0;j<5*224;j++){
				repImageData[n++] = 0;
			}
		}
	}			  
	//printf("img rows : %d , cols : %d \n",Image.rows,Image.cols);
	int lineStride = Image.cols*8;
	ibuf = repImageData;
	obuf = out;

	for (uint32_t y=0; y < Image.rows; y++)
	{
		for (uint32_t x=0; x < Image.cols; x++)
		{
		    for (uint32_t z=0; z < 3; z++)
		    {
				int32_t ioffset = (y * lineStride) + (x * 3) + z;
				int32_t ooffset = (z / 8 * Image.cols*Image.rows*8) + (y * Image.cols*8) + (x * 8) + (z % 8);
				//fprintf(fp2,"ioffset: %d, ooffset: %d\n",ioffset,ooffset);	

				if (ioffset < 0)
					printf("ERROR ioffset\n");
				if (ooffset < 0)
					printf("ERROR ooffset\n");

				uint8_t* inp = ibuf + ioffset;
				char* outp = (char*)(obuf + ooffset);
				*outp = (int8_t)floor(((*inp * 127.0/255.0) + 0.5f));
		    }
		}
	}
	printf("EXIT\n");
}




