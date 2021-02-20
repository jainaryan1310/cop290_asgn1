#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// Defining a vector of points to store the coordinates of the corners
// of the rectangle defining the region of interest
// *globally defining the pts_src vector as it is used by the call back function
vector<Point2f> pts_src;

void LeftClick(int event, int x, int y, int flags, void* param)
{
	
	if(event == EVENT_LBUTTONDOWN){
		Point2f p = Point2f(x,y);
		pts_src.push_back(p);
	}

}

int main(){
	// Read image from file 
	Mat img = imread("empty.jpg");

	// Converting the image to grayscale
	Mat grayImg;

	cvtColor(img, grayImg, COLOR_BGR2GRAY);

	// if the program fails to read the mentioned image
	if(img.empty()){
		// return an error message and terminate the process
		cout 
			<< "Error : The image file could not be correctly read,/n"
			<< "plaese check the file path or whatever is suitable as/n"
			<< "per the makefile" 
			<< endl;
	
		// exit the process
		return -1;
	}

	//Create a window
	namedWindow("Original Frame", 1);

	//set the callback function for any mouse event
	setMouseCallback("Original Frame", LeftClick, NULL);
	
	//show the image
	imshow("Original Frame", grayImg);

	// Wait until user press some key and then close
	waitKey(0);
	destroyWindow("Original Frame");


	// Defining a vector of points to store the coordinates of the corners
	// of the rectangle defining destination of the region of interest in the
	// warped image
	vector<Point2f> pts_dst;
	pts_dst.push_back(Point2f(700,0));
	pts_dst.push_back(Point2f(700,1000));
	pts_dst.push_back(Point2f(1200,1000));
	pts_dst.push_back(Point2f(1200,0));

	
	// defining and calculating the homography matrix for the transformation
	Mat h = findHomography(pts_src, pts_dst);

	// defining and generating the output image by warping the perspective
	Mat warpedImg;

	warpPerspective(img, warpedImg, h, img.size());

	// show the warped image
	imshow("Projected Frame", warpedImg);

	// Wait until user press some key and then close
	waitKey(0);
	destroyWindow("Projected Frame");

	//defining the region of the warped image to be cropped
	Rect cropped_region(700, 0, 500, 1000);

	// cropping the warped image
	Mat croppedImg = warpedImg(cropped_region);
	
	// showing the cropped image
	imshow("Cropped Frame", croppedImg);

	// Wait until user press some key and then close
	waitKey(0);
	destroyWindow("Cropped Frame");

	return 0;



}