#include "warp_crop.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/bgsegm.hpp>
#include <time.h>
#include "queue.h"

using namespace cv;
using namespace std;


int main(int argc, char* argv[]) {

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

	bool previous = false;

	Mat emptyImg = imread(argv[2]);

	getCropCoordinates(emptyImg);


	for (int x = 1; x < 10; x++) {
        VideoCapture capture(argv[1]);

        Mat frame, fgMask;

        Ptr<BackgroundSubtractor> pBackSub;
        pBackSub = createBackgroundSubtractorMOG2();

        if (!capture.isOpened()){
            //error in opening the video input
            cerr << "Unable to open the video frame" << endl;
            return -1;
        }

        Mat warpedImg = warp(emptyImg);
        Mat croppedImg = crop(warpedImg);

        ofstream outfile2;
        pBackSub->apply(croppedImg, fgMask, 0);

        if (x == 1) {
            outfile2.open("m1base.csv");
        }
        else if (x == 2) {
            outfile2.open("m1file1.csv");
        }
        else if (x == 3) {
            outfile2.open("m1file2.csv");
        }
        else if (x == 4) {
            outfile2.open("m1file3.csv");
        }
        else if (x == 5) {
            outfile2.open("m1file4.csv");
        }
        else if (x == 6) {
            outfile2.open("m1file5.csv");
        }
        else if (x == 7) {
            outfile2.open("m1file6.csv");
        }
        else if (x == 8) {
            outfile2.open("m1file7.csv");
        }
        else {
            outfile2.open("m1file8.csv");
        }

        time_t start, end;

        time(&start);

        int framenum = 0;
        previous = false;

        while (true) {

    	    framenum++;

    	    for (int i = 0; i < x; i++) {
                //cout << x << " " << framenum << " " << i << endl;
    		    capture >> frame;
        	    if (frame.empty()) {
                    previous = true;
                    break;
                }  
    	    }

            if (previous) {
                break;
            }
    
            Mat warpedImg = warp(frame);
            Mat croppedImg = crop(warpedImg);

            //update the background model
            pBackSub->apply(croppedImg, fgMask, 0);
        
            Mat thresh;
            threshold(fgMask, thresh, 200, 255, 3);

            float white = countNonZero(thresh);
            float total = thresh.total();
            float queue_density = white/total;
    
            int frameNum = framenum * x;

            imshow("FG Mask", thresh);

            //get the input from the keyboard for quitting
            int keyboard = waitKey(30);
            if (keyboard == 'q' || keyboard == 27)
                break;

            for (int s = x-1; s >= 0; s--) {
                //cout << frameNum - s << ", " << queue_density << endl;
            	outfile2 << frameNum - s << "," << queue_density << endl;
            }
            //cout << frameNum << ", " << queue_density << endl;
            //outfile2 << frameNum << "," << queue_density << endl;
        }
        time(&end);
        double time_taken = double(end - start);
        outfile2 << fixed << time_taken << setprecision(5);
    }

    return 0;
}