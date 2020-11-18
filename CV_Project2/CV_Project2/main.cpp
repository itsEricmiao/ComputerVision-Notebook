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

Mat image1;
Mat image2;
int thresh = 100;



int GetDir_FileSystem(); // Using C++ 14 FileSystem to list all the files in the folder
void interface();
void HarrisCornerDetection(Mat image);
void FastCornerDetection(Mat image);
void Feature_Description(vector<pair<int,int>> Pos, Mat image);
void FeatureMatching(vector<vector<float>>v1, vector<vector<float>>v2);
void read2Images();


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
    cout<<"Option: ";
    cin.clear();
    char option;
    cin>>option;
    if(option == '1'){
        read2Images();
        namedWindow("Harris Corner Detection");
        HarrisCornerDetection(image1);
//        HarrisCornerDetection(image2);
        waitKey(0);

    }else if(option == '2'){
        read2Images();
        namedWindow("FAST Corner Detection");
        FastCornerDetection(image1);
        FastCornerDetection(image2);
        waitKey(0);
    }else{
        cout<<"Invalid input! Please try again!\n"<<endl;
        interface();
    }
}

void FastCornerDetection(Mat image){
    Mat src = image1.clone();
    vector<KeyPoint> keypointsD;
    Ptr<FastFeatureDetector> detector = FastFeatureDetector::create();
    vector<Mat> descriptor;
    detector->detect(src, keypointsD, Mat());
    drawKeypoints(src, keypointsD, src);
    imshow("FAST Corner Detection", src);
}

void HarrisCornerDetection(Mat image){
    
    Mat src, src2, gray;
    
    // Detector parameters
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;
    int fpcnt=0;
    
    // Load source image and convert it to gray
    src = image.clone();
    src2 = image.clone();
    cvtColor( src, gray, COLOR_BGR2GRAY );
    Mat dst, dst_norm, dst_norm_scaled;
    dst = Mat::zeros( src.size(), CV_32FC1 );
    
    // Detecting corners
    cornerHarris( gray, dst, blockSize, apertureSize, k );
    
    // Compute Eigenvector
    Mat dst_eig = Mat::zeros( src.size(), CV_32FC1 );
    Mat dst_eigvecs = Mat::zeros( src.size(), CV_32FC(6) );
    cornerMinEigenVal( gray, dst_eig, blockSize, apertureSize );
    cornerEigenValsAndVecs( gray, dst_eigvecs, blockSize, apertureSize );

    
    // Normalizing
    normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
    convertScaleAbs( dst_norm, dst_norm_scaled );
    vector<pair<int,int>> AllPos;
    vector<vector<float>> Pixels;

    cout << " =========> APPLY THRESHOLD TO CORNERNESS MAP c(H)   " << thresh << endl;
    for( int i = 0; i < dst_norm.rows ; i++ ){
        for( int j = 0; j < dst_norm.cols; j++ ){
            if( (int) dst_norm.at<float>(i,j) > thresh ){
                // Drawing a circle around corners
                circle( src, Point(j,i), 5,  Scalar(255), 2, 8, 0 );
                vector<float> p;
                pair<int, int> pos;
                pos.first = j;
                pos.second = i;
                AllPos.push_back(pos);
                cout<<"--------------------7x7 corner "<<i<<" ("<<j<<","<<i<<")"<<"-----------------------"<<endl;
                for(int m = -3; m < 4; m++){
                    for(int n = -3; n < 4; n++){
                        int xPos = j+m;
                        int yPos = i+n;
                        float pixelVal = 0;
                        if(xPos > src.rows || xPos < 0 || yPos > src.cols || yPos < 0){
        
                        }else{
                            pixelVal = gray.at<float>(xPos, yPos);
                        }
                        p.push_back(pixelVal);
                        cout<<pixelVal<<" ";
                    }
                    cout<<endl;
                }
                
                Pixels.push_back(p);
                cout << "FEATURE POINT: " << ++fpcnt << endl;
                cout << "POSITION: ("<<j << ", " << i <<")"<< endl;
                cout << "c(H): " << dst.at<float>(i,j) << endl;
                cout << "Normalized c(H): " << dst_norm.at<float>(i,j) << endl;
                cout << "MinEigenValues: " << dst_eig.at<float>(i,j) << endl;
                cout << "Eigen Vectors: " << dst_eigvecs.at<Vec6f>(i,j) << endl;
                cout << endl;
            }
        }
    }
    
    // Using the non-maximal suppression
    vector<int> temp;
    for(int i = 0; i < AllPos.size(); i++){
        for(int j = i+1; j < AllPos.size(); j++){
            int x1 = AllPos[i].first;
            int y1 = AllPos[i].second;
            int x2 = AllPos[j].first;
            int y2 = AllPos[j].second;
            vector<float> descriptor_1 = Pixels[i];
            vector<float> descriptor_2 = Pixels[j];
            if(abs(x1-x2) <=7 && abs(y1-y2)<=7){
                float ch1 = dst_norm.at<float>(x1,y1);
                float ch2 = dst_norm.at<float>(x2,y2);
                if(ch1 > ch2){
                    temp.push_back(j);
                }else{
                    temp.push_back(i);
                }
            }
        }
    }
    
    // Getting rid of the duplicates and sort the vector
    vector<pair<int,int>> NewPos;
    sort( temp.begin(), temp.end() );
    temp.erase( unique( temp.begin(), temp.end() ), temp.end() );
    for(int i = 0; i < AllPos.size(); i++){
        if(find(temp.begin(), temp.end(), i) != temp.end()) {
            /* v contains x */
            continue;
        } else {
            /* v does not contain x */
            NewPos.push_back(AllPos[i]);
        }
    }
    
    cout<<"Old Corners: "<<AllPos.size()<<endl;
    cout<<"New Corners: "<<NewPos.size()<<endl;
    cout<<"Removed: "<<temp.size()<<endl;
    
    for(int i = 0; i < NewPos.size(); i++){
        circle( src2, Point(NewPos[i].first,NewPos[i].second), 5,  Scalar(255), 2, 8, 0 );
    }
    Feature_Description(NewPos, image);
    // Showing the result
    imshow( "Harris Corner Detection", src );
    imshow( "After non-maximal suppression", src2 );
}



void read2Images(){
    cout<<endl;
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
}


void Feature_Description(vector<pair<int,int>> Pos, Mat image){
    Mat src, gray;
    src = image.clone();
    cvtColor( src, gray, COLOR_BGR2GRAY );
    vector<vector<float>> neighbors;
    for(int i = 0; i < Pos.size(); i++){
        vector<float> p;
        int x = Pos[i].first;
        int y = Pos[i].second;
        for(int m = -2; m < 3; m++){
            for(int n = -2; n < 3; n++){
                int xPos = x+m;
                int yPos = y+n;
                float pixelVal = 0;
                if(xPos > src.rows || xPos < 0 || yPos > src.cols || yPos < 0){

                }else{
                    pixelVal = gray.at<float>(xPos, yPos);
                }
                p.push_back(pixelVal);
                cout<<pixelVal<<" ";
            }
            cout<<endl;
        }
        neighbors.push_back(p);
    }
    
    
    for(int i = 0; i < neighbors.size(); i++){
        cout<<"--------------------5x5 neighbor ("<<Pos[i].first<<","<<Pos[i].second<<")-----------------------"<<endl;
        for(int j = 0; j < neighbors[i].size(); j++){
            if(j >0 && j %5 == 0){
                cout<<endl;
            }
            cout<<neighbors[i][j]<<" ";
        }
        cout<<endl;
    }
}


void FeatureMatching(vector<vector<float>>v1, vector<vector<float>>v2){
    
}


int GetDir_FileSystem(){
    namespace fs = std::__fs::filesystem;
    for (auto & entry : fs::directory_iterator(FilePath)){
        Files.push_back(entry.path());
    }
    return 0;
}
