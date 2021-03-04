#include "asgn1_sub1.h"
#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>


using namespace cv;
using namespace std;


void queue_density(string video) {
    Ptr<BackgroundSubtractor> pBackSub;
    pBackSub = createBackgroundSubtractorMOG2();
    VideoCapture capture(video);
    if (!capture.isOpened()){
        //error in opening the video input
        cerr << "Unable to open the video frame" << endl;
        return;
    }

    Mat frame, fgMask;

    frame = imread("empty.jpg");
    Mat grayFrame;
    cvtColor(frame, grayFrame, COLOR_BGR2GRAY);

    getCropCoordinates(grayFrame);
    Mat warpedImg = warp(grayFrame);
    Mat croppedImg = crop(warpedImg);

    pBackSub->apply(croppedImg, fgMask, 0);

    while (true) {
        capture >> frame;
        if (frame.empty())
            break;

        Mat grayFrame;
        cvtColor(frame, grayFrame, COLOR_BGR2GRAY);

        Mat warpedImg = warp(grayFrame);
        Mat croppedImg = crop(warpedImg);

        //update the background model
        pBackSub->apply(croppedImg, fgMask, 0);

        float white = countNonZero(fgMask);
        float total = fgMask.total();
        float density = white/total;

        stringstream ss;
        ss << capture.get(CAP_PROP_POS_FRAMES);
        string frameNumberString = ss.str();
        cout << frameNumberString << " " << density << endl;

        //show the backfround subtraction output
        imshow("FG Mask", fgMask);

        //get the input from the keyboard for quitting
        int keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;
    }
}


int main(int argc, char* argv[])
{
    queue_density(argv[1]);
    return 0;
}