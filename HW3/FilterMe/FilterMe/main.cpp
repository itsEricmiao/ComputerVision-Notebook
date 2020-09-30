//
//  main.cpp
//  FilterMe
//
//  Created by Eric Miao on 9/22/20.
//

#include <iostream>
#include<opencv2/opencv.hpp>
using namespace cv;
using namespace std;

    
int main(int argc, const char * argv[]) {
    Mat image;
    VideoCapture cap(0);
    if(!cap.isOpened()){
        cout << "No camera detected on this system" << endl;
        return -1;
    }

    while(true){
        cap >> image;
        cv::resize(image, image, cv::Size(256, 256));

        if(image.empty()){
            cout << "Frame invalid and skipped!" << endl;
            continue;
        }

        //1280 x 720
        Mat im3(768, 768, CV_8UC3);
        
        Mat image1(im3, Rect(0, 0, 256, 256));
        Mat image2(im3, Rect(256, 0, 256, 256));
        Mat image3(im3, Rect(256*2, 0, 256, 256));
        
        Mat image4(im3, Rect(0, 256, 256, 256));
        Mat image5(im3, Rect(256, 256, 256, 256));
        Mat image6(im3, Rect(256*2, 256, 256, 256));
        
        Mat image7(im3, Rect(0, 256*2, 256, 256));
        Mat image8(im3, Rect(256, 256*2, 256, 256));
        Mat image9(im3, Rect(256*2, 256*2, 256, 256));
        
        
        image.copyTo(image1);
        image.copyTo(image2);
        image.copyTo(image3);
        image.copyTo(image4);
        image.copyTo(image5);
        image.copyTo(image6);
        image.copyTo(image7);
        image.copyTo(image8);
        image.copyTo(image9);

        // Image1: Box filter
        blur(image1, image1, cv::Size(3, 3));
        
        // Image2: 5x5 Gaussian
        GaussianBlur(image2, image2, cv::Size(5,5),0);
        
        
        // Image3&4: 3x3 Sobel operators
        Sobel(image3, image3, CV_8U, 1, 0, 3, 1, 0, BORDER_DEFAULT);
        Sobel(image4, image4, CV_8U, 0, 1, 3, 1, 0, BORDER_DEFAULT);
        
        
        // Image5: 3x3 Laplacian kernel
        Laplacian( image5, image5, CV_16S, 3, 1, 0, BORDER_DEFAULT );
        
        
        // Image6: linear combination
        addWeighted(image2, 1.3, image3, -0.3, 0.0, image6);

        
        // Image7: applies a linear combination to remove the mean to enhance edges
        GaussianBlur(image8, image7, cv::Size(5, 5), 3);
        addWeighted(image8, 1.5, image7, -0.5, 0, image7);
        
        
        // Image8: applies a linear edge enhancing filter to with a nonlinear threshold operation
        Mat gray, edge;
        Canny(image8, edge, 50, 150, 3);
        edge.convertTo(image8, CV_8U);
        
        
        cvtColor(im3, im3, COLOR_BGR2GRAY);
        imshow("FilterMe", im3);
        waitKey(5);
       }
    
    
    return 0;
}
