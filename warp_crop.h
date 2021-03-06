#ifndef WARP_CROP_H
#define WARP_CROP_H

#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


void LeftClick(int event, int x, int y, int flags, void* param);
int getCropCoordinates(Mat grayImg);
Mat warp(Mat grayImg);
Mat crop(Mat warpedImg);

#endif 