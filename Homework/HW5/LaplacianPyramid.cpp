//
//  main.cpp
//  LaplacianPyramid
//
//  Created by Eric Miao on 10/26/20.
//

#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <stdio.h>
#include <iostream>
#include <iostream>

using namespace cv;
using namespace std;
void ShowManyImages(string title, int nArgs, ...);

int main(int argc, const char * argv[]) {
    // insert code here...
    Mat image = imread("mouse.jpg");
    cv::resize(image, image, Size(256, 256));
    vector<Mat> allImages;
    Mat prevMat = image.clone();
    for(int i = 0; i < 9; i++){
        Mat image_blurred;
        Mat img;
        cout<<prevMat.rows<<" "<<prevMat.cols<<endl;
        GaussianBlur(prevMat, image_blurred, Size(5, 5), 0);
        Mat newImg = prevMat - image_blurred;
        allImages.push_back(newImg);
        pyrDown(newImg, img, Size( prevMat.rows/2, prevMat.cols/2 ));
        prevMat = img.clone();
    }

    Mat window(512,512, CV_8UC3);
    allImages[0].copyTo(window(Rect(0, 0, allImages[0].cols, allImages[0].rows)));
    allImages[1].copyTo(window(Rect(256, 0, allImages[1].cols, allImages[1].rows)));
    allImages[2].copyTo(window(Rect(384, 0, allImages[2].cols, allImages[2].rows)));
    allImages[3].copyTo(window(Rect(448, 0, allImages[3].cols, allImages[3].rows)));
    allImages[4].copyTo(window(Rect(480, 0, allImages[4].cols, allImages[4].rows)));
    allImages[5].copyTo(window(Rect(496, 0, allImages[5].cols, allImages[5].rows)));
    allImages[6].copyTo(window(Rect(504, 0, allImages[6].cols, allImages[6].rows)));
    allImages[7].copyTo(window(Rect(508, 0, allImages[7].cols, allImages[7].rows)));
    allImages[8].copyTo(window(Rect(510, 0, allImages[8].cols, allImages[8].rows)));
//    allImages[9].copyTo(window(Rect(511, 0, allImages[9].cols, allImages[9].rows)));

    
    
    imshow( "Pyramid",window );
    waitKey(0);
    return 0;
}
