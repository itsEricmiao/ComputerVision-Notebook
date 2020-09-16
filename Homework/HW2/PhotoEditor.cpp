//  main.cpp
//  MiniProject
//  Created by Eric Miao on 9/5/20.
//  Copyright © 2020 Eric Miao. All rights reserved.



//  Project Description: This is an exploratory project I did with openCV. I created 3 track bars for the GBR channels that allow user to display certain color only.

//To make it a bit more fun and creative, I added noise and blur track bars. The algorithm for blur effect is simply averaging the 8 surrounding values of a pixel. By doing that, we get an “average color” of a 3x3 pixels area. Thus, we can gradually reduce the clarity and the picture will eventually merges into one solid color. The algorithm for the noise effect is similar to what I did with the blur. The only difference is that we are now adding a blurval (corresponding to the movement of the trackbar) in each of the color channels.


#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;

void miniProject();

// trackBar functions
void on_trackbar_blue( int, void* );
void on_trackbar_green( int, void* );
void on_trackbar_red( int, void* );
void on_trackbar_noise( int, void* );
void on_trackbar_blur( int, void* );

// global variables
int redval  = 0;
int blueval  = 0;
int greenval  = 0;
int blurval = 0;

Mat img;


int main(int argc, const char * argv[]) {
    miniProject();
    return 0;
}

void miniProject(){
    img = imread("flowergray.png");
    namedWindow("W1", WINDOW_AUTOSIZE);  // Size window to contents
    createTrackbar( "Blue", "W1", &blueval, 255, on_trackbar_blue);
    createTrackbar( "Green", "W1", &greenval, 255, on_trackbar_green);
    createTrackbar( "Red", "W1", &redval, 255, on_trackbar_red);
    createTrackbar( "Blur", "W1", 0, 255, on_trackbar_blur);
    createTrackbar( "Noise", "W1", &blurval, 15, on_trackbar_noise);
    imshow( "W1", img );               // Initial Display
    waitKey(0);
}

void on_trackbar_red( int, void* )
{
    // Modify image based on value set with trackbar
    for (int i = 0; i < img.rows; i++)
       for (int j=0; j < img.cols; j++)
        img.at<Vec3b>(i,j)[2] = redval;       // BGR
    imshow( "W1", img );           // Update the window
}


void on_trackbar_blue( int, void* )
{
    // Modify image based on value set with trackbar
    for (int i = 0; i < img.rows; i++)
       for (int j=0; j < img.cols; j++)
        img.at<Vec3b>(i,j)[0] = blueval;       // BGR
    imshow( "W1", img );           // Update the window
}

void on_trackbar_green( int, void* )
{
    // Modify image based on value set with trackbar
    for (int i = 0; i < img.rows; i++)
       for (int j=0; j < img.cols; j++)
        img.at<Vec3b>(i,j)[1] = greenval;       // BGR
    imshow( "W1", img );           // Update the window
}

int calcRvalue(int i, int j, int index){
    int result;
    result = int((img.at<Vec3b>(i-1,j-1)[index]+img.at<Vec3b>(i-1,j)[index]+img.at<Vec3b>(i-1,j+1)[index]+img.at<Vec3b>(i,j-1)[index]+img.at<Vec3b>(i,j+1)[index]+img.at<Vec3b>(i+1,j-1)[index]+img.at<Vec3b>(i+1,j)[index] + img.at<Vec3b>(i+1,j+1)[index])/8);
    return result;
}

void on_trackbar_noise( int, void* )
{
    for (int i = 1; i < img.rows-1; i++)
        for (int j=1; j < img.cols-1; j++)
        {
            img.at<Vec3b>(i,j)[0] = calcRvalue(i, j, 0) + blurval;
            img.at<Vec3b>(i,j)[1] = calcRvalue(i, j, 1) + blurval;
            img.at<Vec3b>(i,j)[2] = calcRvalue(i, j, 2) + blurval;
        }
    imshow( "W1", img );
}

void on_trackbar_blur( int, void* )
{
    // Modify image based on value set with trackbar
    for (int i = 1; i < img.rows-1; i++)
        for (int j=1; j < img.cols-1; j++)
        {
            img.at<Vec3b>(i,j)[0] = calcRvalue(i, j, 0);
            img.at<Vec3b>(i,j)[1] = calcRvalue(i, j, 1);
            img.at<Vec3b>(i,j)[2] = calcRvalue(i, j, 2);
        }
    imshow( "W1", img );
}
