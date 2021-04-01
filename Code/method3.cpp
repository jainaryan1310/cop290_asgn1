#include "warp_crop.h"
#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/bgsegm.hpp>
#include <fstream>

using namespace std;
using namespace cv;

vector<float> results[36];

struct method3_args {
    Mat roiEmptyFrame;
    Mat roiFrame;
    int thread_no;
    int total_threads;
};

void *method3(void *thread_arg){

    struct method3_args *method3_arg;
    method3_arg = (struct method3_args *) thread_arg;

    Ptr<BackgroundSubtractor> pBackSub;
    pBackSub = createBackgroundSubtractorMOG2();

    Mat fgMask;

    pBackSub->apply(method3_arg->roiEmptyFrame, fgMask, 0);

    pBackSub->apply(method3_arg->roiFrame, fgMask, 0);

    Mat thresh;
    threshold(fgMask, thresh, 50, 255, 3);

    float white = countNonZero(thresh);
    float total = thresh.total();
    float density = white/total;

    cout << density << endl;
    int result_no = method3_arg->total_threads*(method3_arg->total_threads-1)/2+method3_arg->thread_no-1;
    results[result_no].push_back(density);
}

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

    Mat grayEmptyImg;
    cvtColor(emptyImg, grayEmptyImg, COLOR_BGR2GRAY);

    getCropCoordinates(grayEmptyImg);

    Mat warpedEmptyImg = warp(grayEmptyImg);
    Mat croppedEmptyImg = crop(warpedEmptyImg);
    int original_width = croppedEmptyImg.size().width;

    Mat frame, grayFrame;

    ofstream outfile1;
    outfile1.open("method3_runtimes.csv");

    for(int x = 1; x < 9; x++){

        time_t start, end;
        time(&start);

        pthread_t threads[x];
        struct method3_args ta[x];

        VideoCapture capture(argv[1]);

        while(true){

            capture >> frame;

            if (frame.empty())
            {
                cout << "111111111111111111111111" << endl;
                break;
            }

            cvtColor(frame, grayFrame, COLOR_BGR2GRAY);

            Mat warpedImg = warp(grayFrame);
            Mat croppedImg = crop(warpedImg);

            for(int i = 0; i<x; i++){

                Rect roi(original_width*i/x, 0, original_width/x, croppedImg.size().height);
                Mat roiEmptyFrame = croppedEmptyImg(roi);
                Mat roiFrame = croppedImg(roi);

                ta[i].roiEmptyFrame = roiEmptyFrame;
                ta[i].roiFrame = roiFrame;
                ta[i].thread_no = i+1;
                ta[i].total_threads = x;
                int t = pthread_create(&threads[i], NULL, method3, (void *)&ta[i]);
                if (t != 0) {
                    cerr << "Error in thread creation: " << t << endl;
                }
            }
            for(int i = 0 ; i < x; ++i) {
                void* status;
                int t = pthread_join(threads[i], &status);
                if (t != 0) {
                    cout << "Error in thread join: " << t << endl;
                  }
            }
        }


        ofstream outfile;
        outfile.open("method3file"+to_string(x)+".csv");

        float density;
        for(int f = 0; f<results[x*(x-1)/2].size(); f++){
            density = 0.0;
            for(int i = 0; i<x; i++){
                density = density + results[x*(x-1)/2+i].at(f); 
            }
            outfile << to_string(f+1) << ", " << to_string(density/x) << endl;
        }

        time(&end);
        double time_taken = double(end - start);
        outfile1 << to_string(x) << ", " << fixed << time_taken << setprecision(5) << endl;

    }
}