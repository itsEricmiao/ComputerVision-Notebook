//  main.cpp
//  PA01_ExploringOpenCV
//
//  Created by Eric Miao on 9/5/20.
//  Copyright © 2020 Eric Miao. All rights reserved.
//


#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;
void AddRandomNoise();

int main(int argc, const char * argv[]) {
    AddRandomNoise();
    return 0;
}

void AddRandomNoise(){
   Mat src;
   src= imread("flowergray.png",1);

   double val=100;

   Mat img = src.clone();
   Mat noise = Mat(src.size(),CV_8UC3);

   randn(noise, 0, val);
   img += noise;

   normalize(img,img,0, 255, 32, CV_8UC3);

   namedWindow("Output Window");
   namedWindow("Source Window");
   moveWindow("Output Window",10,50);
   moveWindow("Source Window", 300,50);
   imshow("Output Window",img);
   imshow("Source Window",src);

   waitKey(0);
}
