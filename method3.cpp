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


using namespace cv;
using namespace std;

vector<float> results[36];

struct method3_args {
    Mat EmptyImg;
    VideoCapture capture;
    int thread_no;
    int total_threads;
};

void *method3(void *thread_arg){

    struct method3_args *method3_arg;
    method3_arg = (struct method3_args *) thread_arg;

    Ptr<BackgroundSubtractor> pBackSub;
    pBackSub = createBackgroundSubtractorMOG2();

    if (!method3_arg->capture.isOpened()){
        //error in opening the video input
        cerr << "Unable to open the video frame" << endl;
    }

    Mat frame, fgMask, grayFrame;

    Mat warpedImg = warp(method3_arg->EmptyImg);
    Mat croppedImg = crop(warpedImg);

    int original_width;
    original_width = croppedImg.size().width;

    Mat finalCroppedImg;

    Rect roi(original_width*((method3_arg->thread_no)-1)/(method3_arg->total_threads), 0, original_width/method3_arg->total_threads, croppedImg.size().height);
    finalCroppedImg = croppedImg(roi);

    pBackSub->apply(croppedImg, fgMask, 0);

    int result_no = method3_arg->total_threads*(method3_arg->total_threads-1)/2 + method3_arg->thread_no - 1;

    while (true) {
        method3_arg->capture >> frame;
        if (frame.empty())
            break;

        Mat warpedImg = warp(frame);
        Mat croppedImg = crop(warpedImg);

        finalCroppedImg = croppedImg(roi);

        //update the background model
        pBackSub->apply(finalCroppedImg, fgMask, 0);
        
        Mat thresh;
        threshold( fgMask, thresh, 200, 255, 3);

        float white = countNonZero(thresh);
        float total = thresh.total();
        float density = white/total;

        stringstream ss;
        ss << method3_arg->capture.get(CAP_PROP_POS_FRAMES);
        string frameNumberString = ss.str();
        cout << frameNumberString << " " << density << endl;
        results[result_no].push_back(density);

        //show the backfround subtraction output
        imshow("FG Mask", thresh);

        //get the input from the keyboard for quitting
        int keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;
    }
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

    Mat emptyImg = imread("empty.jpg");

    getCropCoordinates(emptyImg);

    Mat warpedImg = warp(emptyImg);
    Mat croppedImg = crop(warpedImg);
    int original_width = croppedImg.size().width;

    ofstream outfile1;
    outfile1.open("method3_runtimes.csv");

    for (int x = 1; x < 9; x++) {

        time_t start, end;

        time(&start);

        pthread_t threads[x];
        struct method3_args ta[x];

        for (int i = 0 ; i < x ; ++i) {

            VideoCapture capture(argv[1]);

            ta[i].EmptyImg = emptyImg;
            ta[i].capture = capture;
            ta[i].thread_no = i+1;
            ta[i].total_threads = x;
            int t = pthread_create(&threads[i], NULL, method3, (void *)&ta[i]);
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


        ofstream outfile;
        outfile.open("m3thread"+to_string(x)+".csv");

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
