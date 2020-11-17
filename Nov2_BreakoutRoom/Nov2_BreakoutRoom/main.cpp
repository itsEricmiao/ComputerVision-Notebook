// Example 11-2. Code for perspective transformation
// Compute a perspective transformation between the 4 src control points
// in srcQuad to 4 dst control points in dstQuad and apply it the image.

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {

  if(argc != 2) {
    cout << "Perspective Warp\nUsage: " <<argv[0] <<" <imagename>\n" << endl;
    return -1;
  }

  cv::Mat src = cv::imread(argv[1],1);
  if( src.empty() ) { cout << "Can not load " << argv[1] << endl; return -1; }

  cv::Point2f srcQuad[] = {
    cv::Point2f(0, 0),                   // src Top left
    cv::Point2f(src.cols-1, 0),          // src Top right
    cv::Point2f(src.cols-1, src.rows-1), // src Bottom right
    cv::Point2f(0, src.rows-1)           // src Bottom left
  };

  cv::Point2f dstQuad[] = {
    cv::Point2f(src.cols*0.05f, src.rows*0.33f),
    cv::Point2f(src.cols*0.9f, src.rows*0.25f),
    cv::Point2f(src.cols*0.8f, src.rows*0.9f),
    cv::Point2f(src.cols*0.2f, src.rows*0.7f)
  };
    cout<<srcQuad[0]<<endl;
    cout<<dstQuad[0]<<endl;
    
    cout<<srcQuad[1]<<endl;
    cout<<dstQuad[1]<<endl;
    
    cout<<srcQuad[2]<<endl;
    cout<<dstQuad[2]<<endl;
    
    cout<<srcQuad[3]<<endl;
    cout<<dstQuad[3]<<endl;
    
    


  // COMPUTE PERSPECTIVE MATRIX
//  cv::Mat warp_mat = cv::getPerspectiveTransform(srcQuad, dstQuad);
    double a = 1;
    double b = 0;
    double c = 0;
    double d = 0;
    double e = 1;
    double f = 0;
    double g = 0;
    double h = 0;
    double i = 4;
    
  cv::Mat warp_mat = (cv::Mat_<double>(3,3) << a,b,c,d,e,f,g,h,i);
  cv::Mat dst;
  cv::warpPerspective(src, dst, warp_mat, src.size(), cv::INTER_LINEAR,
                      cv::BORDER_CONSTANT, cv::Scalar());
    
  cv::Mat src_2 = dst.clone();
  for( int i = 0; i < 4; i++ )
    {
        cv::circle(dst, dstQuad[i], 5, cv::Scalar(255, 0, 255), -1, cv::LINE_AA);
    }

    
  cv::imshow("Scalling", dst);
  cv::waitKey();
  return 0;
}
