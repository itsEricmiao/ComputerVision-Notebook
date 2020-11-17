/**
 * @function goodFeaturesToTrack_Demo.cpp
 * @brief Demo code for detecting corners using Shi-Tomasi method
 * @author OpenCV team
 */

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

/// Global variables
Mat src, src_gray;

int maxCorners = 1;
int maxTrackbar = 100;

RNG rng(12345);
const char* source_window = "Image";

/// Function header
void goodFeaturesToTrack_Demo( int, void* );

/**
 * @function main
 */
int main( int argc, char** argv )
{
    //--- INITIALIZE VIDEOCAPTURE
    VideoCapture cap;
    cap.open(0);                        // Use default camera

    // check if we succeeded
    if (!cap.isOpened()) {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }


    //--- GRAB AND WRITE LOOP
    cout << "Start grabbing" << endl
        << "Press any key to terminate" << endl;



 for (;;)
    {
        // wait for a new frame from camera and store it into 'frame'
        cout<<src.type()<<endl;
        cap.read(src);

        cvtColor( src, src_gray, COLOR_BGR2GRAY );

        // check if we succeeded
        if (src.empty()) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        // show live and wait for a key with timeout long enough to show images
        //imshow("Live", frame);
        goodFeaturesToTrack_Demo( 0, 0 );
        if (waitKey(1000) >= 0)   // Adjust parameter to delay time between frames
            break;
     }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;

}

/**
 * @function goodFeaturesToTrack_Demo.cpp
 * @brief Apply Shi-Tomasi corner detector
 */
void goodFeaturesToTrack_Demo( int, void* )
{
    /// Parameters for Shi-Tomasi algorithm
    maxCorners = MAX(maxCorners, 1);
    vector<Point2f> corners;
    double qualityLevel = 0.01;
    double minDistance = 10;
    int blockSize = 5, gradientSize = 5;
    bool useHarrisDetector = true;
    //bool useHarrisDetector = false;
    double k = 0.04;

    /// Copy the source image
    Mat copy = src.clone();

    /// Apply corner detection
    goodFeaturesToTrack( src_gray,
                         corners,
                         maxCorners,
                         qualityLevel,
                         minDistance,
                         Mat(),
                         blockSize,
                         gradientSize,
                         useHarrisDetector,
                         k );


    /// Draw corners detected
    cout << "** Number of corners detected: " << corners.size() << endl;
    cout << corners[0] << endl;
    int radius = 20;
    for( size_t i = 0; i < corners.size(); i++ )
    {
        circle( copy, corners[i], radius, Scalar(0,255,0), FILLED );
        
    }

    imshow( source_window, copy );
}
