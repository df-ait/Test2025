#include <iostream>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
using namespace std;
using namespace cv;


int main()
{
    string path = "D:/Test2025/nums/3.png";
    //以灰度方式读取这张图
    cv::Mat img = imread(path ,IMREAD_GRAYSCALE);
    //创建窗口 (自由比例)
    cv::namedWindow("input",WINDOW_FREERATIO);
    //这里要保证图片显示窗口和你创建的窗口一致
    //如果你不指定窗口大小，imshow函数也会自己创建一个窗口，窗口格式永远与
    //图像大小保持一致(auto_size)，无法调整
    imshow("input",img);
    waitKey(0);
    return 0;
}
