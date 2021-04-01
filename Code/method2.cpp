#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <fstream>
#include "warp_crop.h"

using namespace cv;
using namespace std;

int main(int argc, char* argv[]){

	if (argc == 1) {
		cout << "Error: No image has been input to the executable.\n"
			 << "Please write the name of the image file you want to use, as a command line argument.\n"
			 << "For example: ./method2 trafficvideo.mp4 empty.jpg" << endl;
		return -1;
	}

	if (argc > 3) {
		cout << "Error: More arguments than needed. \n"
			 << "Please enter the name of only one image file. \n"
			 << "For example: ./method2 trafficvideo.mp4 empty.jpg" << endl;
			 return -1;
	}

	Mat emptyImg = imread(argv[2]);

	getCropCoordinates(emptyImg);

	ofstream outfile;
	outfile.open("method2_runtimes.csv");

	for(double factor = 1.0; factor <= 10.0; factor = factor + 1.0){

		time_t start, end;

		time(&start);
	
		ofstream outfile1;

		string outfilename;

		outfile1.open("m2Factor"+to_string((int) factor)+".csv");

		VideoCapture capture(argv[1]);

		Mat frame, fgMask;

		Ptr<BackgroundSubtractor> pBackSub;
	    pBackSub = createBackgroundSubtractorMOG2();

	    if (!capture.isOpened()){
	        //error in opening the video input
	        cerr << "Unable to open the video frame" << endl;
	        return -1;
	    }

	    Mat resizedEmptyImg;

	    Mat warpedImg = warp(emptyImg);
	    Mat croppedImg = crop(warpedImg);

	    resize(croppedImg, resizedEmptyImg, Size(), 1.0/factor, 1.0/factor);

	    pBackSub->apply(resizedEmptyImg, fgMask, 0);

	    int framenum = 0;

	    while (true) {

	    	framenum++;

	    	capture >> frame;
	    	if (frame.empty())
	    	{
	    		break;
	    	}

	    	Mat resizedFrame;

			warpedImg = warp(frame);
	    	croppedImg = crop(warpedImg);

        	resize(croppedImg, resizedFrame, Size(), 1.0/factor, 1.0/factor);

        	//update the background model
	        pBackSub->apply(resizedFrame, fgMask, 0);
	        
	        Mat thresh;
	        threshold( fgMask, thresh, 200, 255, 3);

	        float white = countNonZero(thresh);
	        float total = thresh.total();
	        float queue_density = white/total;

	        cout << framenum << ", " << queue_density << endl;

	        outfile1 << framenum << ", " << queue_density << endl;

	    }

	    time(&end);
        double time_taken = double(end - start);
        outfile << fixed << time_taken << setprecision(5) << endl;



	}

}
