//  main.cpp
//  PA01_ExploringOpenCV
//
//  Created by Eric Miao on 9/5/20.
//  Copyright © 2020 Eric Miao. All rights reserved.
//


#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;


int t1 = 0;
int t2 = 255;
Mat src = imread("apple.jpg");

void trackbar_2B( int, void* )
{
    cvtColor(src,img,COLOR_BGR2GRAY);
    for (int i = 0; i < img.rows; i++)
        for (int j=0; j < img.cols; j++)
        {
            if(img.at<uchar>(i,j) > t1){
                img.at<uchar>(i,j)= t2;
            }else{
                img.at<uchar>(i,j)= t1;
            }
        }
    imshow( "grayscale", img );
}


void thresholdSlider(){
//    Mat src = imread("apple.jpg");
    cvtColor(src,img,COLOR_BGR2GRAY);
    namedWindow("src", WINDOW_AUTOSIZE);  // Size window to contents
    namedWindow("grayscale", WINDOW_AUTOSIZE);  // Size window to contents
    createTrackbar( "t1", "grayscale", &t1, 255, trackbar_2B);
    createTrackbar( "t2", "grayscale", &t2, 255, trackbar_2B);
    moveWindow("src",10,50);
    moveWindow("grayscale", 300,50);
    imshow( "grayscale", img );               // Initial Display
    imshow( "src", src );
    waitKey(0);
}


int main(int argc, const char * argv[]) {
    thresholdSlider();
    return(0);

}
