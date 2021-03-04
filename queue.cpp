#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>


using namespace cv;
using namespace std;




int main(int argc, char* argv[])
{
    //create Background Subtractor objects
    Ptr<BackgroundSubtractor> pBackSub;
    pBackSub = createBackgroundSubtractorMOG2();

    VideoCapture capture(argv[1]);

    if (!capture.isOpened()){
        //error in opening the video input
        cerr << "Unable to open the video frame" << endl;
        return 0;
    }

    Mat frame, fgMask;

    frame = imread("empty.jpg");
    pBackSub->apply(frame, fgMask, 0);

    while (true) {
        capture >> frame;
        if (frame.empty())
            break;

        //update the background model
        pBackSub->apply(frame, fgMask, 0);
        //absdiff(frame, background, fgMask);

        //rectangle(frame, cv::Point(10, 2), cv::Point(100,20), cv::Scalar(255,255,255), -1);
        stringstream ss;
        ss << capture.get(CAP_PROP_POS_FRAMES);
        string frameNumberString = ss.str();
        cout << frameNumberString << endl;

        //show the current frame and the fg masks
        imshow("Frame", frame);
        imshow("FG Mask", fgMask);

        //get the input from the keyboard for quitting
        int keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;
    }

    return 0;
}