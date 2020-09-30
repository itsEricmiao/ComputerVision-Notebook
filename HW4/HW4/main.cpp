//
//  main.cpp
//  HW4
//
//  Created by Eric Miao on 9/28/20.
//

#include "opencv2/core.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <stdio.h>

using namespace cv;
using namespace std;

//static void help(const char ** argv)
//{
//    printf("\nThis program demonstrated the use of the discrete Fourier transform (dft)\n"
//           "The dft of an image is taken and it's power spectrum is displayed.\n"
//           "Usage:\n %s [image_name -- default lena.jpg]\n",argv[0]);
//}
//
//
//const char* keys =
//{
//    "{help h||}{@image|graypattern.tiff|input image file}"
//};

//farther out means stronger edges -> higher freq -> more changes between light and dark
//convert through dft: higher freq -> compressed sin/cosine wave
int main(int argc, const char ** argv)
{
    printf("%d", argc);
    int i = 11;
    
    
    string filename = argv[i];
    Mat img = imread(samples::findFile(filename), IMREAD_GRAYSCALE);
    
    if( img.empty() )
    {
        printf("Cannot read image file: %s\n", filename.c_str());
        return -1;
    }
    

    int M = getOptimalDFTSize( img.rows );
    int N = getOptimalDFTSize( img.cols );
    Mat padded;
    copyMakeBorder(img, padded, 0, M - img.rows, 0, N - img.cols, BORDER_CONSTANT, Scalar::all(0));

    
//    Mat padded (256, 256, CV_8UC1, Scalar(0));
//    for(int i = 0; i < 256; i++){
//        for (int j = 0; j < 256; j++){
//            padded.at<uchar>(i,j) = rand()%256;
//        }
//    }

    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexImg;
    
    
    
    merge(planes, 2, complexImg);
    
    dft(complexImg, complexImg);
    // complexImg:
    
    // 1. Real: mathmatical representation - magnitude
    // 2. Imaginary: phase range: [0,2pi]
    
    // compute log(1 + sqrt(Re(DFT(img))**2 + Im(DFT(img))**2))
    split(complexImg, planes);
    
    Mat ph;
    phase(planes[0], planes[1], ph);
    magnitude(planes[0], planes[1], planes[0]);
    
    Mat mag = planes[0];
//
    mag += Scalar::all(1);
    log(mag, mag);

//     crop the spectrum, if it has an odd number of rows or columns
    mag = mag(Rect(0, 0, mag.cols & -2, mag.rows & -2));
    int cx = mag.cols/2;
    int cy = mag.rows/2;

//     rearrange the quadrants of Fourier image
//     so that the origin is at the image center
    Mat tmp;
    Mat q0(mag, Rect(0, 0, cx, cy));
    Mat q1(mag, Rect(cx, 0, cx, cy));
    Mat q2(mag, Rect(0, cy, cx, cy));
    Mat q3(mag, Rect(cx, cy, cx, cy));

    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);

    normalize(mag, mag, 0, 1, NORM_MINMAX);

    imshow("spectrum magnitude", mag);
    imshow("phase", ph);
    imshow("original image", padded);
    waitKey();
    return 0;
}
