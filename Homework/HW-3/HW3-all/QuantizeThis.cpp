#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;

void PartB_3(){
    Mat m1;
    Mat m2;
    m1 = imread("temp.png");
    m2 = m1.clone();

    int table[4];
    table[0] = 0;
    table[1] = 64;
    table[2] = 128;
    table[3] = 192;

    for (int i = 0; i < img.rows; i++){
        for (int j=0; j < img.cols*3; j++){
            int a = m1.at<Vec3b>(i,j)[0]/64;
            int b = m1.at<Vec3b>(i,j)[1]/64;
            int c = m1.at<Vec3b>(i,j)[2]/64;

            m2.at<Vec3b>(i,j)[0] = table[a];
            m2.at<Vec3b>(i,j)[1] = table[b];
            m2.at<Vec3b>(i,j)[2] = table[c];
        }
    }

    namedWindow("m1");
    imshow("m1", m1);
    namedWindow("m2");
    imshow("m2", m2);
    waitKey(0);
}

int main(int argc, const char * argv[]) {
    PartB_3();
    return 0;
}
