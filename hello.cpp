#include <iostream>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
using namespace std;
using namespace cv;


int main()
{
    string path = "C:/Users/ddddffff/Pictures/Screenshots/1.png";
    cout<<"halo"<<endl;
    cv::Mat img = cv::imread(path);
    cv::imshow("img", img);    
    cv::waitKey(0);
    return 0;
}
