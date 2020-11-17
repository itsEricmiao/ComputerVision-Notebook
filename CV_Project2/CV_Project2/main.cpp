//  CV_Project2
//  Created by Eric Miao on 11/13/20.
//  I used fileSystem to list all files in the data folder. This might require using C++ 14.

#include <iostream>
#include <string>
#include <stdio.h>
#include <filesystem>
#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>

using namespace cv;
using namespace std;

const char * FilePath = "Project2_DataPairs"; // Name of the images data folder
vector<string> Files; // Stores all image names

Mat image1, img1_gray;
Mat image2, img2_gray;
int thresh = 200;

int GetDir_FileSystem(); // Using C++ 14 FileSystem to list all the files in the folder
void interface();
void HarrisCornerDetection();
void FastCornerDetection();
vector<Mat> read2Images();
int main(int argc, const char * argv[]) {
    GetDir_FileSystem(); // First getting all the images from the data folder
    interface(); // Interface function for this program
    return 0;
}

void interface(){
    cout<<"Welcome to Eric's OpenCV Project-2"<<endl;
    cout<<"Please select which program you would like to run: "<<endl;
    cout<<"1. Type 1 to run Feature Detection - Harris Corner Detection"<<endl;
    cout<<"2. Type 2 to run Feature Detection - FAST Corner Detection"<<endl;
    cout<<"2. Type 3 to run Feature Description"<<endl;
    cout<<"3. Type 4 to run Feature Matching"<<endl;
    cout<<"Option: ";
    cin.clear();
    char option;
    cin>>option;
    if(option == '1'){
        vector<Mat> images = read2Images();
        namedWindow("Harris Corner Detection");
        HarrisCornerDetection();
        waitKey(0);

    }else if(option == '2'){
        vector<Mat> images = read2Images();
        namedWindow("FAST Corner Detection");
        FastCornerDetection();
        waitKey(0);
    }else if(option == '3'){
       
    }
    else{
        cout<<"Invalid input! Please try again!\n"<<endl;
        interface();
    }
}

void FastCornerDetection(){
    Mat src = image1.clone();
    vector<KeyPoint> keypointsD;
    Ptr<FastFeatureDetector> detector = FastFeatureDetector::create();
    vector<Mat> descriptor;
    detector->detect(src, keypointsD, Mat());
    drawKeypoints(src, keypointsD, src);
    imshow("FAST Corner Detection", src);
}

vector<Mat> read2Images(){
    cout<<endl;
    vector<Mat> vec;
    for(int i = 0; i < Files.size(); i++){
        cout<<i+1<<": "<<Files[i]<<endl;
    }
    int option_img1;
    int option_img2;
    cout<<"Image 1 (For Features Detection): ";
    cin>>option_img1;
    cout<<"Image 2 (For Features Matching): ";
    cin>>option_img2;
    
    image1 = imread(Files[option_img1-1]);
    image2 = imread(Files[option_img2-1]);
    
    // Error checking and make sure read in correctly
    if(!image1.data || !image2.data){
        cout<<"Cannot find the image"<<endl;
        exit(0);
    }else{
        cout<<"Found "<<Files[option_img1-1]<<" and "<<Files[option_img2-1]<<"!"<<endl;
    }
    vec.push_back(image1);
    vec.push_back(image2);
    return vec;
}

void HarrisCornerDetection(){
    Mat src, gray;
    // Load source image and convert it to gray
    src = image1.clone();
    cvtColor( src, gray, COLOR_BGR2GRAY );
    Mat dst, dst_norm, dst_norm_scaled;
    dst = Mat::zeros( src.size(), CV_32FC1 );
    // Detecting corners
    cornerHarris( gray, dst, 7, 5, 0.05, BORDER_DEFAULT );
    // Normalizing
    normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
    convertScaleAbs( dst_norm, dst_norm_scaled );

    // Drawing a circle around corners
    for( int j = 0; j < dst_norm.rows ; j++ )
    {
        for( int i = 0; i < dst_norm.cols; i++ )
        {
            if( (int) dst_norm.at<float>(j,i) > thresh )
            {
                circle( dst_norm_scaled, Point( i, j ), 5,  Scalar(0), 2, 8, 0 );
            }
        }
    }
    // Showing the result
    imshow( "Harris Corner Detection", dst_norm_scaled );
}


int GetDir_FileSystem(){
    namespace fs = std::__fs::filesystem;
    for (auto & entry : fs::directory_iterator(FilePath)){
        Files.push_back(entry.path());
    }
    return 0;
}
