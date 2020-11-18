/**
 * @function cornerHarris_Demo.cpp
 * @brief Demo code for detecting corners using Harris-Stephens method
 * @author OpenCV team
 */

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

/// Global variables
Mat src, src_gray;
int thresh = 200;
int max_thresh = 255;

const char* source_window = "Source image";
const char* corners_window = "Corners detected";

/// Function header
void cornerHarris_demo( int, void* );

/**
 * @function main
 */
int main( int argc, char** argv )
{
    /// Load source image and convert it to gray
    //CommandLineParser parser( argc, argv, "{@input | building.jpg | input image}" );
    //src = imread( samples::findFile( parser.get<String>( "@input" ) ) );
    src = imread( "purplesquare.png");
//    src = imread( "triangle1.jpg");
//    src = imread( "triangle2.jpg");
    if ( src.empty() )
    {
        cout << "Could not open or find the image!\n" << endl;
        return -1;
    }

    /// Create a window and a trackbar
    namedWindow( source_window );
    createTrackbar( "Threshold: ", source_window, &thresh, max_thresh, cornerHarris_demo );


    // Study corners
    imshow( source_window, src );
    cvtColor( src, src_gray, COLOR_BGR2GRAY );
    cornerHarris_demo( 0, 0 );

    waitKey();

    return 0;
}

/**
 * @function cornerHarris_demo
 * @brief Executes the corner detection and draw a circle around the possible corners
 */
void cornerHarris_demo( int, void* )
{
    // Detector parameters
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;
    int fpcnt=0;

    // FEATURE DETECTOR:  Detecting corners
    Mat dst = Mat::zeros( src.size(), CV_32FC1 );
    Mat dst_eig = Mat::zeros( src.size(), CV_32FC1 );
    Mat dst_eigvecs = Mat::zeros( src.size(), CV_32FC(6) );
    cornerHarris( src_gray, dst, blockSize, apertureSize, k );
    cornerMinEigenVal( src_gray, dst_eig, blockSize, apertureSize );
    cornerEigenValsAndVecs( src_gray, dst_eigvecs, blockSize, apertureSize );

    imshow("cornerHarris src_gray", src_gray);

    // Normalizing
    Mat dst_norm, dst_norm_scaled;
    normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
    convertScaleAbs( dst_norm, dst_norm_scaled );

    imshow("cornerHarris norm  ", dst_norm_scaled);

    cout << " =========> APPLY THRESHOLD TO CORNERNESS MAP c(H)   " << thresh << endl;
    // Drawing a circle around corners
    for( int i = 0; i < dst_norm.rows ; i++ )
    {
        for( int j = 0; j < dst_norm.cols; j++ )
        {
            if( (int) dst_norm.at<float>(i,j) > thresh )
            {
                circle( dst_norm_scaled, Point(j,i), 5,  Scalar(0), 2, 8, 0 );
                    cout<<"--------------------5x5 of corner "<<"-----------------------"<<endl;
                    for(int m = -2; m < 3; m++){
                        for(int n = -2; n < 3; n++){
                            int xPos = j+m;
                            int yPos = i+n;
                            Vec3b pixelVal = src.at<Vec3b>(xPos, yPos);
//                            newWindow.at<Vec3b>(xPos, yPos) = pixelVal;
                            cout<<pixelVal<<" ";
                        }
                        cout<<endl;
                    }
                
        cout << "FEATURE POINT :" << ++fpcnt << endl;
        cout << j << ", " << i << endl;
        cout << "c(H) " << dst.at<float>(i,j) << endl;
        cout << "Normalized c(H) " << dst_norm.at<float>(i,j) << endl;
        cout << "MinEigenValues " << dst_eig.at<float>(i,j) << endl;
        cout << "EigenVectors " << dst_eigvecs.at<Vec6f>(i,j) << endl;
        cout << endl;

        /* TODO:   Print the 15 element feature vector here - using code from last week's breakout */
        //cout << "FEATURE DESCRIPTOR INFORMATION" << endl;
        //cout << "Feature (5x5) at pnt " << "YOUR CODE" <<  endl;
                
                
        
            }
        }
    }

    /// Showing the result
    namedWindow( corners_window );
    imshow( corners_window, dst_norm_scaled );
}

void cornerHarris_demo2( int, void* )
{
    /// Detector parameters
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;
    int fpcnt=0;

    /// FEATURE DETECTOR:  Detecting corners
    Mat dst = Mat::zeros( src.size(), CV_32FC1 );
    Mat dst_eig = Mat::zeros( src.size(), CV_32FC1 );
    Mat dst_eigvecs = Mat::zeros( src.size(), CV_32FC(6) );
    cornerHarris( src_gray, dst, blockSize, apertureSize, k );
    cornerMinEigenVal( src_gray, dst_eig, blockSize, apertureSize );
    cornerEigenValsAndVecs( src_gray, dst_eigvecs, blockSize, apertureSize );

    imshow("cornerHarris src_gray", src_gray);

    /// Normalizing
    Mat dst_norm, dst_norm_scaled;
    normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
    convertScaleAbs( dst_norm, dst_norm_scaled );

    imshow("cornerHarris norm  ", dst_norm_scaled);


    cout << " =========> APPLY THRESHOLD TO CORNERNESS MAP c(H)   " << thresh << endl;
    /// Drawing a circle around corners
    for( int i = 0; i < dst_norm.rows ; i++ )
    {
        for( int j = 0; j < dst_norm.cols; j++ )
        {
            if( (int) dst_norm.at<float>(i,j) > thresh )
            {
                circle( dst_norm_scaled, Point(j,i), 5,  Scalar(0), 2, 8, 0 );
                    cout<<"--------------------5x5 of corner "<<"-----------------------"<<endl;
                    for(int m = -2; m < 3; m++){
                        for(int n = -2; n < 3; n++){
                            int xPos = j+m;
                            int yPos = i+n;
                            Vec3b pixelVal = src.at<Vec3b>(xPos, yPos);
//                            newWindow.at<Vec3b>(xPos, yPos) = pixelVal;
                            cout<<pixelVal<<" ";
                        }
                        cout<<endl;
                    }
                
        cout << "FEATURE POINT :" << ++fpcnt << endl;
        cout << j << ", " << i << endl;
        cout << "c(H) " << dst.at<float>(i,j) << endl;
        cout << "Normalized c(H) " << dst_norm.at<float>(i,j) << endl;
        cout << "MinEigenValues " << dst_eig.at<float>(i,j) << endl;
        cout << "EigenVectors " << dst_eigvecs.at<Vec6f>(i,j) << endl;
        cout << endl;

        /** TODO:   Print the 15 element feature vector here - using code from last week's breakout **/
        //cout << "FEATURE DESCRIPTOR INFORMATION" << endl;
        //cout << "Feature (5x5) at pnt " << "YOUR CODE" <<  endl;

        
            }
        }
    }

    /// Showing the result
    namedWindow( corners_window );
    imshow( corners_window, dst_norm_scaled );
}
