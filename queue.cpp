#include "warp_crop.h"
#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/bgsegm.hpp>
#include "queue.h"


using namespace cv;
using namespace std;


void queue_density(Mat emptyImg, VideoCapture capture) {

    Ptr<BackgroundSubtractor> pBackSub;
    pBackSub = createBackgroundSubtractorMOG2();

    if (!capture.isOpened()){
        //error in opening the video input
        cerr << "Unable to open the video frame" << endl;
        return;
    }

    Mat frame, fgMask, grayFrame;

    Mat warpedImg = warp(emptyImg);
    Mat croppedImg = crop(warpedImg);

    pBackSub->apply(croppedImg, fgMask, 0);

    while (true) {
        capture >> frame;
        if (frame.empty())
            break;

        Mat warpedImg = warp(frame);
        Mat croppedImg = crop(warpedImg);

        //update the background model
        pBackSub->apply(croppedImg, fgMask, 0);
        
        Mat thresh;
        threshold( fgMask, thresh, 200, 255, 3);

        float white = countNonZero(thresh);
        float total = thresh.total();
        float density = white/total;

        stringstream ss;
        ss << capture.get(CAP_PROP_POS_FRAMES);
        string frameNumberString = ss.str();
        cout << frameNumberString << " " << density << endl;

        //show the backfround subtraction output
        imshow("FG Mask", thresh);

        //get the input from the keyboard for quitting
        int keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;
    }
}

/*
int main(int argc, char* argv[])
{
    queue_density(argv[1]);
    return 0;
}
*/