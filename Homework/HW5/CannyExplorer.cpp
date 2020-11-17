#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;

Mat src, src_gray;
Mat dst, detected_edges;
int lowThreshold = 0;
const int max_lowThreshold = 100;
int highThreshold = 0;
const int max_highThreshold = 100;
//const int ratio = 3;
int sigma= 3;
const int kernel_size = 3;
const char* window_name = "Edge Map";
static void CannyThreshold(int, void*)
{
    cout << " Threshold 1: " << lowThreshold << " Threshold 2: " << highThreshold << " Sigma " << sigma << endl;
    sigma = max (3, sigma);
    blur( src_gray, detected_edges, Size(sigma,sigma) );
    Canny( detected_edges, detected_edges, lowThreshold, highThreshold, kernel_size );
    dst = Scalar::all(0);
    src.copyTo( dst, detected_edges);
    imshow( window_name, dst );
}
int main( int argc, char** argv )
{
  src = imread( "flowergray.png", IMREAD_COLOR ); // Load an image
  if( src.empty() )
  {
    std::cout << "Could not open or find the image!\n" << std::endl;
    std::cout << "Usage: " << argv[0] << " <Input image>" << std::endl;
    return -1;
  }
  dst.create( src.size(), src.type() );
  cvtColor( src, src_gray, COLOR_BGR2GRAY );
  namedWindow( window_name, WINDOW_AUTOSIZE );
  createTrackbar( "Sigma:", window_name, &sigma, 7, CannyThreshold );
  createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );
  createTrackbar( "Max Threshold:", window_name, &highThreshold, max_highThreshold, CannyThreshold );
  CannyThreshold(0, 0);
  waitKey(0);
  return 0;
}
