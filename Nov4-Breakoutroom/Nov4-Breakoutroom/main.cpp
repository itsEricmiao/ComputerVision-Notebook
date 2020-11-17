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

int maxCorners = 3;
int maxTrackbar = 100;
string image1 = "triangle1.jpg";
string image2 = "triangle2.jpg";

RNG rng(12345);
const char* source_window = "Image";

/// Function header
void goodFeaturesToTrack_Demo( int, void* );

/**
 * @function main
 */
int main( int argc, char** argv )
{
    src = imread(image2);
    cvtColor( src, src_gray, COLOR_BGR2GRAY );
    // check if we succeeded
    if (src.empty()) {
        cerr << "ERROR! blank frame grabbed\n";
    }

    // show live and wait for a key with timeout long enough to show images
    //imshow("Live", frame);
    goodFeaturesToTrack_Demo( 0, 0 );
    waitKey(0);
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
    //cout << "** Number of corners detected: " << corners.size() << endl;
    
    cout << corners[0] << endl;
    cout << corners[1] << endl;
    cout << corners[2] << endl;
    Mat m[maxCorners];
    
    
    Mat newWindow(200,200, CV_8UC3, Scalar(255,255,255));
    
    for(int m = 0; m < corners.size(); m++){
        cout<<"--------------------5x5 of corner "<<m<<"-----------------------"<<endl;
        for(int i = -2; i < 3; i++){
            for(int j = -2; j < 3; j++){
                int xPos = corners[m].x+i;
                int yPos = corners[m].y+j;
                Vec3b pixelVal = src.at<Vec3b>(xPos, yPos);
                newWindow.at<Vec3b>(xPos, yPos) = pixelVal;
                cout<<pixelVal<<" ";
            }
            cout<<endl;
        }
    }
    resize(newWindow,newWindow, Size(newWindow.rows*2, newWindow.cols*2));
    
    
    int radius = 5;
    for( size_t i = 0; i < corners.size(); i++ )
    {
        
        circle( copy, corners[i], radius, Scalar(255,0,0), FILLED );
    }

    Mat dst (src.rows, src.cols, CV_8UC1, Scalar(0,0,0));
    
    
    imshow( source_window, copy );
    imshow( "des", newWindow );
}
