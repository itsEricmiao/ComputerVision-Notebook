#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;

void PartA_2(){
    Mat m1(512, 512, CV_32FC1, Scalar(0,0,0));
    Mat m2(512, 512, CV_32FC1, Scalar(0,0,0));
    Mat m3(512, 512, CV_32FC1, Scalar(0,0,0));
    Mat m4(512, 512, CV_32FC1, Scalar(0,0,0));

    for (int i = 0; i < m1.rows; i++){
        for (int j = 0; j < m1.cols; j++){
            m1.at<float>(i,j) = 0.5*sin(i/255.* 4 * 2 *M_PI)+0.5;
        }
    }

    for (int i = 0; i < m2.rows; i++){
        for (int j = 0; j < m2.cols; j++){
            m2.at<float>(i,j) = 0.5*sin(sqrt(i*i + j*j)/255. * 3 * M_PI)+0.5;
        }
    }

    for (int i = 0; i < m3.rows; i++){
        for (int j = 0; j < m3.cols; j++){
            m3.at<float>(i,j) = 0.5*sin(j/255. * 2 * 2 *M_PI)+0.5;
        }
    }

    for (int i = 0; i < m4.rows; i++){
        for (int j = 0; j < m4.cols; j++){
            m4.at<float>(i,j) = 0.5*sin(sqrt(pow(i-512/2,2)+pow(j-512/2,2))/512 * M_PI*5)+.5;
        }
    }

    namedWindow("A");
    imshow("A", m1);


    namedWindow("B");
    imshow("B", m2);


    namedWindow("C");
    imshow("C", m3);

    namedWindow("D");
    imshow("D", m4);
    waitKey(0);
}


int main(int argc, const char * argv[]) {
    PartA_2();
    return 0;
}
