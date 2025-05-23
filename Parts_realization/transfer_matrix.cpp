#include <bits/stdc++.h>
#include <winsock2.h>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
#include "head/Matrix.h"
#include "head/Model.h"
#pragma comment(lib , "ws2_32.lib")

Matrix<float> read_img(const std::string path , cv::Mat target){
    target = cv::imread(path , cv::IMREAD_GRAYSCALE);
    if(target.empty()){
        std::cout<<"Failed to load picture:"<<path<<"\n\n";
        return Matrix<float>(0,0);
    }
    cv::Mat new_im;
    cv::resize(target , new_im ,cv::Size(28,28) , 0 , 0 , cv::INTER_AREA);
    cv::imshow("output" , new_im);
    cv::waitKey(100);
    //开始获取图像对应的矩阵
    std::vector<std::vector<float>>res;
    for(int i = 0 ; i < new_im.rows ; i++){
        std::vector<float>row;
        for(int j = 0 ; j < new_im.cols ; j++){
            int px = static_cast<int>(new_im.at<uchar>(i , j));
            row.push_back((float)px/255);
        }
        res.push_back(row);
    }
    return Matrix<float>(res);
}

void create_socket(){
    WSADATA net;
    if(WSAStartup(MAKEWORD(2 ,2) , &net)!=0){
        std::cerr<<"WSAStarup failed\n\n";
        return;
    }
    SOCKET tranfer = socket(AF_INET , SOCK_STREAM , 0);
    if(tranfer == INVALID_SOCKET){
        std::cerr<<"Create socket failed\n\n";
        WSACleanup();
        return;
    }
    sockaddr_in tranfer_addr;
    tranfer_addr.sin_family = AF_INET;
    tranfer_addr.sin_port = htons(8080);
    tranfer_addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(tranfer , (sockaddr*)&tranfer_addr , sizeof(tranfer_addr)) == SOCKET_ERROR){
        std::cerr<<"Bind IP and port failed\n\n";
        return;
    }
    if(listen(tranfer , SOMAXCONN) == SOCKET_ERROR){
        std::cerr<<"Bugging failed\n\n";
        return;
    }
    std::cout<<"Tranfer Connected to port 8080 successfully\n\n";
}

int main(){
    cv::Mat img_read;
    Matrix<float> img_matrix;
    std::string path;
    std::cout<<"Pls enter the path of img:";
    std::cin>>path;
    img_matrix = read_img(path , img_read);
    //传入forward中计算
    //先建立通信
    create_socket();

    return 0;
}