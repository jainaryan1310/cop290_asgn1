#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "warp_crop.h"
#include "dynamic.h"

using namespace cv;
using namespace std;

void dynamic_density(VideoCapture capture) {

	Mat frame, grayframe;

	capture >> frame;
	if (frame.empty()){
		cerr << "Unable to open the video frame" << endl;
		return;
	}
	cvtColor(frame, grayframe, COLOR_BGR2GRAY);

	Mat prev = crop(warp(grayframe));

	
	while(1){

		capture >> frame;
		if (frame.empty()){
			break;
		}
		cvtColor(frame, grayframe, COLOR_BGR2GRAY);

		Mat next = crop(warp(grayframe));

		Mat flow(prev.size(), CV_32FC2);

		calcOpticalFlowFarneback(prev, next, flow, 0.5, 3, 15, 3, 5, 1.2, 0);

		Mat flow_parts[2];

		split(flow, flow_parts);

		Mat magnitude, angle, magn_norm;

		cartToPolar(flow_parts[0], flow_parts[1], magnitude, angle, true);

		normalize(magnitude, magn_norm, 0.0f, 1.0f, NORM_MINMAX);

		angle *= ((1.f / 360.f)*(180.f / 255.f));

		Mat _hsv[3], hsv, hsv8, bgr;

		_hsv[0] = angle;
		_hsv[1] = Mat::ones(angle.size(), CV_32F);
		_hsv[2] = magn_norm;

		merge(_hsv, 3, hsv);

		hsv.convertTo(hsv8, CV_8U, 255.0);

		float movement = sum(hsv8)[2];
		float total = hsv8.total()*255.0;
		float density = movement/total;

		stringstream ss;
        ss << capture.get(CAP_PROP_POS_FRAMES);
        string frameNumberString = ss.str();

		cout << frameNumberString << " : " << density << endl;

		cvtColor(hsv8, bgr, COLOR_HSV2BGR);

		imshow("frame", bgr);

		int keyboard = waitKey(30);

		if (keyboard == 'q' || keyboard == 27){
			break;
		}

		prev = next;

	}


}
