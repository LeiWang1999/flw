#ifndef JPEG2DIMG_H
#define JPEG2DIMG_H

#include <iostream>
#include <stdio.h>
#include <cstdio> 
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "half.h"
#include "RuntimeTest.h"
#include "NvDlaType.h"
#include "DlaImage.h"
#include "dlaerror.h"
#include "IRuntime.h"
extern "C" {
#include "jpeglib.h"
}
using namespace std;
using namespace nvdla;
using namespace half_float;
    
void JPEG2DIMG(const char * inputFileName, NvDlaImage* in,uint8_t *out);

#endif



