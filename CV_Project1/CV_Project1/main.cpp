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

// below are the variables for my sliders in ImageA
const int MAX = 100;
int sigma = 11;
Mat image1;
Mat image2;
Mat hybrid_image;
Mat low_freq_img;
Mat high_freq_img;

vector<Mat> read2Images();

void process();
void fftShift(Mat magI);
void interface();
void program1a();
void program1b();
void program2();
static void on_trackbar( int, void* );

Mat highPassFilter(Mat src, Mat filter);
Mat computeDFT(Mat image);
Mat process_B(Mat img);
Mat computeIDFT(const cv::Mat &complexImage);


int main()
{
    interface();
    return 0;
}


vector<Mat> read2Images(){
    vector<Mat> vec;
    for(int i = 0; i < 15; i++){
        cout<<i+1<<": "<<files[i]<<endl;
    }
    
    int option_img1;
    int option_img2;
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
    vec.push_back(image1);
    vec.push_back(image2);
    return vec;
    
}


void interface(){
    cout<<"Welcome to Eric's OpenCV Project-1"<<endl;
    cout<<"Please select which program you would like to run: "<<endl;
    cout<<"1. Type 1 to run Program A - FAST: using filter2D()"<<endl;
    cout<<"2. Type 2 to run Program A - SLOW: using my own high pass filter()"<<endl;
    cout<<"3. Type 3 to run Program B"<<endl;
    cout<<"Option: ";
    cin.clear();
    char option;
    cin>>option;
    if(option == '1'){
        program1a();
    }else if(option == '2'){
        program1b();
    }else if(option == '3'){
        program2();
    }
    else{
        cout<<"Invalid input! Please try again!\n"<<endl;
        interface();
    }
}


void program1a(){
    cout<<"\nWelcome to Program 1!"<<endl;
    vector<Mat> srcImages = read2Images();
    image1 = srcImages[0];
    image2 = srcImages[1];
    
    image1.convertTo(image1, CV_64FC3);
    image2.convertTo(image2, CV_64FC3);
    
    resize(image1, image1, Size(300, 300));
    resize(image2, image2, Size(300, 300));
    
    namedWindow("Hybrid image", WINDOW_AUTOSIZE);
    createTrackbar( "Blend", "Hybrid image", &sigma, MAX, on_trackbar );
    on_trackbar(sigma, 0 );
    waitKey(0);
}

static void on_trackbar( int, void* )
{
    process();
    imshow( "Hybrid image", hybrid_image );
}

void process(){
    Mat low_freq_img1;
    Mat low_freq_img2;
    Mat high_freq_img1;
    Mat high_freq_img2;
    
    int n = sigma * 2 + 1;
    Mat filter = getKernel(n, sigma, CV_64F);
    
    filter2D(image1, low_freq_img1, -1, filter, Point(-1, -1), 0, BORDER_CONSTANT);
    filter2D(image2, low_freq_img2, -1, filter, Point(-1, -1), 0, BORDER_CONSTANT);
    
    high_freq_img2 = image2 - low_freq_img2;
    hybrid_image = low_freq_img1 + high_freq_img2;
    
    high_freq_img2.convertTo(high_freq_img2, CV_8UC3);
    low_freq_img1.convertTo(low_freq_img1, CV_8UC3);
    hybrid_image.convertTo(hybrid_image, CV_8UC3);
    
    imshow("Low frequencies", low_freq_img1);
    imshow("High frequencies", high_freq_img2);
}


void program1b(){
    cout<<"\nWelcome to Program 1!"<<endl;
    
    vector<Mat> srcImages = read2Images();
    image1 = srcImages[0];
    image2 = srcImages[1];
    
    image1.convertTo(image1, CV_64FC3);
    image2.convertTo(image2, CV_64FC3);
    
    resize(image1, image1, Size(300, 300));
    resize(image2, image2, Size(300, 300));
    cout<<"Converting....This can take a while..."<<endl;
    int n = sigma * 3;
    Mat filter = getKernel(n, sigma, CV_64F);
    filter = filter*filter.t();
    
    low_freq_img = highPassFilter(image1, filter);
    high_freq_img = image2 - highPassFilter(image2, filter);
    hybrid_image = low_freq_img + high_freq_img;
    
    high_freq_img.convertTo(high_freq_img, CV_8UC3);
    low_freq_img.convertTo(low_freq_img, CV_8UC3);
    hybrid_image.convertTo(hybrid_image, CV_8UC3);
    
    imshow("Low frequencies", low_freq_img);
    imshow("High frequencies", high_freq_img);
    imshow("Hybrid image", hybrid_image );
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
    
    vector<Mat> srcImages = read2Images();
    image1 = srcImages[0];
    image2 = srcImages[1];

    resize(image1, image1, Size(300, 300));
    resize(image2, image2, Size(300, 300));
    
    cvtColor(image1, image1, COLOR_BGR2GRAY);
    cvtColor(image2, image2, COLOR_BGR2GRAY);
    
    Mat x = process_B(image1);
    Mat y = process_B(image2);
    Mat z = x + y;
    Mat z2 = computeIDFT(x);
    
    imshow("image1 mag", x);
    imshow("image2 mag", y);
    imshow("image3", z2);
    waitKey();
}

// Compute the Discrete fourier transform
Mat computeDFT(Mat image) {
    Mat padded;                            //expand input image to optimal size
    int m = getOptimalDFTSize( image.rows );
    int n = getOptimalDFTSize( image.cols ); // on the border add zero values
    copyMakeBorder(image, padded, 0, m - image.rows, 0, n - image.cols, BORDER_CONSTANT, Scalar::all(0));
    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};

    Mat complex;
    merge(planes, 2, complex);         // Add to the expanded another plane with zeros

    dft(complex, complex, DFT_COMPLEX_OUTPUT);  // fourier transform

    return complex;
}

// Compute the inverse of the Fourier Transform
Mat computeIDFT(const cv::Mat &complexImage) {
    //calculating the idft
    cv::Mat inverseTransform;
    cv::dft(complexImage, inverseTransform, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);
    normalize(inverseTransform, inverseTransform, 0, 1, NORM_MINMAX);
    return inverseTransform;
}

void fftShift(Mat magI) {

    // crop if it has an odd number of rows or columns
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    int cx = magI.cols/2;
    int cy = magI.rows/2;

    Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp;                            // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                     // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);
}



Mat process_B(Mat input){
    Mat img = input.clone();
    // The DFT takes a REAL image and returns a COMPLEX image
    int M = getOptimalDFTSize( img.rows );
    int N = getOptimalDFTSize( img.cols );
    Mat padded;
    copyMakeBorder(img, padded, 0, M - img.rows, 0, N - img.cols, BORDER_CONSTANT, Scalar::all(0));


    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexImg;
    
    
    
    merge(planes, 2, complexImg);
    
    dft(complexImg, complexImg);
    // complexImg:
    
    // 1. Real: mathmatical representation - magnitude
    // 2. Imaginary: phase range: [0,2pi]
    
    // compute log(1 + sqrt(Re(DFT(img))**2 + Im(DFT(img))**2))
    split(complexImg, planes);
    
    Mat ph;
    phase(planes[0], planes[1], ph);
    magnitude(planes[0], planes[1], planes[0]);
    
    Mat mag = planes[0];
    mag += Scalar::all(1);
    log(mag, mag);

//     crop the spectrum, if it has an odd number of rows or columns
    mag = mag(Rect(0, 0, mag.cols & -2, mag.rows & -2));
    int cx = mag.cols/2;
    int cy = mag.rows/2;

//     rearrange the quadrants of Fourier image
//     so that the origin is at the image center
    Mat tmp;
    Mat q0(mag, Rect(0, 0, cx, cy));
    Mat q1(mag, Rect(cx, 0, cx, cy));
    Mat q2(mag, Rect(0, cy, cx, cy));
    Mat q3(mag, Rect(cx, cy, cx, cy));

    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);

    normalize(mag, mag, 0, 1, NORM_MINMAX);

//    imshow("spectrum magnitude", mag);
//    imshow("phase", ph);
//    imshow("original image", padded);
    return ph;
}
