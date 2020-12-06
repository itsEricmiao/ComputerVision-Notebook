#include <fstream>
#include <iostream>
#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

using namespace std;
using namespace cv;
int main()
{
    // Read input images
    cv::Mat image1 = cv::imread("data/pic_a.jpg");
    cv::Mat image2 = cv::imread("data/pic_b.jpg");
    if (!image1.data || !image2.data){
        std::cout<<"No image is found"<<std::endl;
        return 0;
    }

    std::ifstream data1("data/pts2d-pic_a.txt");
    cv::Point img1[20];
    std::ifstream data2("data/pts2d-pic_b.txt");
    cv::Point img2[20];

    if(data1.is_open())
    {
        for (int i = 0; i < 20; i++){
            data1 >> img1[i].x >> img1[i].y;
        }
    }
    if(data2.is_open())
    {
        for (int i = 0; i < 20; i++){
            data2 >> img2[i].x >> img2[i].y;
        }
    }

//    (43,203) and (22, 248)?
//    (738, 728) and (1064, 470) ?

    Point pts1 = img1[1];
    Point pts2 = img2[1];

    vector<Point2f> points1;
    vector<Point2f> points2;
    for(int i = 0; i < 20; i++){
        points1.push_back(img1[i]);
    }

    for(int i = 0; i < 20; i++){
        points2.push_back(img2[i]);
    }

    Mat F = findFundamentalMat(points1,points2, FM_RANSAC);

    cout<<"Matrix:"<<endl;
    cout<<F<<endl;

    vector <Vec3f> lines1, lines2;
    computeCorrespondEpilines(points1, 1, F, lines1);
    computeCorrespondEpilines(points2, 2, F, lines2);

    cout<<"Lines 1:"<<endl;
    for(int i = 0; i < lines1.size(); i++){
        cout<<lines1[i]<<endl;
    }

    cout<<"Lines 2:"<<endl;
    for(int i = 0; i < lines2.size(); i++){
        cout<<lines2[i]<<endl;
    }

    cv::Mat outImg(image1.rows, image1.cols*2, CV_8UC3);
    cv::RNG rng(0);
    cv::Rect rect1(0,0, image1.cols, image1.rows);
    cv::Rect rect2(image1.cols, 0, image1.cols, image1.rows);
    image1.copyTo(outImg(rect1));
    image2.copyTo(outImg(rect2));

    for(size_t i=0; i<lines1.size(); i++)
    {
        Scalar color(rng(256),rng(256),rng(256));

        cv::line(outImg(rect2),
        cv::Point(0,-lines1[i][2]/lines1[i][1]),
        cv::Point(image1.cols,-(lines1[i][2]+lines1[i][0]*image1.cols)/lines1[i][1]),color);
        cv::circle(outImg(rect1), points1[i], 10, color, -1);

        cv::line(outImg(rect1),
        cv::Point(0,-lines2[i][2]/lines2[i][1]),
        cv::Point(image2.cols,-(lines2[i][2]+lines2[i][0]*image2.cols)/lines2[i][1]),color);
        cv::circle(outImg(rect2), points2[i], 10, color, -1);
    }

    cv::imshow("output", outImg);
    cv::waitKey(0);


    return 0;
}
