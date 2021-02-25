#include "picture_to_data.h"
#include "jpeglib.h"

void picture_to_data(const char * inputFileName, struct Metadata in,uint8_t* out)
{
	struct jpeg_decompress_struct info;//变量定义
	struct jpeg_error_mgr err;
	uint8_t* rowPtr[1];
	uint8_t* ibuf = 0;
	uint8_t* obuf = 0;
	
	FILE * fp;

	if ((fp = fopen(inputFileName, "rb")) == NULL)//指定图像文件
	{
		printf("Cant open file %s",inputFileName);
		return;
	}
	printf("open file is --%s\n",inputFileName);
     
	info.err = jpeg_std_error(&err);//绑定标准错误处理结构 
	info.dct_method = JDCT_ISLOW; 
	jpeg_create_decompress(&info);//初始化JPEG对象

	jpeg_stdio_src(&info, fp);//读取图像信息	
	printf("read jpeg data ok!\n");
	jpeg_read_header(&info, TRUE);
	printf("read header ok!\n");

	switch(info.jpeg_color_space) {
		case JCS_GRAYSCALE:
		    in.channel = 1;
		    break;
		case JCS_YCbCr: 
		case JCS_RGB:
		    info.out_color_space = JCS_EXT_BGR;                    
		    in.channel = 3;
		    break;
		case JCS_EXT_RGB: // upsizing to 4 Chnls from 3 Chnls
		case JCS_EXT_RGBX:
		    in.channel = 4;
		    break;
		case JCS_EXT_BGR: // upsizing to 4 Chnls from 3 Chnls
		case JCS_EXT_BGRX:
		    in.channel = 4;
		    break;
		case JCS_EXT_XBGR:
		    in.channel = 4;
		    break;
		case JCS_EXT_XRGB:
		    in.channel = 4;
		    break;
		case JCS_EXT_RGBA:
		    in.channel = 4;
		    break;
		case JCS_EXT_BGRA:
		    in.channel = 4;
		    break;
		case JCS_EXT_ABGR:
		    in.channel = 4;
		    break;
		case JCS_EXT_ARGB:
		    in.channel = 4;
		    break;
		case JCS_CMYK:
		case JCS_YCCK:
		default: printf("JPEG color space %d not supported\n", info.jpeg_color_space);
	}
	printf("JPEG color space is : %d\n", info.jpeg_color_space);
	(void) jpeg_start_decompress(&info);//开始解压缩图像
	in.height = info.image_height;
	in.width = info.image_width;
	in.lineStride = info.image_width*8;
	in.surfaceStride = 0;
	in.size = info.image_width*info.image_height*8;

	//printf("dlaimg height: %d x %d x %d: ", in->m_meta.height, in->m_meta.width, in->m_meta.channel);
	//printf("LS: %d SS: %d Size: %d\n", in->m_meta.lineStride, in->m_meta.surfaceStride, in->m_meta.size);

	// Allocate the buffer
	in.m_pData =(uint8_t *)malloc(in.size);
	if (!in.m_pData) {
		printf("allocate in buffer fail\n");
		return;
	}
	memset(in.m_pData, 0, in.size);   
	printf("alloc buffer over \n");
	
	rowPtr[0] = in.m_pData;

        while (info.output_scanline < info.output_height) {
            jpeg_read_scanlines(&info, rowPtr, 1);
            rowPtr[0] += in.lineStride;

	    //printf("data   : %p\n",*rowPtr);
	    //printf("stride : %d\n",rowPtr[0]);
        }

	
	(void) jpeg_finish_decompress(&info);//结束解压缩操作
	printf("jpeg decompress finish !\n");
	jpeg_destroy_decompress(&info);//释放资源

	ibuf =in.m_pData;
	obuf =out;

	for (uint32_t y=0; y < in.height; y++)
	{
		for (uint32_t x=0; x < in.width; x++)
		{
		    for (uint32_t z=0; z < in.channel; z++)
		    {
			//int32_t ioffset = in->getAddrOffset(x, y, z);
			int32_t ioffset = (y * in.lineStride) + (x * in.channel) + z;

			//int32_t ooffset = out->getAddrOffset(x, y, z);
			int32_t ooffset = (z / 8 * info.image_width*info.image_height*8) + (y * info.image_width*8) + (x * 8) + (z % 8);
			//printf("ioffset %d ooffset %d \n",ioffset,ooffset);
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
	fclose(fp);
}




