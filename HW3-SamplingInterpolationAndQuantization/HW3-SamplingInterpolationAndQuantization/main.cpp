//
//  main.cpp
//  HW3-SamplingInterpolationAndQuantization
//
//  Created by Eric Miao on 9/14/20.
//  Copyright © 2020 Eric Miao. All rights reserved.
//
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;

void PartA_1();
void PartA_2();
void PartB_1();
void PartB_2();
void PartB_3();
void PartB_4();
void PartB_4_2();
void on_trackbar_amplitude(int, void*);
void on_trackbar_frequency(int, void*);
void on_trackbar_phase(int, void*);
void on_trackbar_VFreq(int, void*);
void on_trackbar(int, void*);

int aVal = 0;
int fVal = 0;
int pVal = 0;
int vFreqVal = 0;

Mat img(256, 256, CV_32FC1, Scalar(0,0,0));

int main(int argc, const char * argv[]) {
    // insert code here...
    PartA_1();
//    PartB_4();
//    PartB_4_2();
//    PartB_3();
//    PartB_4_2();
    return 0;
}


/*
    Consider a sine function used to define the intensity of each pixel across a 512x512 floating point image, varying only
    with column. How would you define the intensity for a single element f(i,j) in a such a way that the sine way completes 64
    complete cycles across the full length of the image?
 */
void PartA_1(){
    Mat m1(512, 512, CV_32FC1, Scalar(0,0,0));
    for (int i = 0; i < m1.rows; i++){
        for (int j = 0; j < m1.cols; j++){
            m1.at<float>(i,j) = 0.5* sin(j/512.*64*M_PI)+0.5;
            
        }
    }
    cout<<0.5* sin(0/512.*64*M_PI)+0.5<<endl;
    cout<<0.5* sin(4/512.*64*M_PI)+0.5<<endl;
    cout<<0.5* sin(8/512.*64*M_PI)+0.5<<endl;
    cout<<0.5* sin(12/512.*64*M_PI)+0.5<<endl;
    cout<<0.5* sin(16/512.*64*M_PI)+0.5<<endl;
    namedWindow("M1");
    imshow("M1", m1);
    waitKey(0);
}



/*
    Considering the same 512 x 512 image, what is the maximum number of cycles that could be used to assure that the sine wave
    frequency can be reconstructed from the image?
*/
void PartA_2(){
    Mat m1(512, 512, CV_32FC1, Scalar(0,0,0));
    Mat m2(512, 512, CV_32FC1, Scalar(0,0,0));
    Mat m3(512, 512, CV_32FC1, Scalar(0,0,0));
    Mat m4(512, 512, CV_32FC1, Scalar(0,0,0));
    
    for (int i = 0; i < m1.rows; i++){
        for (int j = 0; j < m1.cols; j++){
            m1.at<float>(i,j) = 0.5*sin(i/255.* 4 * 2 *M_PI)+0.5;
        }
    }
    
    for (int i = 0; i < m2.rows; i++){
        for (int j = 0; j < m2.cols; j++){
            m2.at<float>(i,j) = 0.5*sin(sqrt(i*i + j*j)/255. * 3 * M_PI)+0.5;
        }
    }
    
    for (int i = 0; i < m3.rows; i++){
        for (int j = 0; j < m3.cols; j++){
            m3.at<float>(i,j) = 0.5*sin(j/255. * 2 * 2 *M_PI)+0.5;
        }
    }
    
    for (int i = 0; i < m4.rows; i++){
        for (int j = 0; j < m4.cols; j++){
            m4.at<float>(i,j) = 0.5*sin(sqrt(pow(i-512/2,2)+pow(j-512/2,2))/512 * M_PI*5)+.5;
        }
    }
    
    namedWindow("A");
    imshow("A", m1);

    
    namedWindow("B");
    imshow("B", m2);
    
    
    namedWindow("C");
    imshow("C", m3);
    
    namedWindow("D");
    imshow("D", m4);
    waitKey(0);
}



void PartB_1(){
    /*  Create a program, ColorRamp.cpp, to generate a two tone color ramp image of arbitrary dimensions, MxN, so that the amount of one color plan is linearly interpolated across the entire width. Use linear interpolation
    
        Let the color in the first column be C0 = (0,0,0) and let the color in the last column be CN-1=(255,0,0).
        Thatis,compute Ci =𝛼C0+(1–𝛼)CN-1
     */
    Mat m1(256, 256, CV_8UC3);
    for (int i = 0; i < m1.rows; i++){
        for (int j = 0; j < m1.cols; j++){
            double a = (j - m1.cols)/256.;
            m1.at<Vec3b>(i,j)[0] = ((1-a) * 0)  + (a*255);
            m1.at<Vec3b>(i,j)[1] = ((1-a) * 0)  + (a*255);
            m1.at<Vec3b>(i,j)[2] = ((1-a) * 0)  + (a*0);
        }
    }

    /*
        Create a TwoToneColorRamp.cpp so that the color varies smoothly horizontally and vertically in two colors. Use
        bilinear interpolation to vary across the plane.
     */
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
    
    namedWindow("B-1");
    imshow("B-1", m1);
    namedWindow("B-2");
    imshow("B-2", m2);
    waitKey(0);
}







/* Create a program, AddStructuredNoise.cpp, to generate and display a new 256x256 gray scale image that adds a sinusoidal image to flowersgray.tiff. HINT: Use techniques you used Part A, problem 2A */
void PartB_2(){
    Mat src;
    Mat img;
    src = imread( samples::findFile( "flowergray.png" ), IMREAD_COLOR );
    cvtColor(src,img, COLOR_BGR2GRAY);
    
    Mat m1(256, 256, CV_8UC1, Scalar(0,0,0));
    
    for (int i = 0; i < img.rows; i++){
        for (int j = 0; j < img.cols; j++){
            m1.at<uchar>(i,j) = (sin(i/256.*8*2*M_PI)*0.5+0.5) * 255 + img.at<uchar>(i,j);
        }
    }
    namedWindow("m1");
    imshow("m1", m1);
    namedWindow("img");
    imshow("img", src);
    waitKey(0);
}




/*
    Create a program, QuantizeThis.cpp, choose a MxN 8-bit color image, where 256 < M < 1024 and 256 < N < 1024, and create a second
    image in which each color plane is separately quantized to a 2, 3 or 4 bits representation. Save your image to a new png file using
    imwrite. Upload the source code, your input image and the png you created. Note – consider this to be the implementation of a tonal
    mapping function that maps an input (horizontal axis) to discrete outputs (vertical axis) as a step function.
*/
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



void on_trackbar_frequency( int, void* )
{
    for (int i = 0; i < img.rows; i++)
       for (int j=0; j < img.cols; j++)
           img.at<float>(i,j) = sin(j/256. * fVal * M_PI);
        imshow( "img", img );           // Update the window
}

void on_trackbar_phase( int, void* )
{
    for (int i = 0; i < img.rows; i++)
       for (int j=0; j < img.cols; j++)
           img.at<float>(i,j) = pVal * sin(j/256. * fVal * M_PI);
        imshow( "img", img );           // Update the window
}

void on_trackbar_amplitude( int, void* )
{
    for (int i = 0; i < img.rows; i++)
       for (int j=0; j < img.cols; j++)
           img.at<float>(i,j) = pVal * sin(j/256. * fVal * M_PI) + 0.05* aVal;
        imshow( "img", img );           // Update the window
}

void on_trackbar_VFreq( int, void* )
{
    for (int i = 0; i < img.rows; i++)
       for (int j=0; j < img.cols; j++)
        img.at<float>(i,j) = pVal * sin(j/256. * vFreqVal * fVal * M_PI) + 0.05* aVal;
        imshow( "img", img );           // Update the window
}


void on_trackBar(int, void*){
    for (int i = 0; i < img.rows; i++){
        for (int j=0; j < img.cols; j++){
            float h_pixel = aVal * 0.05 * sin(j*fVal*2*M_PI/(256+pVal)) + 0.5;
            float v_pixel = aVal * 0.05 * sin(i*vFreqVal*2*M_PI/(256+pVal)) + 0.5;
            img.at<float>(i,j) = h_pixel/2 + v_pixel/2;
        }
    }
    imshow("img", img);
    
}

void PartB_4(){
/*
    Develop an interactive tool SineWaveExplorer.cpp for constructing a single sinusoidal wave grating. Have one slider control the
    horizontal frequency, one control the phase shift and one control the amplitude.
*/
    namedWindow("img", WINDOW_AUTOSIZE);  // Size window to contents
    createTrackbar( "Frequency", "img", &fVal, 10, on_trackBar);
    createTrackbar( "Phase", "img", &pVal, 10, on_trackBar);
    createTrackbar( "Amplitude", "img", &aVal, 10, on_trackBar);
    imshow( "img", img );               // Initial Display
    waitKey(0);
    
}

void PartB_4_2(){
/*
    Develop an interactive tool SineWaveExplorer.cpp for constructing a single sinusoidal wave grating. Have one slider control the
    horizontal frequency, one control the phase shift and one control the amplitude.
*/
    namedWindow("img", WINDOW_AUTOSIZE);  // Size window to contents
    createTrackbar( "H_Frequency", "img", &fVal, 10, on_trackBar);
    createTrackbar( "Phase", "img", &pVal, 10, on_trackBar);
    createTrackbar( "Amplitude", "img", &aVal, 10, on_trackBar);
    createTrackbar( "V_Frequency", "img", &vFreqVal, 10, on_trackBar);
    imshow( "img", img );               // Initial Display
    waitKey(0);
    
}
