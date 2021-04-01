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
#include "pthread.h"
#include <string>

using namespace cv;
using namespace std;

float output[8][5737];

struct thread_data {
	string video;
	Mat image;
	int num_threads;
	int id;
};


void* call_queue_density(void *thread_arg) {

	struct thread_data *my_data;
    my_data = (struct thread_data *) thread_arg;

	VideoCapture capture(my_data->video);
	if (!capture.isOpened()) {
            //error in opening the video input
            cerr << "Unable to open the video frame" << endl;
            pthread_exit(NULL);
    }

    Mat frame, fgMask;

    Ptr<BackgroundSubtractor> pBackSub;
    pBackSub = createBackgroundSubtractorMOG2();

    Mat warpedImg = warp(my_data->image);
    Mat croppedImg = crop(warpedImg);
    pBackSub->apply(croppedImg, fgMask, 0);

    int framenum = (5737/my_data->num_threads)*(my_data->id);

    capture.set(CAP_PROP_POS_FRAMES, framenum);
    for (int j = 0; j < (5737/my_data->num_threads); j++) {
    	capture >> frame;
        if (frame.empty()) {
            break;
        }
        warpedImg = warp(frame);
        croppedImg = crop(warpedImg);

        //update the background model
        pBackSub->apply(croppedImg, fgMask, 0);
        Mat thresh;
        threshold(fgMask, thresh, 200, 255, 3);

        float white = countNonZero(thresh);
        float total = thresh.total();
        float density = white/total;
        cout << my_data->num_threads << " " << framenum << " " << density << endl;
        if (framenum >= 5737) {
            break;
        }
        output[(my_data->num_threads)-1][framenum] = density;
        framenum++;
    }

    pthread_exit(NULL);

}


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

	Mat emptyImg = imread(argv[2]);
	getCropCoordinates(emptyImg);

	
	for (int x = 1; x < 9; x++) {

		ofstream outfile2;

		if (x == 1) {
    	outfile2.open("m4file1.csv");
    	}
    	else if (x == 2) {
    	    outfile2.open("m4file2.csv");
    	}
    	else if (x == 3) {
    	    outfile2.open("m4file3.csv");
    	}
    	else if (x == 4) {
    	    outfile2.open("m4file4.csv");
    	}
    	else if (x == 5) {
    	    outfile2.open("m4file5.csv");
    	}
    	else if (x == 6) {
    	    outfile2.open("m4file6.csv");
    	}
    	else if (x == 7) {
    		outfile2.open("m4file7.csv");
    	}
    	else {
    	    outfile2.open("m4file8.csv");
    	}

        time_t start, end;

        time(&start);

        pthread_t threads[x];
        struct thread_data td[x];

        for (int i = 0 ; i < x ; ++i) {
        	td[i].video = argv[1];
        	td[i].image = emptyImg;
        	td[i].num_threads = x;
        	td[i].id = i;
    	   	int t = pthread_create(&threads[i], NULL, call_queue_density, (void *)&td[i]);
    	   	if (t != 0) {
    	       	cout << "Error in thread creation: " << t << endl;
    	   	}
    	}
 
    	for(int i = 0 ; i < x; ++i) {
    	   	void* status;
    	   	int t = pthread_join(threads[i], &status);
    	   	if (t != 0) {
    	       	cout << "Error in thread join: " << t << endl;
    	    }
    	}

        for (int i = 0; i < 5737; i++) {
        	outfile2 << i+1 << "," << output[x-1][i] << endl;
        }

        time(&end);
        double time_taken = double(end - start);
        outfile2 << fixed << time_taken << setprecision(5);

	}
}