//
//  main.cpp
//  midterm
//
//  Created by Eric Miao on 10/31/20.
//

#include <iostream>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void showMat1(){
    Mat m1(128, 128, CV_32FC1, Scalar(0,0,0));
    Mat m2(128, 128, CV_32FC1, Scalar(0,0,0));
    for(int i = 0; i < m1.rows; i++){
        for(int j = 0; j < m1.cols; j++){
            m1.at<float>(i,j) = 0.5*sin(j/255. * 4 * 2 *M_PI)+0.5;
        }
    }
    imshow("output", m1);
    waitKey(0);
}

void showMat2(){
    Mat m1(128, 128, CV_32FC1, Scalar(0,0,0));
    for(int i = 0; i < m1.rows; i++){
        for(int j = 0; j < m1.cols; j++){
            m1.at<float>(i,j) = sin(i/255. * 16 * 2 *M_PI)+0.5;
        }
    }
    imshow("output", m1);
    waitKey(0);
}

int main(int argc, const char * argv[]) {
    showMat2();
}

