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
void read2Images();
void read1Image();
void MyLine( Mat img, Point start, Point end );
void FastCornerDetection(Mat image);
vector<pair<int,int>> HarrisCornerDetection(Mat image);
vector<vector<float>> Feature_Description(vector<pair<int,int>> Pos, Mat image);
vector<int> FeatureMatching(vector<vector<float>>v1, vector<vector<float>>v2);



int main(int argc, const char * argv[]) {
    GetDir_FileSystem(); // First getting all the images from the data folder
    interface(); // Interface function for this program
    return 0;
}

void interface(){
    cout<<"Welcome to Eric's OpenCV Project-2"<<endl;
    cout<<"Please select which program you would like to run: "<<endl;
    cout<<"1. Press 1 to run Feature Detection - Harris Corner Detection"<<endl;
    cout<<"2. Press 2 to run Feature Detection - FAST Corner Detection"<<endl;
    cout<<"3. Press 3 to run Feature Description and Feature Matching"<<endl;
    cout<<"Option: ";
    cin.clear();
    char option;
    cin>>option;
    if(option == '1'){
        read1Image();
        namedWindow("Harris Corner Detection");
        HarrisCornerDetection(image1);
        waitKey(0);

    }else if(option == '2'){
        read1Image();
        namedWindow("FAST Corner Detection");
        FastCornerDetection(image1);
        waitKey(0);
    }else if(option == '3'){
        read2Images();
        vector<pair<int,int>>p1 = HarrisCornerDetection(image1);
        vector<pair<int,int>>p2 = HarrisCornerDetection(image2);
        vector<vector<float>>n1 = Feature_Description(p1, image1);
        vector<vector<float>>n2 = Feature_Description(p2, image2);
        vector<int> match = FeatureMatching(n1, n2);
        
        int rows = max(image1.rows, image2.rows);
        int cols = image1.cols + image2.cols;
        Mat3b res(rows, cols, Vec3b(0,0,0));
        image1.copyTo(res(Rect(0, 0, image1.cols, image1.rows)));
        image2.copyTo(res(Rect(image1.cols, 0, image2.cols, image2.rows)));
        
        // drawing corners on each images
        for(int i = 0; i < p1.size(); i++){
            circle( res, Point(p1[i].first,p1[i].second), 5,  Scalar(0,255,0), 2, 8, 0 );
        }
        for(int i = 0; i < p2.size(); i++){
            circle( res, Point(p2[i].first + image1.cols,p2[i].second), 5,  Scalar(0,0,255), 2, 8, 0 );
        }
        
        for(int i = 0; i < match.size(); i++){
            cout<<i<<" - "<<match[i]<<endl;
        }
        
        //drawing connection lines
        for(int i = 0; i < match.size(); i++){
            int desIndex = match[i];
            if(match[i] != -1){ //if has connection
                int originX = p1[i].first;
                int originY = p1[i].second;
                int desX = p2[desIndex].first + image1.cols;
                int desY = p2[desIndex].second;
                MyLine(res,Point(originX, originY), Point(desX, desY));
            }
        }
        imshow("Result", res);
        waitKey();
        
    }else{
        cout<<"Invalid input! Please try again!\n"<<endl;
        interface();
    }
}

void MyLine( Mat img, Point start, Point end )
{
  int thickness = 2;
  int lineType = LINE_8;
  line( img,
    start,
    end,
    Scalar( 255, 255, 0 ),
    thickness,
    lineType );
}

void FastCornerDetection(Mat image){
    Mat src = image.clone();
    vector<KeyPoint> keypointsD;
    Ptr<FastFeatureDetector> detector = FastFeatureDetector::create();
    vector<Mat> descriptor;
    detector->detect(src, keypointsD, Mat());
    drawKeypoints(src, keypointsD, src);
    imshow("FAST Corner Detection", src);
}

vector<pair<int,int>> HarrisCornerDetection(Mat image){
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
    
    imshow( "Harris Corner Detection", src );
    imshow( "After non-maximal suppression", src2 );
    
    return NewPos;
    
    // Showing the result
    
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

void read1Image(){
    cout<<endl;
    for(int i = 0; i < Files.size(); i++){
        cout<<i+1<<": "<<Files[i]<<endl;
    }
    int option_img1;
    cout<<"Image: ";
    cin>>option_img1;
    image1 = imread(Files[option_img1-1]);
    // Error checking and make sure read in correctly
    if(!image1.data){
        cout<<"Cannot find the image"<<endl;
        exit(0);
    }else{
        cout<<"Found "<<Files[option_img1-1]<<"!"<<endl;
    }
}

vector<vector<float>> Feature_Description(vector<pair<int,int>> Pos, Mat image){
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
                    pixelVal = 0;
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
    return neighbors;
}


vector<int> FeatureMatching(vector<vector<float>>v1, vector<vector<float>>v2){
    
    vector<int> match;
    
    for(int i = 0; i < v1.size(); i++){
        float min = 1000;
        int minIndex = -1;
        for(int j = 0; j < v2.size(); j++){
            float ssd = norm(v1[i], v2[j], NORM_L2);
            if(ssd < min){
                if(find(match.begin(), match.end(), j) == match.end()){
                    min = ssd;
                    minIndex = j;
                }
            }
        }
        if(min < 1000){
            match.push_back(minIndex);
        }else{
            match.push_back(-1);
        }
    }
    return match;
}


int GetDir_FileSystem(){
    namespace fs = std::__fs::filesystem;
    for (auto & entry : fs::directory_iterator(FilePath)){
        Files.push_back(entry.path());
    }
    return 0;
}
