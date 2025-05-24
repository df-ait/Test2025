#include <bits/stdc++.h>
#include <winsock2.h>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <ws2tcpip.h>
#include "head/Matrix.h"
#include "head/Model.h"
#pragma comment(lib , "ws2_32.lib")


template<typename T>
void show(Matrix<T>& index){
    std::cout<<"\n";
      for(auto i = 0 ; i<index.line ; i++){
            for(auto j = 0 ; j<index.column; j++){
                std::cout<<index.matrix[i][j]<<" ";
            }
            std::cout<<"\n";
      }
      std::cout<<"\n\n";
   }


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
    //��ʼ��ȡͼ���Ӧ�ľ���
    std::vector<std::vector<float>>res;
    for(int i = 0 ; i < new_im.rows ; i++){
        std::vector<float>row;
        for(int j = 0 ; j < new_im.cols ; j++){
            int px = static_cast<int>(new_im.at<uchar>(i , j));
            row.push_back((float)px/255);
        }
        res.push_back(row);
    }
    Matrix<float>res_(res);
    show(res_);
    return Matrix<float>(res);
}

void create_socket(Matrix<float>& tranfer){
    WSAData net;
    if(WSAStartup(MAKEWORD(2,2) , &net) != 0){
        std::cerr<<"client:�����׽��ֿ�ʧ��\n\n";
        WSACleanup();
        return;
    }
    SOCKET se_matrix = socket(AF_INET , SOCK_STREAM , 0);
    if(se_matrix == INVALID_SOCKET){
        std::cerr<<"client:�����׽���ʧ��\n\n";
        WSACleanup();
        return;
    }
    sockaddr_in cit_matrix_addr;
    cit_matrix_addr.sin_family = AF_INET;
    cit_matrix_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &cit_matrix_addr.sin_addr);
    // SOCKET connect_fd = connect(se_matrix , (sockaddr*)&cit_matrix_addr , sizeof(cit_matrix_addr));
    if(connect(se_matrix , (sockaddr*)&cit_matrix_addr , sizeof(cit_matrix_addr)) == SOCKET_ERROR){
            std::cerr<<"client:�ͻ�����������ʧ��\n\n";
            closesocket(se_matrix);
            WSACleanup();
            return;
    } 
   
    std::cout<<"client:�ɹ����ӵ���������\n";

    std::vector<float>befor_cal =  tranfer.matrix[0];

    std::cout << "Ҫ����֮ǰ�ľ���: ";
    for (float f : befor_cal) {
        std::cout << f << " ";
    }
    std::cout << "\n";
    //�ȴ�������С
    size_t tranfer_m_size = befor_cal.size();

    send(se_matrix, (char*)&tranfer_m_size, sizeof(size_t), 0);
    //�ٰѾ�������ݴ���ȥ
    send(se_matrix , (char*)befor_cal.data() , sizeof(float)*tranfer_m_size , 0);
    
    //ͬ�����Ƚ��վ����С���ٽ���Ԫ��
    size_t beforcal_size;
    recv(se_matrix , (char*)&beforcal_size ,sizeof(size_t) , 0);
    std::vector<float>after(beforcal_size);
    recv(se_matrix , (char*)after.data() , sizeof(float)*beforcal_size , 0);
   
    std::cout << "ͨ��forward����󷵻صľ���: ";
    for (float f : befor_cal) {
        std::cout << f << " ";
    }
    std::cout << "\n";

    system("pause");
    closesocket(se_matrix);
    WSACleanup();
    return;
}


int main(){
    cv::Mat img_read;
    Matrix<float> img_matrix;
    std::string path;
    std::cout<<"Pls enter the path of img:";
    std::cin>>path;
    img_matrix = read_img(path , img_read);
    //����forward�м���
    //�Ƚ���ͨ��
    std::cout<<"Ҫ����ľ���:\n";
    show(img_matrix);
    create_socket(img_matrix);
    
    return 0;
}