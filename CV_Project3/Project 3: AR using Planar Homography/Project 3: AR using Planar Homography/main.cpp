//
//  main.cpp
//  Project 3: AR using Planar Homography
//
//  Created by Eric Miao on 12/02/20.
//


#include <iostream>
#include <string>
#include <stdio.h>
#include <filesystem>
#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include "opencv2/calib3d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

void Interface();
void AR_Planar_Homography(string obj, string scene, string video);
void AR_Video(string video_path1, string video_path2, string image_path);
int main(int argc, const char * argv[]) {
    Interface();
    return 0;
}

void Interface(){
    cout<<"Welcome to Eric's OpenCV Project-3"<<endl;
    cout<<"Please select which image pair you would like to run: "<<endl;
    cout<<"1. Press 1 to run AR in Harry Potter Book"<<endl;
    cout<<"2. Press 2 to run AR in OpenCV Book"<<endl;
    cout<<"3. Press 3 to run AR in Box"<<endl;
    cout<<"4. Press 4 to run AR in Videos"<<endl;
    cout<<"Option: ";
    cin.clear();
    char option;
    cin>>option;
    if(option == '1'){
        AR_Planar_Homography("Project3_data/hp_cover.jpg", "Project3_data/hp_desk.png","Project3_data/ar_source.mov");
    }
    else if(option == '2'){
        AR_Planar_Homography("Project3_data/cv_cover.jpg", "Project3_data/cv_desk.png","Project3_data/ar_source.mov");
    }
    else if(option == '3'){
        AR_Planar_Homography("Project3_data/box.png", "Project3_data/box_in_scene.png","Project3_data/ar_source.mov");
    }
    else if(option == '4'){
        AR_Video("Project3_data/book.mov", "Project3_data/ar_source.mov", "Project3_data/cv_cover.jpg");
    }else{
        cout<<"Invalid Command. Please try again. "<<endl;
        Interface();
    }
}

void AR_Planar_Homography(string cover_name, string scene_name, string video){
    Mat img_object = imread(cover_name);
    Mat img_scene = imread(scene_name);
    
    
//-- Step 1: Detect the keypoints using SURF Detector, compute the descriptors
    int minHessian = 900;
    Ptr<SURF> detector = SURF::create( minHessian );
    std::vector<KeyPoint> keypoints_object, keypoints_scene;
    Mat descriptors_object, descriptors_scene;
    detector->detectAndCompute( img_object, noArray(), keypoints_object, descriptors_object );
    detector->detectAndCompute( img_scene, noArray(), keypoints_scene, descriptors_scene );
    
    //-- Step 2: Matching descriptor vectors with a FLANN based matcher
    // Since SURF is a floating-point descriptor NORM_L2 is used
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
    std::vector< std::vector<DMatch> > knn_matches;
    matcher->knnMatch( descriptors_object, descriptors_scene, knn_matches, 2 );
    //-- Filter matches using the Lowe's ratio test
    const float ratio_thresh = 0.75f;
    std::vector<DMatch> good_matches;
    for (size_t i = 0; i < knn_matches.size(); i++)
    {
        if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance)
        {
            good_matches.push_back(knn_matches[i][0]);
        }
    }
    //-- Draw matches
    Mat img_matches;
    drawMatches( img_object, keypoints_object, img_scene, keypoints_scene, good_matches, img_matches, Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
    //-- Localize the object
    std::vector<Point2f> obj;
    std::vector<Point2f> scene;
    for( size_t i = 0; i < good_matches.size(); i++ )
    {
        //-- Get the keypoints from the good matches
        obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
        scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
    }
    
    Mat H = findHomography( obj, scene, RANSAC );
    
    //-- Get the corners from the image_1 ( the object to be "detected" )
    std::vector<Point2f> obj_corners(4);
    obj_corners[0] = Point2f(0, 0);
    obj_corners[1] = Point2f( (float)img_object.cols, 0 );
    obj_corners[2] = Point2f( (float)img_object.cols, (float)img_object.rows );
    obj_corners[3] = Point2f( 0, (float)img_object.rows );
    std::vector<Point2f> scene_corners(4);
    
    
    Mat mask;
    Mat temp1(img_object.rows, img_object.cols, CV_8UC3, Scalar(255,255,255));
    warpPerspective(temp1, mask, H, img_scene.size());
    Mat src = img_scene.clone();
    Mat background = src - mask;

    
    imshow("Mask", mask);
    imshow("Source", src);
    imshow("Background", background);
    imshow("Matching", img_matches);
    string filename = video;
    VideoCapture capture(filename);
    Mat frame;

    if( !capture.isOpened()){
        throw "Error when reading media file";
    }
        
    for( ; ; )
    {
        capture >> frame;
        if(frame.empty()){
            break;
        }
        Mat f;
        resize(frame,frame,img_object.size());
        warpPerspective(frame, f, H, img_scene.size());
        imshow("video", f+background);
        waitKey(20); // waits to display frame
    }
    waitKey(0); // key press to close window
}

void AR_Video(string video_path1, string video_path2, string image_path){
    
    
    VideoCapture capture1(video_path1);
    VideoCapture capture2(video_path2);
    Mat img_object = imread(image_path);
    
    if( !capture1.isOpened() || !capture2.isOpened()){
        throw "Error when reading media file";
    }
    
    
    for( ; ; )
    {
        Mat img_scene;
        Mat frame;
        capture1 >> img_scene;
        capture2 >> frame;
        if(img_scene.empty()){
            break;
        }
        int minHessian = 1200;
        Ptr<SURF> detector = SURF::create( minHessian );
        std::vector<KeyPoint> keypoints_object, keypoints_scene;
        Mat descriptors_object, descriptors_scene;
        detector->detectAndCompute( img_object, noArray(), keypoints_object, descriptors_object );
        detector->detectAndCompute( img_scene, noArray(), keypoints_scene, descriptors_scene );
        
        //-- Step 2: Matching descriptor vectors with a FLANN based matcher
        // Since SURF is a floating-point descriptor NORM_L2 is used
        Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
        std::vector< std::vector<DMatch> > knn_matches;
        matcher->knnMatch( descriptors_object, descriptors_scene, knn_matches, 2 );
        //-- Filter matches using the Lowe's ratio test
        const float ratio_thresh = 0.75f;
        std::vector<DMatch> good_matches;
        for (size_t i = 0; i < knn_matches.size(); i++)
        {
            if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance)
            {
                good_matches.push_back(knn_matches[i][0]);
            }
        }
        //-- Draw matches
        Mat img_matches;
        drawMatches( img_object, keypoints_object, img_scene, keypoints_scene, good_matches, img_matches, Scalar::all(-1),Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
        //-- Localize the object
        std::vector<Point2f> obj;
        std::vector<Point2f> scene;
        for( size_t i = 0; i < good_matches.size(); i++ )
        {
            //-- Get the keypoints from the good matches
            obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
            scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
        }
        
        Mat H = findHomography( obj, scene, RANSAC );
        
        //-- Get the corners from the image_1 ( the object to be "detected" )
        std::vector<Point2f> obj_corners(4);
        obj_corners[0] = Point2f(0, 0);
        obj_corners[1] = Point2f( (float)img_object.cols, 0 );
        obj_corners[2] = Point2f( (float)img_object.cols, (float)img_object.rows );
        obj_corners[3] = Point2f( 0, (float)img_object.rows );
        std::vector<Point2f> scene_corners(4);
        
        
        Mat mask;
        Mat temp1(img_object.rows, img_object.cols, CV_8UC3, Scalar(255,255,255));
        warpPerspective(temp1, mask, H, img_scene.size());
        Mat src = img_scene.clone();
        Mat background = src - mask;

        imshow("Mask", mask);
        imshow("Source", src);
        imshow("Background", background);
        imshow("Matching", img_matches);
        imshow("background video", img_scene);

        if(frame.empty()){
            break;
        }
        Mat f;
        resize(frame,frame,img_object.size());
        warpPerspective(frame, f, H, img_scene.size());
        imshow("video", f + background);
        waitKey(20); // waits to display frame
    }

}
