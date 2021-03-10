#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <fstream>
#include "warp_crop.h"
#include "dynamic.h"
#include "queue.h"

using namespace cv;
using namespace std;

int main(int argc, char* argv[]){

	if (argc == 1) {
		cout << "Error: No image has been input to the executable.\n"
			 << "Please write the name of the image file you want to use, as a command line argument.\n"
			 << "For example: ./asgn1 trafficvideo.mp4 empty.jpg" << endl;
		return -1;
	}

	if (argc > 3) {
		cout << "Error: More arguments than needed. \n"
			 << "Please enter the name of only one image file. \n"
			 << "For example: ./asgn1 trafficvideo.mp4 empty.jpg" << endl;
			 return -1;
	}

	ofstream outfile1;
	ofstream outfile2;
	outfile1.open("out.txt");
	outfile2.open("out.csv");

	Mat emptyImg = imread(argv[2]);

	getCropCoordinates(emptyImg);

	VideoCapture capture(argv[1]);

    Mat frame, fgMask, grayFrame;

	Ptr<BackgroundSubtractor> pBackSub;
    pBackSub = createBackgroundSubtractorMOG2();

    if (!capture.isOpened()){
        //error in opening the video input
        cerr << "Unable to open the video frame" << endl;
        return -1;
    }

    Mat warpedImg = warp(emptyImg);
    Mat croppedImg = crop(warpedImg);

    pBackSub->apply(croppedImg, fgMask, 0);

    capture >> frame;
    if (frame.empty()){
    	cerr << "Empty Video" << endl;
		return -1;
    }

    cvtColor(frame, grayFrame, COLOR_BGR2GRAY);

	Mat prev = crop(warp(grayFrame));

    int framenum = 0;

    while (true) {

    	framenum++;

        capture >> frame;
        capture >> frame;
        capture >> frame;
        if (frame.empty())
            break;

        Mat warpedImg = warp(frame);
        Mat croppedImg = crop(warpedImg);

        //update the background model
        pBackSub->apply(croppedImg, fgMask, 0);
        
        Mat thresh;
        threshold( fgMask, thresh, 200, 255, 3);

        float white = countNonZero(thresh);
        float total = thresh.total();
        float queue_density = white/total;

        cvtColor(frame, grayFrame, COLOR_BGR2GRAY);

        Mat next = crop(warp(grayFrame));

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
		float max_movement = hsv8.total()*255.0;
		float moving_density = movement/max_movement;

        int frameNum = framenum * 3;

        
        cout << frameNum << ", " << queue_density << ", " << moving_density << endl;
        outfile1 << frameNum << " " << queue_density << " " << moving_density << endl;


        //show the backfround subtraction output
        imshow("FG Mask", thresh);

        cvtColor(hsv8, bgr, COLOR_HSV2BGR);

        //show the optical flow output
		imshow("frame", bgr);

        //get the input from the keyboard for quitting
        int keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;

        prev = next;
    }

}