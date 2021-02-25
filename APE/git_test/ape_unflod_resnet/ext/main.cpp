#include <iostream>
#include <stdio.h>
#include <cstdio> 
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "half.h"
#include "NvDlaType.h"
#include "dlaerror.h"
#include "IRuntime.h"
extern "C" {
#include "jpeglib.h"
}

using namespace std;
using namespace nvdla;
using namespace half_float;



class NvDlaImage
{
public:
	typedef enum _PixelFormat
    	{
        T_R8 = 0,
        T_R8_I = 1,
        T_R10 = 2,
        T_R12 = 3,
        T_R16 = 4,
        T_R16_I = 5,
        T_R16_F = 6,
        T_A16B16G16R16 = 7,
        T_A16B16G16R16_F = 8,
        T_X16B16G16R16 = 9,
        T_A16Y16U16V16 = 10,
        T_A16Y16U16V16_F = 11,
        T_V16U16Y16A16 = 12,
        T_A8B8G8R8 = 13,
        T_A8R8G8B8 = 14,
        T_B8G8R8A8 = 15,
        T_R8G8B8A8 = 16,
        T_X8B8G8R8 = 17,
        T_X8R8G8B8 = 18,
        T_B8G8R8X8 = 19,
        T_R8G8B8X8 = 20,
        T_A2B10G10R10 = 21,
        T_A2R10G10B10 = 22,
        T_B10G10R10A2 = 23,
        T_R10G10B10A2 = 24,
        T_A2Y10U10V10 = 25,
        T_V10U10Y10A2 = 26,
        T_A8Y8U8V8 = 27,
        T_V8U8Y8A8 = 28,
        T_Y8___U8V8_N444 = 29,
        T_Y8___V8U8_N444 = 30,
        T_Y10___U10V10_N444 = 31,
        T_Y10___V10U10_N444 = 32,
        T_Y12___U12V12_N444 = 33,
        T_Y12___V12U12_N444 = 34,
        T_Y16___U16V16_N444 = 35,
        T_Y16___V16U16_N444 = 36,

        D_F8_CHW_I = 37,
        D_F16_CHW_I = 38,
        D_F16_CHW_F = 39,

        D_F8_CxHWx_x32_I =  40,
        D_F8_CxHWx_x8_I =  41,
        D_F16_CxHWx_x16_I = 42,
        D_F16_CxHWx_x16_F = 43,

        D_F32_CHW_F = 44,
        D_F32_CxHWx_x8_F = 45,

        T_R8G8B8 = 46,
        T_B8G8R8 = 47,
    } PixelFormat;

    typedef enum _PixelFormatType
    {
        UINT = 0,
        INT = 1,
        IEEEFP = 2,
        UNKNOWN = 3
    } PixelFormatType;

	struct Metadata
	{
		PixelFormat surfaceFormat;
		uint32_t width;
		uint32_t height;
		uint32_t channel;

		uint32_t lineStride;
		uint32_t surfaceStride;
		uint32_t size;
	} m_meta;

	void* m_pData;
	int8_t getBpe() const;
	PixelFormatType getPixelFormatType() const;
	int32_t getAddrOffset(uint32_t w, uint32_t h, uint32_t c) const;
	NvDlaError printInfo() const;
	NvDlaError printBuffer(bool showBorders) const;

	NvDlaError serialize(std::stringstream& sstream, bool stableHash) const;
	NvDlaError deserialize(std::stringstream& sstream);
	NvDlaError packData(std::stringstream& sstream, bool stableHash, bool asRaw) const;
	NvDlaError unpackData(std::stringstream& sstream);
};





struct TestAppArgs
{
    std::string inputPath;
    std::string inputName;
    std::string loadableName;
    int32_t serverPort;
    uint8_t normalize_value;
    float mean[4];
    bool rawOutputDump;

    TestAppArgs() :
        inputPath("./"),
        inputName(""),
        loadableName(""),
        serverPort(6666),
        normalize_value(1),
        mean{0.0, 0.0, 0.0, 0.0},
        rawOutputDump(false)
    {}
};

struct TestInfo
{
    TestInfo() :
        runtime(NULL),
        inputLoadablePath(""),
        inputHandle(NULL),
        outputHandle(NULL),
        pData(NULL),
        dlaServerRunning(false),
        dlaRemoteSock(-1),
        dlaServerSock(-1),
        numInputs(0),
        numOutputs(0),
        inputImage(NULL),
        outputImage(NULL)
    {}
    // runtime
    nvdla::IRuntime* runtime;
    std::string inputLoadablePath;
    uint8_t *inputHandle;
    uint8_t *outputHandle;
    uint8_t *pData;
    bool dlaServerRunning;
    int32_t dlaRemoteSock;
    int32_t dlaServerSock;
    uint32_t numInputs;
    uint32_t numOutputs;
    NvDlaImage* inputImage;
    NvDlaImage* outputImage;
};


int8_t NvDlaImage::getBpe() const
{
    int8_t bpe = -1;

    switch(m_meta.surfaceFormat)
    {
    case T_R8:
    case T_R8_I:
    case T_R8G8B8:
    case T_B8G8R8:
    case T_A8B8G8R8:
    case T_A8R8G8B8:
    case T_B8G8R8A8:
    case T_R8G8B8A8:
    case T_X8B8G8R8:
    case T_X8R8G8B8:
    case T_B8G8R8X8:
    case T_R8G8B8X8:
    case D_F8_CHW_I:
    case D_F8_CxHWx_x32_I:
    case D_F8_CxHWx_x8_I:
        bpe = 1;
        break;
    case T_A16B16G16R16_F:
    case D_F16_CHW_I:
    case D_F16_CHW_F:
    case D_F16_CxHWx_x16_I:
    case D_F16_CxHWx_x16_F:
        bpe = 2;
        break;
    case D_F32_CHW_F:
    case D_F32_CxHWx_x8_F:
        bpe = 4;
        break;
    default:
        bpe = -1;
    }

    return bpe;
}



int32_t NvDlaImage::getAddrOffset(uint32_t w, uint32_t h, uint32_t c) const
{
    int32_t offset = -1;

    if (w >= static_cast<uint32_t>(m_meta.width) || h >= static_cast<uint32_t>(m_meta.height) || c >= static_cast<uint32_t>(m_meta.channel))
    {
        //REPORT_ERROR(NvDlaError_BadParameter);
	printf("E1\n");
        return -1;
    }

    int8_t bpe = getBpe();
    if (bpe <= 0)
    {
        //REPORT_ERROR(NvDlaError_BadParameter);
        
	printf("E2\n");
	return -1;
    }

    if (m_meta.surfaceFormat == T_R8)
    {
        if (c != 0)
        {
            //REPORT_ERROR(NvDlaError_BadParameter);
	printf("E3\n");
            return -1;
		
        }

        offset = ((h * m_meta.lineStride) + w) * bpe;
    }
    else if (m_meta.surfaceFormat == T_B8G8R8 ||
             m_meta.surfaceFormat == T_R8G8B8)
    {
        offset = (h * m_meta.lineStride) + (w * m_meta.channel) + c;
    }
    else if (m_meta.surfaceFormat == T_A8B8G8R8 || m_meta.surfaceFormat == T_A16B16G16R16_F)
    {
        uint32_t x = 4;
        uint32_t xStride = x * bpe;
        offset = (h * m_meta.lineStride) + (w * xStride) + (c * bpe);
    }
    else if (m_meta.surfaceFormat == D_F8_CxHWx_x32_I)
    {
        uint32_t x = 32;
        uint32_t xStride = x * bpe;
        uint32_t cquotient = c / x;
        uint32_t cremainder = c % x;

        offset = (cquotient * m_meta.surfaceStride) + (h * m_meta.lineStride) + (w * xStride) + (cremainder * bpe);
    }
    else if (m_meta.surfaceFormat == D_F8_CxHWx_x8_I)
    {
        uint32_t x = 8;
        uint32_t xStride = x * bpe;
        uint32_t cquotient = c / x;
        uint32_t cremainder = c % x;

        offset = (cquotient * m_meta.surfaceStride) + (h * m_meta.lineStride) + (w * xStride) + (cremainder * bpe);
    }
    else if (m_meta.surfaceFormat == D_F16_CxHWx_x16_I ||
             m_meta.surfaceFormat == D_F16_CxHWx_x16_F)
    {
        uint32_t x = 16;
        uint32_t xStride = x * bpe;
        uint32_t cquotient = c / x;
        uint32_t cremainder = c % x;

        offset = (cquotient * m_meta.surfaceStride) + (h * m_meta.lineStride) + (w * xStride) + (cremainder * bpe);
    }
    else if (m_meta.surfaceFormat == D_F32_CxHWx_x8_F)
    {
        uint32_t x = 8;
        uint32_t xStride = x * bpe;
        uint32_t cquotient = c / x;
        uint32_t cremainder = c % x;

        offset = (cquotient * m_meta.surfaceStride) + (h * m_meta.lineStride) + (w * xStride) + (cremainder * bpe);
    }
    else if (m_meta.surfaceFormat == D_F8_CHW_I ||
             m_meta.surfaceFormat == D_F16_CHW_I ||
             m_meta.surfaceFormat == D_F16_CHW_F ||
             m_meta.surfaceFormat == D_F32_CHW_F)
    {
        offset = (c * m_meta.surfaceStride) + (h * m_meta.lineStride) + (w * bpe);
    }
    else
    {
        //REPORT_ERROR(NvDlaError_BadParameter);
        return -1;
    }

    return offset;
}



////////////////////////////////////////////////////////////////////////////////////////////
void JPEG2DIMG(const char * inputFileName, NvDlaImage* buffer,nvdla::IRuntime::NvDlaTensor *tensorDesc)
{
	struct jpeg_decompress_struct info;//变量定义
	struct jpeg_error_mgr err;
	uint8_t* rowPtr[1];
	FILE * fp;
	FILE *fp1=fopen("qiejpgdecompress.bin","wb");
	int row_stride ;  

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
		    buffer->m_meta.surfaceFormat = NvDlaImage::T_R8;
		    buffer->m_meta.channel = 1;
		    break;
		case JCS_YCbCr: // FIXME: dont know how to handle compression yet
		case JCS_RGB:
		    info.out_color_space = JCS_EXT_BGR;                    // FIXME: currently extracting as BGR (since caffe ref model assumes BGR)
		    buffer->m_meta.surfaceFormat = NvDlaImage::T_B8G8R8;
		    buffer->m_meta.channel = 3;
		    break;
		case JCS_EXT_RGB: // upsizing to 4 Chnls from 3 Chnls
		case JCS_EXT_RGBX:
		    buffer->m_meta.surfaceFormat = NvDlaImage::T_R8G8B8X8;
		    buffer->m_meta.channel = 4;
		    break;
		case JCS_EXT_BGR: // upsizing to 4 Chnls from 3 Chnls
		case JCS_EXT_BGRX:
		    buffer->m_meta.surfaceFormat = NvDlaImage::T_B8G8R8X8;
		    buffer->m_meta.channel = 4;
		    break;
		case JCS_EXT_XBGR:
		    buffer->m_meta.surfaceFormat = NvDlaImage::T_X8B8G8R8;
		    buffer->m_meta.channel = 4;
		    break;
		case JCS_EXT_XRGB:
		    buffer->m_meta.surfaceFormat = NvDlaImage::T_X8R8G8B8;
		    buffer->m_meta.channel = 4;
		    break;
		case JCS_EXT_RGBA:
		    buffer->m_meta.surfaceFormat = NvDlaImage::T_R8G8B8A8;
		    buffer->m_meta.channel = 4;
		    break;
		case JCS_EXT_BGRA:
		    buffer->m_meta.surfaceFormat = NvDlaImage::T_B8G8R8A8;
		    buffer->m_meta.channel = 4;
		    break;
		case JCS_EXT_ABGR:
		    buffer->m_meta.surfaceFormat = NvDlaImage::T_A8B8G8R8;
		    buffer->m_meta.channel = 4;
		    break;
		case JCS_EXT_ARGB:
		    buffer->m_meta.surfaceFormat = NvDlaImage::T_A8R8G8B8;
		    buffer->m_meta.channel = 4;
		    break;
		case JCS_CMYK:
		case JCS_YCCK:
		default: printf("JPEG color space %d not supported\n", info.jpeg_color_space);
	    }
	printf("JPEG color space is : %d\n", info.jpeg_color_space);
	(void) jpeg_start_decompress(&info);//开始解压缩图像
	buffer->m_meta.height = info.image_height;
    buffer->m_meta.width = info.image_width;
    buffer->m_meta.lineStride = info.image_width*8;
    buffer->m_meta.surfaceStride = 0;
    buffer->m_meta.size = info.image_width*info.image_height*8;

    printf("dlaimg height: %d x %d x %d: ", buffer->m_meta.height, buffer->m_meta.width, buffer->m_meta.channel);
    printf("LS: %d SS: %d Size: %d\n", buffer->m_meta.lineStride, buffer->m_meta.surfaceStride, buffer->m_meta.size);

    // Allocate the buffer
    buffer->m_pData = malloc(buffer->m_meta.size);
   
    printf("buffer->m_meta.height %d\n",buffer->m_meta.height);
    printf("buffer->m_meta.width %d\n",buffer->m_meta.width);
    printf("buffer->m_meta.size %d\n ",buffer->m_meta.size);
    if (!buffer->m_pData) {
        printf("allocate buffer fail\n");
        return;
    }
      
    memset(buffer->m_pData, 0, buffer->m_meta.size);
    

	printf("alloc buffer over \n");
	
	rowPtr[0] = static_cast<uint8_t*>(buffer->m_pData);
	printf("row...\n");
        while (info.output_scanline < info.output_height) {
            jpeg_read_scanlines(&info, rowPtr, 1);
            rowPtr[0] += buffer->m_meta.lineStride;

	    printf("data   : %p\n",*rowPtr);
	    printf("stride : %d\n",rowPtr[0]);
        }
	NvDlaImage *pBuffer=(NvDlaImage *)malloc(buffer->m_meta.size);
	memcpy(pBuffer,buffer->m_pData, buffer->m_meta.size);
	fwrite(pBuffer,1,buffer->m_meta.size,fp1);
	



	(void) jpeg_finish_decompress(&info);//结束解压缩操作
	printf("jpeg decompress finish !\n");
	jpeg_destroy_decompress(&info);//释放资源
	fclose(fp);
	fclose(fp1);
	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void createImageCopy(const TestAppArgs* appArgs, const NvDlaImage* in, const nvdla::IRuntime::NvDlaTensor* outTensorDesc, NvDlaImage* out)
{
    uint8_t* ibuf = 0;
    uint8_t* obuf = 0;
    FILE *fp1=fopen("9JpegCopy.bin","wb");
	
    printf("111111\n");
    out->m_meta.width = outTensorDesc->dims.w;
    out->m_meta.height = outTensorDesc->dims.h;
    out->m_meta.channel = outTensorDesc->dims.c;
    printf("222222\n");

    if (in->m_meta.width != out->m_meta.width )
        printf("Mismatched width: %u != %u\n", in->m_meta.width, out->m_meta.width);
    if (in->m_meta.height != out->m_meta.height )
        printf("Mismatched height: %u != %u\n", in->m_meta.height, out->m_meta.height);
    if (in->m_meta.channel != out->m_meta.channel )
        printf("Mismatched channel: %u != %u\n", in->m_meta.channel, out->m_meta.channel);

    /*switch(outTensorDesc->pixelFormat)
    {
    case NVDLA_PIXEL_FORMAT_R8:
        out->m_meta.surfaceFormat = NvDlaImage::T_R8;
        out->m_meta.lineStride    = outTensorDesc->stride[1];
        out->m_meta.surfaceStride = 0;
        out->m_meta.size          = outTensorDesc->bufferSize;
        break;

    case NVDLA_PIXEL_FORMAT_FEATURE:
        if (outTensorDesc->dataType == NVDLA_DATA_TYPE_HALF)
            out->m_meta.surfaceFormat = NvDlaImage::D_F16_CxHWx_x16_F;
        else if (outTensorDesc->dataType == NVDLA_DATA_TYPE_INT8)
        {
            out->m_meta.surfaceFormat = NvDlaImage::D_F8_CxHWx_x32_I;
        }
        else
            printf("Unsupported (pixel, data) combination: (%u, %u)", outTensorDesc->pixelFormat, outTensorDesc->dataType);

        out->m_meta.lineStride    = outTensorDesc->stride[1];
        out->m_meta.surfaceStride = outTensorDesc->stride[2];
        out->m_meta.size          = outTensorDesc->bufferSize;
        break;

    case NVDLA_PIXEL_FORMAT_FEATURE_X8:*/
        //if (outTensorDesc->dataType == NVDLA_DATA_TYPE_INT8)
       /*     out->m_meta.surfaceFormat = NvDlaImage::D_F8_CxHWx_x8_I;
        //else
           // printf("Unsupported (pixel, data) combination: (%u, %u)", outTensorDesc->pixelFormat, outTensorDesc->dataType);

        out->m_meta.lineStride    = outTensorDesc->stride[1];
        out->m_meta.surfaceStride = outTensorDesc->stride[2];
        out->m_meta.size          = outTensorDesc->bufferSize;
        //break;

    case NVDLA_PIXEL_FORMAT_A16B16G16R16_F:
        out->m_meta.surfaceFormat = NvDlaImage::T_A16B16G16R16_F;
        out->m_meta.lineStride    = outTensorDesc->stride[1];
        out->m_meta.surfaceStride = 0;
        out->m_meta.size          = outTensorDesc->bufferSize;
        break;

    case NVDLA_PIXEL_FORMAT_A8B8G8R8:
        out->m_meta.surfaceFormat = NvDlaImage::T_A8B8G8R8;
        out->m_meta.lineStride    = outTensorDesc->stride[1];
        out->m_meta.surfaceStride = 0;
        out->m_meta.size          = outTensorDesc->bufferSize;
        break;
    default:
        printf("Unsupported pixel format: %u", outTensorDesc->pixelFormat);
    }*/
    printf(" pixel format is : %u\n", outTensorDesc->pixelFormat);
    printf(" out->m_meta.lineStride : %d\n", out->m_meta.lineStride);
    printf(" out->m_meta.surfaceStride : %d\n", out->m_meta.surfaceStride);
    printf(" out->m_meta.size : %d\n", out->m_meta.size);

    if (out->getBpe() <= 0)
		printf("NvDlaError_BadParameter 0\n");
   
    // These calculations work for channels <= 16
    if (out->m_meta.channel > 16)
		printf("NvDlaError_BadParameter 1\n");
    
    // Number of input channels should be <= 4
    if (in->m_meta.channel > 4)
        printf("Input channel should not be greater than 4\n");

    if ( 0 )
    {
        printf("Dims: %d x %d x %d: ", out->m_meta.height, out->m_meta.width, out->m_meta.channel);
        printf("LS: %d SS: %d Size: %d\n", out->m_meta.lineStride, out->m_meta.surfaceStride, out->m_meta.size);
    }

    // Allocate the buffer
    out->m_pData = malloc(out->m_meta.size);

    // Copy the data
    memset(out->m_pData, 0, out->m_meta.size);

    ibuf = static_cast<uint8_t*>(in->m_pData);
    obuf = static_cast<uint8_t*>(out->m_pData);

    for (uint32_t y=0; y < in->m_meta.height; y++)
    {
        for (uint32_t x=0; x < in->m_meta.width; x++)
        {
            for (uint32_t z=0; z < in->m_meta.channel; z++)
            {
                int32_t ioffset = in->getAddrOffset(x, y, z);
                int32_t ooffset = out->getAddrOffset(x, y, z);

                if (ioffset < 0)
                    //ORIGINATE_ERROR(NvDlaError_BadParameter);
			        printf("ERROR3\n");
                if (ooffset < 0)
                    //ORIGINATE_ERROR(NvDlaError_BadParameter);
			        printf("ERROR4\n");
                uint8_t* inp = ibuf + ioffset;

                /*if (outTensorDesc->dataType == NVDLA_DATA_TYPE_HALF)
                {
                    half_float::half* outp = reinterpret_cast<half_float::half*>(obuf + ooffset);
                    //*outp = static_cast<half_float::half>((float(*inp) - float(appArgs->mean[z]))/appArgs->normalize_value);
                }
                else*/ 
		if (outTensorDesc->dataType == NVDLA_DATA_TYPE_INT8)
                {
                    char* outp = reinterpret_cast<char*>(obuf + ooffset);
			
                    //*outp = char(*inp); // no normalization happens
                    // compress the image from [0-255] to [0-127]
                    *outp = static_cast<int8_t>(std::floor((*inp * 127.0/255.0) + 0.5f));
                    printf("ioffset %d ooffset %d \n",ioffset,ooffset);
                }
            }
        }
    }
	//
	NvDlaImage *pBuffer=(NvDlaImage *)malloc(out->m_meta.size);
	memcpy(pBuffer,out->m_pData, out->m_meta.size);
	fwrite(pBuffer,1,out->m_meta.size,fp1);
	fclose(fp1);
	
}




	
int main() 
{
	printf("start\n");
	const TestAppArgs* appArgs;
	void** pImgBuffer;
	
	nvdla::IRuntime::NvDlaTensor tensorDesc;
	printf("start1\n");
	tensorDesc.dims.w = 28;
	tensorDesc.dims.h = 28;
	tensorDesc.dims.c = 1;
	tensorDesc.bufferSize = 6272;
	tensorDesc.dataType = 4;
	tensorDesc.pixelFormat = 37;
	tensorDesc.stride[1] =1;
	NvDlaImage *out_data = new NvDlaImage();

	printf("start2\n");
	NvDlaImage  tensorImage;

	printf("1\n");
	JPEG2DIMG("9.jpg", out_data,&tensorDesc);

	

	printf("2\n");

	//tensorImage = i.inputImage;
	printf("input Image\n");
	tensorImage.m_meta.surfaceFormat =(NvDlaImage::PixelFormat) 41;
	tensorImage.m_meta.width = 28;
	tensorImage.m_meta.height = 28;
	tensorImage.m_meta.channel = 1;
	tensorImage.m_meta.lineStride = 224; //
	tensorImage.m_meta.surfaceStride =6272;
	tensorImage.m_meta.size = 6272;

	createImageCopy(appArgs, out_data, &tensorDesc, &tensorImage);

	printf("3\n");

	printf("4\n");

    
}
