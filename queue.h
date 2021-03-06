#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "warp_crop.h"

using namespace cv;
using namespace std;

void queue_density(Mat emptyGrayFrame, VideoCapture capture);

#endif 