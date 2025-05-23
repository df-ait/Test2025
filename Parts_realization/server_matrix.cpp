#include <bits/stdc++.h>
#include <winsock2.h>
#include "head/Matrix.h"
#include "head/Model.h"
#include "../json.hpp"
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
#pragma comment(lib , "ws2_32.lib")

// Matrix<float> read_img(const std::string path , cv::Mat target){
//     target = cv::imread(path , cv::IMREAD_GRAYSCALE);
//     cv::imshow("output",target);
//     cv::waitKey(1000);
//     if(target.empty()){
//         std::cout<<"Failed to load picture:"<<path<<"\n\n";
//         return Matrix<float>(0,0);
//     }
//     cv::Mat new_im;
//     cv::resize(target , new_im ,cv::Size(28,28) , 0 , 0 , cv::INTER_AREA);
//     cv::imshow("output" , new_im);
//     cv::waitKey(100);
//     //开始获取图像对应的矩阵
//     std::vector<std::vector<float>>res;
//     std::vector<float>row;
//     for(int i = 0 ; i < new_im.rows ; i++){
//         for(int j = 0 ; j < new_im.cols ; j++){
//             int px = static_cast<int>(new_im.at<uchar>(i , j));
//             row.push_back((float)px/255);
//         }
//     }
//     res.push_back(row);
//     return Matrix<float>(res);
// }
void create_connect(std::shared_ptr<Base_Model> only){
    WSAData net;
    if(WSAStartup(MAKEWORD(2,2) , &net) != 0){
        std::cerr<<"server:加载套接字库失败\n\n";
        WSACleanup();
        return ;
    }
    SOCKET listen_client = socket(AF_INET , SOCK_STREAM , 0);
    if(listen_client == INVALID_SOCKET){
        std::cerr<<"server:创建套接字失败\n\n";
        WSACleanup();
        return ;
    }
    sockaddr_in ser_matrix_addr;
    ser_matrix_addr.sin_family = AF_INET;
    ser_matrix_addr.sin_port = htons(8080);
    ser_matrix_addr.sin_addr.S_un.S_addr = INADDR_ANY;
    if(bind(listen_client , (sockaddr*)&ser_matrix_addr , sizeof(ser_matrix_addr)) == SOCKET_ERROR){
        std::cerr<<"server:绑定服务端端口和ip地址失败\n\n";
        closesocket(listen_client);
        WSACleanup();
        return ;
    }
    if(listen(listen_client , SOMAXCONN) == SOCKET_ERROR){
        std::cerr<<"server:服务端无法监听客户端连接请求\n\n";
        closesocket(listen_client);
        WSACleanup();
        return ;
    }
    
    sockaddr_in client_matrix;
    int cl_size = sizeof(client_matrix);
    SOCKET connect_fd = accept(listen_client , (sockaddr*)&client_matrix , &cl_size);
    if(connect_fd == INVALID_SOCKET){
        std::cerr<<"server:客户端请求连接失败\n\n";
        closesocket(listen_client);
        WSACleanup();
        return ;
    } 
    std::cout<<"server:成功链接到客户端\n";  
    
    //先接受矩阵
    size_t pre_matrix_size;
    recv(connect_fd , (char*)&pre_matrix_size , sizeof(size_t) , 0);
    std::cout<<"pre_matrix_size:"<<pre_matrix_size<<std::endl;
    std::vector<float>vec(pre_matrix_size);
    recv(connect_fd , (char*)vec.data() , pre_matrix_size*sizeof(float) , 0);
    std::cout << "Received vector from client: ";
    for (float f : vec) {
        std::cout << f << " ";
    }
    std::cout << "\n";
    std::vector<std::vector<float>>mid;
    mid.push_back(vec);
    Matrix<float>pic(mid);
    //计算过后再发回去
    
    Matrix<float>res = only->forward(pic);
    std::vector<float>output = res.matrix[0];
    size_t bf_matrix_size = output.size();
    std::cout<<"res.size:"<<bf_matrix_size<<std::endl;
    send(connect_fd , (char*)&bf_matrix_size , sizeof(size_t) , 0);
    send(connect_fd , (char*)output.data() , bf_matrix_size*sizeof(float) , 0);
    std::cout << "Sent processed vector back to client\n";

    closesocket(listen_client);
    closesocket(connect_fd);
    WSACleanup();
    return;
}

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

int main(){
    std::string folder;
    std::cout<<"Pls enter the folder path:";
    std::cin>>folder;

    nlohmann::json j;
    std::ifstream fin(folder+"/meta.json");
    j = nlohmann::json::parse(fin);

    std::cout<<"json path is"<<folder+"/meta.json\n\n";
    std::shared_ptr<Base_Model> only;
    if(j["type"] == "fp32"){
        std::cout<<"is float\n\n";
        only = std::make_shared<Model<float>>(folder , j);
    }
    else if(j["type"] == "fp64"){
        std::cout<<"is double\n\n";
        only = std::make_shared<Model<double>>(folder , j);
    }
    else{
        std::cout<<"Has no match type\n\n";
    }
    create_connect(only);
    /*传入一个矩阵*/
    // cv::Mat img_read;
    // Matrix<float> img_matrix;
    // std::string path;
    // std::cout<<"Pls enter the path of img:";
    // std::cin>>path;
    // img_matrix = read_img(path , img_read);
    // Matrix<float>push = only->forward(img_matrix);
    //show(push);
}