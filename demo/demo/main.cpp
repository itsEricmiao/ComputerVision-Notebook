/*
*    GOAL:  Display only the GREEN channel of the apple image in W2
*           Show both the original and the green channel
*
*/

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>
#include <vector>
using namespace cv;
using namespace std;

int main( int argc, const char** argv ){
//    Mat src(3,3,CV_8UC1, [[1,2,3], [1,2,3], [1,2,3]]);
//    Mat src_gray;
    
//    Mat dst;
//    Laplacian(src, dst, CV_16S,3,1,0,BORDER_DEFAULT);
    
    vector<vector<uchar>> vec = {{1,2,3}, {1,2,3}, {1,2,3}};
    Mat src( 3, 3, CV_8UC1, vec.data());
    
    cout<<src.rows<<endl;
    for (int i = 0; i < src.rows; i++){
        cout<<src.at<Vec3b>(i)<<endl;
    }
    cout<<"hello World"<<endl;
    return(0);
}

