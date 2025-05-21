#include <iostream>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
using namespace std;
using namespace cv;


int main()
{
    string path = "D:/DeBuG_opencv/pic1.png";
    cv::Mat img = imread(path);
    imshow("img",img);
    waitKey(0);
    return 0;
}
