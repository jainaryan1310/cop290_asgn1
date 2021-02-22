#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// Defining a vector of points to store the coordinates of the corners
// of the rectangle defining the region of interest
// *globally defining the pts_src vector as it is used by the call back function
vector<Point2f> pts_src;

void LeftClick(int event, int x, int y, int flags, void* param) {
	
	if (event == EVENT_LBUTTONDOWN) {
		Point2f p = Point2f(x,y);
		pts_src.push_back(p);
	}

}

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

	// Read image from file
	Mat img = imread(argv[1]);

	// Converting the image to grayscale
	Mat grayImg;

	cvtColor(img, grayImg, COLOR_BGR2GRAY);

	// if the program fails to read the mentioned image
	if (img.empty()) {
		// return an error message and terminate the process
		cout << "Error: The image file could not be correctly read. \n"
			 << "Please check the file path or extension. \n"
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
	vector<Point2f> pts_dst;
	pts_dst.push_back(Point2f(472,52));
	pts_dst.push_back(Point2f(472,830));
	pts_dst.push_back(Point2f(800,830));
	pts_dst.push_back(Point2f(800,52));

	
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
	Rect cropped_region(472, 52, 328, 778);

	// cropping the warped image
	Mat croppedImg = warpedImg(cropped_region);
	
	// showing the cropped image
	imshow("Cropped Frame", croppedImg);

	// Wait until user press some key and then close
	waitKey(0);
	destroyWindow("Cropped Frame");

	return 0;
}