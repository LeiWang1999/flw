#ifndef JPEG2DIMG_H
#define JPEG2DIMG_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "DlaImage.h"




    
void JPEG2DIMG(const char * inputFileName, struct Metadata in,uint8_t *out);

#endif



