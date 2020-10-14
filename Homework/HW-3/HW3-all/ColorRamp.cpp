#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;

void PartB_1(){
    /*  Create a program, ColorRamp.cpp, to generate a two tone color ramp image of arbitrary dimensions, MxN, so that the amount of one color plan is linearly interpolated across the entire width. Use linear interpolation

        Let the color in the first column be C0 = (0,0,0) and let the color in the last column be CN-1=(255,0,0).
        Thatis,compute Ci =𝛼C0+(1–𝛼)CN-1
     */
    Mat m1(256, 256, CV_8UC3);
    for (int i = 0; i < m1.rows; i++){
        for (int j = 0; j < m1.cols; j++){
            double a = (j - m1.cols)/256.;
            m1.at<Vec3b>(i,j)[0] = ((1-a) * 0)  + (a*255);
            m1.at<Vec3b>(i,j)[1] = ((1-a) * 0)  + (a*255);
            m1.at<Vec3b>(i,j)[2] = ((1-a) * 0)  + (a*0);
        }
    }


    namedWindow("B-1");
    imshow("B-1", m1);

    waitKey(0);
}


int main(int argc, const char * argv[]) {
    PartB_1();
    return 0;
}
