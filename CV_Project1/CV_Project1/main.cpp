#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <stdio.h>
#include <iostream>
#include "Kernel.h"

using namespace cv;
using namespace std;


string files[16]=
{"CatDog.png","Zebra.png","cat1.jpg","fish.bmp","mouse.jpg",
 "Chebra.png","bicycle.bmp","dog.jpg","marilyn.bmp","submarine.bmp",
 "Cheetah.png","cat.jpg","einstein.bmp","motorcycle.bmp","tiger.jpg"
};

// https://www.researchgate.net/figure/Discrete-approximation-of-the-Gaussian-kernels-3x3-5x5-7x7_fig2_325768087
Mat kernel3x3 = (Mat_<float>(3,3) <<1.0,2.0,1.0,
                                    2.0,4.0,2.0,
                                    1.0,2.0,1.0)/(float)(16);

Mat kernel5x5 = (Mat_<float>(5,5) <<1.0, 4.0, 7.0, 4.0, 1.0,
                                    4.0, 16.0, 26.0, 16.0, 4.0,
                                    7.0, 26.0, 41.0, 26.0, 7.0,
                                    4.0, 16.0, 26.0, 16.0, 4.0,
                                    1.0, 4.0, 7.0, 4.0, 1.0)/(float)(273);

Mat kernel7x7 = (Mat_<float>(7,7) <<0.0,0.0,1.0,2.0,1.0,0.0,0.0,
                                    0.0,3.0,13.0,22.0,13.0,3.0,0.0,
                                    1.0,13.0,59.0,97.0,59.0,13.0,1.0,
                                    2.0,22.0,97.0,159.0,97.0,22.0,1.0,
                                    1.0,13.0,59.0,97.0,59.0,13.0,1.0,
                                    0.0,3.0,13.0,22.0,13.0,3.0,0.0,
                                    0.0,0.0,1.0,2.0,1.0,0.0,0.0)/(float)(1003);


const int sigmaMax = 5;
int sigma = 2;
Mat image1;
Mat image2;
Mat hybrid_image;
Mat low_freq_img;
Mat high_freq_img;


Mat highPassFilter(Mat src, Mat filter);
static void on_trackbar( int, void* );
double get_val_with_replicate(Mat src, int x, int y, int c);
double get_val_with_reflect(Mat src, int x, int y, int c);
void processing(int n, int sigma, Mat filter);
void interface();
void program1();
void program2();
void findFiles();

// find the nearest point to x, y inside the image

int main()
{
    interface();
    return 0;

}

static void on_trackbar( int, void* )
{
    int n = sigma * 3;
    Mat filter = getKernel(n, sigma, CV_64F);
    filter = filter*filter.t();
    processing(n, sigma, filter);
    
}

void processing(int n, int sigma, Mat filter){
    low_freq_img = highPassFilter(image1, filter);
    high_freq_img = image2 - highPassFilter(image2, filter);
    hybrid_image = low_freq_img + high_freq_img;

    high_freq_img.convertTo(high_freq_img, CV_8UC3);
    low_freq_img.convertTo(low_freq_img, CV_8UC3);
    hybrid_image.convertTo(hybrid_image, CV_8UC3);
    
    imshow("Low frequencies", low_freq_img);
    imshow("High frequencies", high_freq_img);
    imshow("Hybrid image", hybrid_image );
}


void interface(){
    cout<<"Welcome to Eric's OpenCV Project-1"<<endl;
    cout<<"Please select which program you would like to run: "<<endl;
    cout<<"1. Type 1 to run Program A"<<endl;
    cout<<"2. Type 2 to run Program B"<<endl;
    cout<<"Option: ";
    cin.clear();
    char option;
    cin>>option;
    if(option == '1'){
        program1();
    }else if(option == '2'){
        program2();
    }else{
        cout<<"Invalid input! Please try again!\n"<<endl;
        interface();
    }
}

void program1(){
    cout<<"\nWelcome to Program 1!"<<endl;
    
    for(int i = 0; i < 15; i++){
        cout<<i+1<<": "<<files[i]<<endl;
    }
    
    int option_img1;
    int option_img2;
    cout<<"\nPlease select two images: "<<endl;
    cout<<"Image 1: ";
    cin>>option_img1;
    cout<<"Image 2: ";
    cin>>option_img2;
    
    image1 = imread("data/"+files[option_img1-1]);
    image2 = imread("data/"+files[option_img2-1]);
    image1.convertTo(image1, CV_64FC3);
    image2.convertTo(image2, CV_64FC3);
    
    // Error checking and make sure read in correctly
    if(!image1.data || !image2.data){
        cout<<"Cannot find the image"<<endl;
        exit(0);
    }else{
        cout<<"Found "<<files[option_img1-1]<<" and "<<files[option_img2-1]<<"!"<<endl;
    }
    
    
    namedWindow("Hybrid image", WINDOW_AUTOSIZE);
    createTrackbar("Blend", "Hybrid image", &sigma, sigmaMax, on_trackbar );
    waitKey(0);
}


Mat highPassFilter(Mat src, Mat filter)
{
    Mat dst = src.clone();
    double val = 0.0;

    for (int y= 0; y < dst.rows; y++){
        for (int x= 0; x < dst.cols; x++){
            Vec3f color = Vec3f(0.0, 0.0, 0.0);
            for (int channel = 0; channel < 3; channel ++){
                
                for (int j= 0; j < filter.rows; j++){
                    for (int i= 0; i < filter.cols; i++){
                        int posX = (j - filter.rows / 2) + x;
                        int poxY = (i - filter.cols / 2) + y;
                        if (posX < 0 || poxY < 0 || posX >= src.cols || poxY >= src.rows){
                            val = 0.0;
                        }else{
                            val = src.at<Vec3d>(poxY, posX)[channel]; // get the pixel from src image
                        }
                        color[channel] = color[channel] + val * filter.at<double>(j, i);
                    }
                }
                dst.at<Vec3d>(y, x)[channel] = color[channel]; // update output image with accumulated color
            }
        }
    }
    return dst;
}


void program2(){
    cout<<"\nWelcome to Program 2!"<<endl;
    for(int i = 0; i < 15; i++){
        cout<<i+1<<": "<<files[i]<<endl;
    }
    
    int option_img1;
    int option_img2;
    cout<<"\nPlease select two images: "<<endl;
    cout<<"Image 1: ";
    cin>>option_img1;
    cout<<"Image 2: ";
    cin>>option_img2;
    
    image1 = imread("data/"+files[option_img1-1]);
    image2 = imread("data/"+files[option_img2-1]);
    // Error checking and make sure read in correctly
    if(!image1.data || !image2.data){
        cout<<"Cannot find the image"<<endl;
        exit(0);
    }else{
        cout<<"Found "<<files[option_img1-1]<<" and "<<files[option_img2-1]<<"!"<<endl;
    }
    
}
