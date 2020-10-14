#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;
/*
Create a program, AddStructuredNoise.cpp, to generate and display a new 256x256 gray scale image that adds a sinusoidal image
to flowersgray.tiff. HINT: Use techniques you used Part A, problem 2A
*/

void PartB_2(){
  Mat m2(256, 256, CV_8UC3);
  for (int i = 0; i < m2.rows; i++){
      for (int j = 0; j < m2.cols; j++){

          double a = (i-m2.rows)/256.;
          double b = (j-m2.cols)/256.;

          m2.at<Vec3b>(i,j)[0] = ((1-b) * (1-a)*0) + ((1-b)*a*255) + (b*(1-a)*0) + (b*a*255);
          m2.at<Vec3b>(i,j)[1] = ((1-b) * (1-a)*0) + ((1-b)*a*0) + (b*(1-a)*255) + (b*a*255);
          m2.at<Vec3b>(i,j)[2] = ((1-b) * (1-a)*0) + ((1-b)*a*0) + (b*(1-a)*0) + (b*a*0);
      }
  }
  
  namedWindow("B-2");
  imshow("B-2", m2);
  waitKey(0);
}

int main(int argc, const char * argv[]) {
  PartB_2();
    return 0;
}
