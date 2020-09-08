//
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

void displayGreenChannel(char* fileName);
void homework1A();
void channelDisplay();
void AddRandomNoise();
int helloImageDisplay();

int redval  = 0;
int blueval  = 0;
int greenval  = 0;
int blurval = 0;

Mat img;
Mat src1;
Mat src2;
Mat dst;




void on_trackbar_red( int, void* )
{
    // Modify image based on value set with trackbar
    for (int i = 0; i < img.rows; i++)
       for (int j=0; j < img.cols; j++)
        img.at<Vec3b>(i,j)[2] = redval;       // BGR
    cout<<"red: "<<redval<<endl;
    imshow( "W1", img );           // Update the window
}


void on_trackbar_blue( int, void* )
{
    // Modify image based on value set with trackbar
    for (int i = 0; i < img.rows; i++)
       for (int j=0; j < img.cols; j++)
        img.at<Vec3b>(i,j)[0] = blueval;       // BGR
    cout<<"blue: "<<blueval<<endl;
    imshow( "W1", img );           // Update the window
}

void on_trackbar_green( int, void* )
{
    // Modify image based on value set with trackbar
    for (int i = 0; i < img.rows; i++)
       for (int j=0; j < img.cols; j++)
        img.at<Vec3b>(i,j)[1] = greenval;       // BGR
    cout<<"blue: "<<blueval<<endl;
    imshow( "W1", img );           // Update the window
}

int calcRvalue(int i, int j, int index){
    int result;
    result = int((img.at<Vec3b>(i-1,j-1)[index]+img.at<Vec3b>(i-1,j)[index]+img.at<Vec3b>(i-1,j+1)[index]+img.at<Vec3b>(i,j-1)[index]+img.at<Vec3b>(i,j+1)[index]+img.at<Vec3b>(i+1,j-1)[index]+img.at<Vec3b>(i+1,j)[index] + img.at<Vec3b>(i+1,j+1)[index])/8);
    return result;
}

void on_trackbar_noise( int, void* )
{
    cout<<blurval<<endl;
    // Modify image based on value set with trackbar
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


Mat src = imread("apple.jpg");
int t1 = 0;
int t2 = 255;
void trackbar_2B( int, void* )
{
    cvtColor(src,img,COLOR_BGR2GRAY);
    for (int i = 0; i < img.rows; i++)
        for (int j=0; j < img.cols; j++)
        {
            if(img.at<uchar>(i,j) > t1){
                img.at<uchar>(i,j)= t2;
            }else{
                img.at<uchar>(i,j)= t1;
            }
        }
    imshow( "grayscale", img );
}

void thresholdSlider(){
//    Mat src = imread("apple.jpg");
    cvtColor(src,img,COLOR_BGR2GRAY);
    namedWindow("src", WINDOW_AUTOSIZE);  // Size window to contents
    namedWindow("grayscale", WINDOW_AUTOSIZE);  // Size window to contents
    createTrackbar( "t1", "grayscale", &t1, 255, trackbar_2B);
    createTrackbar( "t2", "grayscale", &t2, 255, trackbar_2B);
    moveWindow("src",10,50);
    moveWindow("grayscale", 300,50);
    imshow( "grayscale", img );               // Initial Display
    imshow( "src", src );
    waitKey(0);
}

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

void MakeThis(){
    
    Mat m1 = imread("flowergray.png");;
    Mat m2(256,256, CV_8UC3, Scalar(255,0,0));
    Mat m3(256,512, CV_8UC3, Scalar(0,255,0));
    
    namedWindow("M1");
    namedWindow("M2");
    namedWindow("M3");
    moveWindow("M1",10,50);
    moveWindow("M2", 276,50);
    moveWindow("M3",10,600);
    imshow("M1", m1);
    imshow("M2", m2);
    imshow("M3", m3);
    waitKey(0);
}

void imageInfomation(){
    Mat pngimg = imread("flowergray.png");
    std::cout << "Flowergray.png" << std::endl;
    std::cout << "rows: "<<pngimg.rows << std::endl;
    std::cout << "cols: "<< pngimg.cols << std::endl;
    std::cout << "channels: "<< pngimg.channels() << std::endl;
    std::cout << "type: "<< pngimg.type() << std::endl;
}

int main(int argc, const char * argv[]) {
//    thresholdSlider();
    channelDisplay();
    return(0);

}


void homework1A(){
// Q1: Create a 1024x1024 8-bit 4 channel image, with each element in the range 0..255 ?
//    Mat m1 = Mat(1024, 1024, CV_8UC4);
//    namedWindow("M1");
//    moveWindow("M1",10,50);
//    imshow("M1", m1);
//    waitKey(0);

// Q2: Create a 600 x 300 solid yellow image?
//    Mat m1 = Mat(600, 300, CV_8UC4, Scalar(0,255,255));
//    namedWindow("M1");
//    moveWindow("M1",10,50);
//    imshow("M1", m1);
//    waitKey(0);

// Q3: Create a 256x256 image filled with random 32-bit floats in the range [0,1)?
//    RNG rng;
//    float x = rng.uniform(0.0f ,1.0f);
//    float y = rng.uniform(0.0f ,1.0f);
//    float z = rng.uniform(0.0f ,1.0f);
//    std::cout<<x<<" "<<y<<" "<<z<<std::endl;
//    Mat m1 = Mat(256, 256, CV_8UC4);
//    for (int i = 0; i < m1.rows; i++) {
//        m1.at<cv::Vec3b>(i,m1.cols/2)[0] = x;
//        m1.at<cv::Vec3b>(i,m1.cols/2)[1] = y;
//        m1.at<cv::Vec3b>(i,m1.cols/2)[2] = z;  // NOTE:  DataType
//    }
//    namedWindow("M1");
//    moveWindow("M1",10,50);
//    imshow("M1", m1);
//    waitKey(0);
    
// Q4: Create a 9x9 identity matrix
//    Mat1d m1 (9,9);
//    namedWindow("M1");
//    moveWindow("M1",10,50);
//    imshow("M1", m1);
//    waitKey(0);

// Q5: Create a 20 x 50 matrix initialized to all ones.
//    Mat1d m1 (20, 50);
//    for (int i = 0; i < m1.rows; i++) {
//        for(int j = 0; j < m1.cols; j++){
//            m1.at<float>(i,j) = 1;
//        }
//    }
//    namedWindow("M1");
//    moveWindow("M1",10,50);
//    imshow("M1", m1);
//    waitKey(0);
    
//  Q6: Read a color image (call it “color.png”) convert to gray scale and display
//    Mat m1 = imread("apple.jpg");
//    Mat grayImage;
//    cvtColor(m1, grayImage, COLOR_BGR2GRAY);
//    namedWindow("GrayImage");
//    moveWindow("GrayImage",10,50);
//    imshow("GrayImage", grayImage);
//    waitKey(0);
    
//  Q7: Given a Mat object, m, that has type CV_32FC1 and has 256 rows and 256 columns, write a code segment to find the maximum value.
//    Mat m1 = imread("apple.jpg");
//    Mat m2(500, 500, CV_32FC1);
//    m1.convertTo(m2, CV_32FC1);
//    float max = 0.0;
//    for (int i = 0; i < m2.rows; i++){
//        for(int j = 0; j < m2.cols; j++){
//            if (m2.at<float>(i,j) > max){
//                max = m2.at<float>(i,j);
//            }
//        }
//    }
    
//  Q8: Given a Mat object, m, that has type CV_8UC3, write a code segment that would change the values of the 64th row to all white. (What is max intensity. Assume there are >= 64 rows in the image.) Apply this to a color image and UPLOAD a SCREEN SHOT.
    Mat m1 = imread("flowergray.png");
    Mat m2(400, 600, CV_8UC3);
    m1.convertTo(m2, CV_8UC3);
    
    for(int i = 0; i < m2.cols; i++){
        cv::Vec4b & pixel = m2.at<cv::Vec4b>(64, i);
        pixel[0] = 255;
        pixel[1] = 255;
        pixel[2] = 255;
    }
    namedWindow("m2");
    moveWindow("m2",10,50);
    imshow("m2", m2);
    waitKey(0);
}

void channelDisplay(){
    Mat m1 = imread("apple.jpg");
    Mat m2 = Mat(m1.rows, m1.cols, m1.type());
    Mat m3 = Mat(m1.rows, m1.cols, m1.type());
    Mat m4 = Mat(m1.rows, m1.cols, m1.type());
    
    for (int i = 0; i < m1.rows; i++) {
        for (int j = 0; j < m1.cols; j++) {
            // Display only the GREEN channel of the apple image
            m2.at<Vec3b>(i,j)[1] = m1.at<Vec3b>(i,j)[1];
        }
    }
    

    for (int i = 0; i < m1.rows; i++) {
        for (int j = 0; j < m1.cols; j++) {
            // Display only the GREEN channel of the apple image
            cout<<m3.at<Vec3b>(i,j)<<endl;
            m3.at<Vec3b>(i,j)[0] = m1.at<Vec3b>(i,j)[1];
        }
    }
    
    
    for (int i = 0; i < m1.rows; i++) {
        for (int j = 0; j < m1.cols; j++) {
            // Display only the GREEN channel of the apple image
            m4.at<Vec3b>(i,j)[2] = m1.at<Vec3b>(i,j)[1];
        }
    }


    namedWindow("M1");
    namedWindow("M2");
    namedWindow("M3");
    namedWindow("M4");
    moveWindow("M1",10,50);
    moveWindow("M2", 300,50);
    moveWindow("M3",10,600);
    moveWindow("M4", 300,600);
    imshow("M1", m1);
    imshow("M2", m2);
    imshow("M3", m3);
    imshow("M4", m4);
    waitKey(0);
}

void displayGreenChannel(char* fileName){
    Mat m1 = imread(fileName);
    Mat m2 = Mat(m1.rows, m1.cols, m1.type());
    cout<<m1.type()<<endl;
    for (int i = 0; i < m1.rows; i++) {
        for (int j = 0; j < m1.cols; j++) {
            // Display only the GREEN channel of the apple image
            m2.at<Vec3b>(i,j)[1] = m1.at<Vec3b>(i,j)[1];
        }
    }

    namedWindow("M1");
    namedWindow("M2");
    moveWindow("M1",10,50);
    moveWindow("M2", 300,50);
    imshow("M1", m1);
    imshow("M2", m2);
    waitKey(0);
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

int helloImageDisplay(){
    //create three Mat obects
    Mat jpgimg = imread("flowergray.png");
    Mat tiffimg = imread("flowergray.tiff");
    Mat pngimg = imread("flowergray.jpg");

    std::cout << "Flowergray.png" << std::endl;
    std::cout << pngimg.rows << std::endl;
    std::cout << pngimg.cols << std::endl;
    std::cout << pngimg.channels() << std::endl;
    std::cout << pngimg.type() << std::endl;
      
    std::cout << "Flowergray.tiff" << std::endl;
    std::cout << tiffimg.rows << std::endl;
    std::cout << tiffimg.cols << std::endl;
    std::cout << tiffimg.channels() << std::endl;
    std::cout << tiffimg.type() << std::endl;

    std::cout << "Flowergray.jpg" << std::endl;
    std::cout << jpgimg.rows << std::endl;
    std::cout << jpgimg.cols << std::endl;
    std::cout << jpgimg.channels() << std::endl;
    std::cout << jpgimg.type() << std::endl;

    //if the img is empty, die with error exit status
    if (jpgimg.empty() || pngimg.empty() || tiffimg.empty()){
       std::cout << "File read error somewhere.  Good luck finding it with this bad error handling code!" << std::endl;
       return -1;
    }

    // Create three windows
    namedWindow("TIFFGray", WINDOW_AUTOSIZE);
    namedWindow("PNGGray", WINDOW_NORMAL);
    namedWindow("JPGGray", WINDOW_AUTOSIZE);

    moveWindow("TIFFGray",50,200);

    // Show each image in a different window.
    imshow("TIFFGray", tiffimg);
    imshow("PNGGray", pngimg);
    imshow("JPGGray", jpgimg);

    // Destroy all and exit upon any key
    waitKey(0);
    destroyWindow("TIFFGray");
    destroyWindow("PNGGray");
    destroyWindow("JPGGray");
    return (0);
}

void constructor(){
    Mat m1 = Mat(500, 200, CV_32F);
    Mat m2 = Mat(200, 400, CV_8UC3, Scalar (255,255,0));
    Mat m3 = imread("flowergray.png");
    Mat m4 = Mat(m3);

    namedWindow("M1");
    namedWindow("M2");
    namedWindow("M4");

    imshow("M1", m1);
    imshow("M2", m2);
    imshow("M4", m4);

}


int setValues(){
    
    //create three Mat obects
    Mat m1 = Mat(100, 400, CV_32F);
    Mat m2 = Mat(200, 100, CV_8UC3, Scalar (255,255,0));
    Mat m3 = Mat (128, 256, CV_32FC3);
    Mat m4 = Mat (256, 256, CV_8U);

    // set value at each point as a function of row I(i,j) = i*(1/N)
    for (int i = 0; i < m1.rows; i++) {
        for (int j = 0; j < m1.cols; j++) {
            m1.at<float>(i,j) = j * 1./m1.cols; //
        }
    }

    // Set one column to red
    for (int i = 0; i < m2.rows; i++) {
        m2.at<cv::Vec3b>(i,m2.cols/2)[0] = 0;
        m2.at<cv::Vec3b>(i,m2.cols/2)[1] = 0;
        m2.at<cv::Vec3b>(i,m2.cols/2)[2] = 255;  // NOTE:  DataType
    }
         
    //  Set alternate rows to random noise values
    RNG r;
    for (int i = 0; i < m3.rows; i++) {
        for (int j = 0; j < m3.cols; j++) {
            m3.at<Vec3f>(i,j)[0] = 0.5f;
            m3.at<Vec3f>(i,j)[1] = 1.0f;
            m3.at<Vec3f>(i,j)[2] = (float)sin(j*M_PI/16);
        }
    }


    //  Set 20% of pixels to white, randomly
    for (int i = 0; i < m4.rows; i++) {
        for (int j = 0; j < m4.cols; j++) {
            if (r.uniform(0.0f, 1.0f) < 0.20)
               m4.at<uchar>(i,j) = 255;
        }
    }
    
    namedWindow("M1");
    namedWindow("M2");
    namedWindow("M3");
    namedWindow("M4");

    imshow("M1", m1);
    imshow("M2", m2);
    imshow("M3", m3);
    imshow("M4", m4);

    waitKey(0);
    destroyAllWindows();
    return(0);
}
