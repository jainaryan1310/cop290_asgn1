#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "warp_crop.h"

using namespace cv;
using namespace std;

// Defining a vector of points to store the coordinates of the corners
// of the rectangle defining the region of interest
// *globally defining the pts_src vector as it is used by the call back function


vector<Point2f> pts_src;
vector<Point2f> pts_dst;


void LeftClick(int event, int x, int y, int flags, void* param) {
	
	if (event == EVENT_LBUTTONDOWN) {
		Point2f p = Point2f(x,y);
		pts_src.push_back(p);
	}

}


int getCropCoordinates(Mat grayImg) {

	//Create a window
	namedWindow("Original Frame", 1);

	//set the callback function for any mouse event
	setMouseCallback("Original Frame", LeftClick, NULL);
	
	//show the image
	imshow("Original Frame", grayImg);

	// Wait until user press some key and then close
	waitKey(0);
	destroyWindow("Original Frame");

	//error handling if the user clicks at less or more than 4 points
	if (pts_src.size() < 4) {
		cout << "Error: You have clicked on too little points. Please select 4 points on the image, in anticlockwise direction." << endl;
		return -1;
	}

	else if (pts_src.size() > 4) {
		cout << "Error: You have clicked on too many points. Please select only 4 points on the image, in anticlockwise direction." << endl;
		return -1;
	}

	// Defining a vector of points to store the coordinates of the corners
	// of the rectangle defining destination of the region of interest in the
	// warped image
	pts_dst.push_back(Point2f(472,52));
	pts_dst.push_back(Point2f(472,830));
	pts_dst.push_back(Point2f(800,830));
	pts_dst.push_back(Point2f(800,52));
	return 0;
}


Mat warp(Mat grayImg){

	// defining and calculating the homography matrix for the transformation
	Mat h = findHomography(pts_src, pts_dst);

	// defining and generating the output image by warping the perspective
	Mat warpedImg;

	warpPerspective(grayImg, warpedImg, h, grayImg.size());

	return warpedImg;

}


Mat crop(Mat warpedImg) {

	//defining the region of the warped image to be cropped
	Rect cropped_region(472, 52, 328, 778);

	// cropping the warped image
	Mat croppedImg = warpedImg(cropped_region);

	return croppedImg;

}
