//
//  main.cpp
//  BlendHW3
//
//  Created by Eric Miao on 9/24/20.
//
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;


const int maxSlider = 100;
int alpha_slider = 0;
Mat src1;
Mat src2;
Mat dst;


static void on_trackbar( int, void* )
{
    double alpha = float(alpha_slider)/maxSlider;
    cout<<alpha_slider<<" "<<maxSlider<<endl;
    cout<<alpha<<endl;
    for(int i = 0; i < dst.rows; i++){
        for(int j = 0; j < dst.cols; j++){
            for(int channel = 0; channel < 3; channel ++){
                dst.at<Vec3b>(i,j)[channel] = int((1.0-alpha) * src1.at<Vec3b>(i,j)[channel] + alpha * src2.at<Vec3b>(i,j)[channel]);
            }
        }
    }
    imshow("Linear interpolation", dst);
}


int main( int argc, const char** argv )
{
    src1 = imread( "img1.jpg");
    src2 = imread( "img2.jpg");
    

    resize(src1, src1, Size(256, 256),0, 0, INTER_NEAREST);
    resize(src2, src2, Size(256, 256),0, 0, INTER_NEAREST);
    dst = Mat(256, 256, src1.type(), Scalar(0));

    alpha_slider = 0;
    namedWindow("Linear interpolation", WINDOW_AUTOSIZE); // Create Window
    createTrackbar("Blended Level","Linear interpolation", &alpha_slider, 100, on_trackbar);
    imshow("Linear interpolation", dst);
    waitKey(0);
    
    return 0;
    
}
