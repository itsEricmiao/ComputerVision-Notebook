#include "opencv2/highgui/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;
void sineRect_breakoutroom();
int main( int argc, const char** argv ){
    sineRect_breakoutroom();
    return(0);
}


void sineRect_breakoutroom(){
    // Rectangular:  Full intensity inside a left and right boundary zero elsewhere
    // SineWave:     One full sine wave cycle
    Mat m1(256, 256, CV_32FC1, Scalar(0,0,0));
    Mat m2(256, 256, CV_32FC1, Scalar(1,0,0));
    for (int i = 0; i < 256; i++){
        for (int j = 0; j < 256; j++){
            m1.at<float>(i,j) = sin(j/255.* 4 * M_PI) * 0.5;
        }
    }
    for (int i = 0; i < 256; i++){
        for (int j = 0; j < 256; j++){
            if( j/256. < 0.3 || j/256. > 0.6){
                m2.at<float>(i,j) = 0;
            }else{
                m2.at<float>(i,j) = 255;
            }
        }
    }
    namedWindow("M1");
    imshow("M1", m1);
//    namedWindow("M2");
//    imshow("M2", m2);
    waitKey(0);
}
