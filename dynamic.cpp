#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "asgn1_sub1.h"

using namespace cv;
using namespace std;

//vector<Point2f> pts_src;
//vector<Point2f> pts_dst;


int main(int argc, char* argv[]) {

	Mat emptyImg = imread("empty.jpg");

	Mat emptyGrayFrame;

	cvtColor(emptyImg, emptyGrayFrame, COLOR_BGR2GRAY);

	getCropCoordinates(emptyGrayFrame);

	VideoCapture capture(samples::findFile(argv[1]));

	Mat prev = crop(warp(emptyGrayFrame));

	int i = 0;
	
	while(1){

		i++;
		
		Mat frame, grayframe;

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

		cout << i/15 << " : " << sum(hsv8)[2] << endl;

		cvtColor(hsv8, bgr, COLOR_HSV2BGR);

		imshow("frame", bgr);

		int keyboard = waitKey(30);

		if (keyboard == 'q' || keyboard == 27){
			break;
		}

		prev = next;

	}


}