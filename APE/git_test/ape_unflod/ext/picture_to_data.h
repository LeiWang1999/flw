#ifndef  PICTURE_TO_DATA_H
#define  PICTURE_TO_DATA_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "DlaImage.h"


    
void picture_to_data(const char * inputFileName, struct Metadata in,uint8_t *out);

#endif



