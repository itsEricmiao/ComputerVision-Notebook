#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;
/*
    Consider a sine function used to define the intensity of each pixel across a 512x512 floating point image, varying only
    with column. How would you define the intensity for a single element f(i,j) in a such a way that the sine way completes 64
    complete cycles across the full length of the image?
 */
void PartA_1(){
    Mat m1(512, 512, CV_32FC1, Scalar(0,0,0));
    for (int i = 0; i < m1.rows; i++){
        for (int j = 0; j < m1.cols; j++){
            m1.at<float>(i,j) = 0.5* sin(j/512.*64*M_PI)+0.5;
        }
    }
    namedWindow("M1");
    imshow("M1", m1);
    waitKey(0);
}



int main(int argc, const char * argv[]) {
    PartA_1();
    return 0;
}
