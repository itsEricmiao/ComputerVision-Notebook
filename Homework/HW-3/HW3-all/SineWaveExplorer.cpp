#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int aVal = 0;
int fVal = 0;
int pVal = 0;
int vFreqVal = 0;
Mat img(256, 256, CV_32FC1, Scalar(0,0,0));
void on_trackBar(int, void*){
    for (int i = 0; i < img.rows; i++){
        for (int j=0; j < img.cols; j++){
            float h_pixel = aVal * 0.05 * sin(j*fVal*2*M_PI/(256+pVal)) + 0.5;
            float v_pixel = aVal * 0.05 * sin(i*vFreqVal*2*M_PI/(256+pVal)) + 0.5;
            img.at<float>(i,j) = h_pixel/2 + v_pixel/2;
        }
    }
    imshow("img", img);

}

void PartB_4(){
/*
    Develop an interactive tool SineWaveExplorer.cpp for constructing a single sinusoidal wave grating. Have one slider control the
    horizontal frequency, one control the phase shift and one control the amplitude.
*/
    namedWindow("img", WINDOW_AUTOSIZE);  // Size window to contents
    createTrackbar( "H_Frequency", "img", &fVal, 10, on_trackBar);
    createTrackbar( "Phase", "img", &pVal, 10, on_trackBar);
    createTrackbar( "Amplitude", "img", &aVal, 10, on_trackBar);
    imshow( "img", img );               // Initial Display
    waitKey(0);

}

void PartB_4_2(){
/*
    Develop an interactive tool SineWaveExplorer.cpp for constructing a single sinusoidal wave grating. Have one slider control the
    horizontal frequency, one control the phase shift and one control the amplitude.
*/
    namedWindow("img", WINDOW_AUTOSIZE);  // Size window to contents
    createTrackbar( "H_Frequency", "img", &fVal, 10, on_trackBar);
    createTrackbar( "Phase", "img", &pVal, 10, on_trackBar);
    createTrackbar( "Amplitude", "img", &aVal, 10, on_trackBar);
    createTrackbar( "V_Frequency", "img", &vFreqVal, 10, on_trackBar);
    imshow( "img", img );               // Initial Display
    waitKey(0);

}

int main(int argc, const char * argv[]) {
    // PartB_4();
    PartB_4_2();
    return 0;
}
