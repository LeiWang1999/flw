#ifndef JPEG2DIMG_H
#define JPEG2DIMG_H

#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <typeinfo>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
       
void JPEG2DIMG(Mat Image,uint8_t *out);

#endif



