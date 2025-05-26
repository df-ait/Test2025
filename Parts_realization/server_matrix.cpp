#include <bits/stdc++.h>
#include <winsock2.h>
#include "head/Matrix.h"
#include "head/Model.h"
#include "../json.hpp"
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
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
    // std::cout << "从客户端接收到的函数: ";
    // for (float f : vec) {
    //     std::cout << f << " ";
    // }
    // std::cout << "\n";
    std::vector<std::vector<float>>mid;
    mid.push_back(vec);
    Matrix<float>pic(mid);
    //计算过后再发回去
    /****************************************************************
     * ****** */
    auto start = std::chrono::high_resolution_clock::now();
    Matrix<float>res = only->forward(pic);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout<<"当前forward函数用时为:"<<duration.count()<<"ms\n";
    
    std::cout<<"计算后的矩阵"<<res.line<<" "<<res.column<<"\n";
    show(res);
    std::vector<float>output = res.matrix[0];
    size_t bf_matrix_size = output.size();
    //std::cout<<"res.size:"<<bf_matrix_size<<std::endl;
    send(connect_fd , (char*)&bf_matrix_size , sizeof(size_t) , 0);
    send(connect_fd , (char*)output.data() , bf_matrix_size*sizeof(float) , 0);
    //std::cout << "将矩阵传回到客户端\n";

    closesocket(listen_client);
    closesocket(connect_fd);
    WSACleanup();
    return;
}

int main(){
    std::string folder = "D:/Test2025/mnist-fc";
    // std::cout<<"Pls enter the folder path:";
    // std::cin>>folder;

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
}