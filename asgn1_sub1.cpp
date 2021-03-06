#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "warp_crop.h"

using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {

	if (argc == 1) {
		cout << "Error: No image has been input to the executable.\n"
			 << "Please write the name of the image file you want to use, as a command line argument.\n"
			 << "For example: ./asgn1 traffic.jpg" << endl;
		return -1;
	}

	if (argc > 2) {
		cout << "Error: More arguments than needed. \n"
			 << "Please enter the name of only one image file. \n"
			 << "For example: ./asgn1 traffic.jpg" << endl;
			 return -1;
	}

	// Saving the name of the input image in a string
	string filename = argv[1];

	Mat img = imread(filename);

	if (img.empty()) {
		// return an error message and terminate the process
		cout << "Error: The image file could not be correctly read. \n"
			 << "Please check the file path or extension. \n"
			 << endl;
	
		// exit the process
		return -1;
	}

	Mat grayImg;

	cvtColor(img, grayImg, COLOR_BGR2GRAY);

	int a = getCropCoordinates(grayImg);

	if (a == 0) {
		Mat warpedImg = warp(grayImg);
		// saving the warped image under the name "warped_<original_image_name>.jpg"
		imwrite("warped_"+filename, warpedImg);

		// show the warped image
		imshow("Projected Frame", warpedImg);

		// Wait until user press some key and then close
		waitKey(0);
		destroyWindow("Projected Frame");

		Mat croppedImg = crop(warpedImg);

		// saving the cropped image under the name "cropped_<original_image_name>.jpg"
		imwrite("cropped_"+filename, croppedImg);

		// showing the cropped image
		imshow("Cropped Frame", croppedImg);

		// Wait until user press some key and then close
		waitKey(0);
		destroyWindow("Cropped Frame");
		return 0;
	}

	else {
		return -1;
	}

}