#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;

void invertChannel(){
    Mat m3 = imread("flowergray.png");
    Mat m4 = Mat(m3.rows, m3.cols, m3.type());
    cout<<m3.type()<<endl;
    cout<<m4.type()<<endl;
    // MISSION: Do not alter the original flower image
    for (int i = 0; i < m4.rows; i++) {
        for (int j = 0; j < m4.cols*3; j++) {
            m4.at<uchar>(i,j) = 255 - m3.at<uchar>(i,j);
        }
    }

    namedWindow("M3");
    namedWindow("M4");
    imshow("M3", m3);
    imshow("M4", m4);
    waitKey(0);
}

int main(int argc, const char * argv[]) {
    invertChannel();
    }
