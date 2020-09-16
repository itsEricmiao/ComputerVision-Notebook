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

void channelDisplay();

int main(int argc, const char * argv[]) {
    channelDisplay();
    return 0;
}

void channelDisplay(){
    Mat m1 = imread("apple.jpg");
    Mat m2 = Mat(m1.rows, m1.cols, m1.type());
    Mat m3 = Mat(m1.rows, m1.cols, m1.type());
    Mat m4 = Mat(m1.rows, m1.cols, m1.type());

    for (int i = 0; i < m1.rows; i++) {
        for (int j = 0; j < m1.cols; j++) {
            // Display only the Blue channel of the apple image
            cout<<m3.at<Vec3b>(i,j)<<endl;
            m3.at<Vec3b>(i,j)[0] = m1.at<Vec3b>(i,j)[1];
        }
    }

    for (int i = 0; i < m1.rows; i++) {
        for (int j = 0; j < m1.cols; j++) {
            // Display only the GREEN channel of the apple image
            m2.at<Vec3b>(i,j)[1] = m1.at<Vec3b>(i,j)[1];
        }
    }

    for (int i = 0; i < m1.rows; i++) {
        for (int j = 0; j < m1.cols; j++) {
            // Display only the Red channel of the apple image
            m4.at<Vec3b>(i,j)[2] = m1.at<Vec3b>(i,j)[1];
        }
    }

    namedWindow("M1");
    namedWindow("M2");
    namedWindow("M3");
    namedWindow("M4");
    moveWindow("M1",10,50);
    moveWindow("M2", 300,50);
    moveWindow("M3",10,600);
    moveWindow("M4", 300,600);
    imshow("M1", m1);
    imshow("M2", m2);
    imshow("M3", m3);
    imshow("M4", m4);
    waitKey(0);
}
