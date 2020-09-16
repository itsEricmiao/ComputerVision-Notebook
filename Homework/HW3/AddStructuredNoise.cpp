#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;

/* Create a program, AddStructuredNoise.cpp, to generate and display a new 256x256 gray scale image that adds a sinusoidal image to flowersgray.tiff. HINT: Use techniques you used Part A, problem 2A */
void PartB_2(){
    Mat src;
    Mat img;
    src = imread( samples::findFile( "flowergray.png" ), IMREAD_COLOR );
    cvtColor(src,img, COLOR_BGR2GRAY);

    Mat m1(256, 256, CV_8UC1, Scalar(0,0,0));

    for (int i = 0; i < img.rows; i++){
        for (int j = 0; j < img.cols; j++){
            m1.at<uchar>(i,j) = (sin(i/256.*8*2*M_PI)*0.5+0.5) * 255 + img.at<uchar>(i,j);
        }
    }
    namedWindow("m1");
    imshow("m1", m1);
    namedWindow("img");
    imshow("img", src);
    waitKey(0);
}


int main(int argc, const char * argv[]) {
  PartB_2();
    return 0;
}
