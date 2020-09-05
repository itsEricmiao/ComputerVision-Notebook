//
//  main.cpp
//  PA01_ExploringOpenCV
//
//  Created by Eric Miao on 9/5/20.
//  Copyright © 2020 Eric Miao. All rights reserved.
//


#include "opencv2/highgui/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;


int main(int argc, const char * argv[]) {
        Mat m1 = imread("apple.jpg");
        Mat m2 = Mat(m1.rows, m1.cols, m1.type());

        cout<<m1.type()<<endl;
        for (int i = 0; i < m1.rows; i++) {
            for (int j = 0; j < m1.cols; j++) {
                // Display only the GREEN channel of the apple image
                m2.at<Vec3b>(i,j)[1] = m1.at<Vec3b>(i,j)[1];
            }
        }


        namedWindow("M1");
        namedWindow("M2");
        moveWindow("M1",10,50);
        moveWindow("M2", 300,50);

        imshow("M1", m1);
        imshow("M2", m2);
    
        waitKey(0);
        return(0);
}
