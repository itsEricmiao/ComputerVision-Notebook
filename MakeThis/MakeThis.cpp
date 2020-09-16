#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;


void MakeThis(){

    Mat m1 = imread("flowergray.png");;
    Mat m2(256,256, CV_8UC3, Scalar(255,0,0));
    Mat m3(256,512, CV_8UC3, Scalar(0,255,0));

    namedWindow("M1");
    namedWindow("M2");
    namedWindow("M3");
    moveWindow("M1",10,50);
    moveWindow("M2", 276,50);
    moveWindow("M3",10,600);
    imshow("M1", m1);
    imshow("M2", m2);
    imshow("M3", m3);
    waitKey(0);
}


int main(int argc, const char * argv[]) {
    MakeThis();
}
