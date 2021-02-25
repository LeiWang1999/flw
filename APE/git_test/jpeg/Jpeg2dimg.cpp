#include "Jpeg2dimg.h"

     
void JPEG2DIMG(const char * inputFileName, NvDlaImage* in,uint8_t* out)
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
		    in->m_meta.surfaceFormat = NvDlaImage::T_R8;
		    in->m_meta.channel = 1;
		    break;
		case JCS_YCbCr: 
		case JCS_RGB:
		    info.out_color_space = JCS_EXT_BGR;                    
		    in->m_meta.surfaceFormat = NvDlaImage::T_B8G8R8;
		    in->m_meta.channel = 3;
		    break;
		case JCS_EXT_RGB: // upsizing to 4 Chnls from 3 Chnls
		case JCS_EXT_RGBX:
		    in->m_meta.surfaceFormat = NvDlaImage::T_R8G8B8X8;
		    in->m_meta.channel = 4;
		    break;
		case JCS_EXT_BGR: // upsizing to 4 Chnls from 3 Chnls
		case JCS_EXT_BGRX:
		    in->m_meta.surfaceFormat = NvDlaImage::T_B8G8R8X8;
		    in->m_meta.channel = 4;
		    break;
		case JCS_EXT_XBGR:
		    in->m_meta.surfaceFormat = NvDlaImage::T_X8B8G8R8;
		    in->m_meta.channel = 4;
		    break;
		case JCS_EXT_XRGB:
		    in->m_meta.surfaceFormat = NvDlaImage::T_X8R8G8B8;
		    in->m_meta.channel = 4;
		    break;
		case JCS_EXT_RGBA:
		    in->m_meta.surfaceFormat = NvDlaImage::T_R8G8B8A8;
		    in->m_meta.channel = 4;
		    break;
		case JCS_EXT_BGRA:
		    in->m_meta.surfaceFormat = NvDlaImage::T_B8G8R8A8;
		    in->m_meta.channel = 4;
		    break;
		case JCS_EXT_ABGR:
		    in->m_meta.surfaceFormat = NvDlaImage::T_A8B8G8R8;
		    in->m_meta.channel = 4;
		    break;
		case JCS_EXT_ARGB:
		    in->m_meta.surfaceFormat = NvDlaImage::T_A8R8G8B8;
		    in->m_meta.channel = 4;
		    break;
		case JCS_CMYK:
		case JCS_YCCK:
		default: printf("JPEG color space %d not supported\n", info.jpeg_color_space);
	}
	printf("JPEG color space is : %d\n", info.jpeg_color_space);
	(void) jpeg_start_decompress(&info);//开始解压缩图像
	in->m_meta.height = info.image_height;
	in->m_meta.width = info.image_width;
	in->m_meta.lineStride = info.image_width*8;
	in->m_meta.surfaceStride = 0;
	in->m_meta.size = info.image_width*info.image_height*8;

	printf("dlaimg height: %d x %d x %d: ", in->m_meta.height, in->m_meta.width, in->m_meta.channel);
	printf("LS: %d SS: %d Size: %d\n", in->m_meta.lineStride, in->m_meta.surfaceStride, in->m_meta.size);

	// Allocate the buffer
	in->m_pData = malloc(in->m_meta.size);
	if (!in->m_pData) {
		printf("allocate in buffer fail\n");
		return;
	}
	memset(in->m_pData, 0, in->m_meta.size);    
	printf("alloc buffer over \n");
	
	rowPtr[0] = static_cast<uint8_t*>(in->m_pData);

        while (info.output_scanline < info.output_height) {
            jpeg_read_scanlines(&info, rowPtr, 1);
            rowPtr[0] += in->m_meta.lineStride;

	    //printf("data   : %p\n",*rowPtr);
	    //printf("stride : %d\n",rowPtr[0]);
        }

	
	(void) jpeg_finish_decompress(&info);//结束解压缩操作
	printf("jpeg decompress finish !\n");
	jpeg_destroy_decompress(&info);//释放资源

	ibuf = static_cast<uint8_t*>(in->m_pData);
	obuf = static_cast<uint8_t*>(out);

	for (uint32_t y=0; y < in->m_meta.height; y++)
	{
		for (uint32_t x=0; x < in->m_meta.width; x++)
		{
		    for (uint32_t z=0; z < in->m_meta.channel; z++)
		    {
			//int32_t ioffset = in->getAddrOffset(x, y, z);
			int32_t ioffset = (y * in->m_meta.lineStride) + (x * in->m_meta.channel) + z;

			//int32_t ooffset = out->getAddrOffset(x, y, z);
			int32_t ooffset = (z / 8 * info.image_width*info.image_height*8) + (y * info.image_width*8) + (x * 8) + (z % 8);

			if (ioffset < 0)
			printf("ERROR ioffset\n");
			if (ooffset < 0)
			printf("ERROR ooffset\n");

			uint8_t* inp = ibuf + ioffset;

			char* outp = reinterpret_cast<char*>(obuf + ooffset);

			*outp = static_cast<int8_t>(std::floor((*inp * 127.0/255.0) + 0.5f));
		    }
		}
	}
	fclose(fp);
}




