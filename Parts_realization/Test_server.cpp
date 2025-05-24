#include <bits/stdc++.h>
#include <winsock2.h>
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


int main(){
    WSAData net;
    if(WSAStartup(MAKEWORD(2,2) , &net) != 0){
        std::cerr<<"server:加载套接字库失败\n\n";
        WSACleanup();
        return -1;
    }
    SOCKET listen_client = socket(AF_INET , SOCK_STREAM , 0);
    if(listen_client == INVALID_SOCKET){
        std::cerr<<"server:创建套接字失败\n\n";
        WSACleanup();
        return -1;
    }
    sockaddr_in ser_matrix_addr;
    ser_matrix_addr.sin_family = AF_INET;
    ser_matrix_addr.sin_port = htons(8080);
    ser_matrix_addr.sin_addr.S_un.S_addr = INADDR_ANY;
    if(bind(listen_client , (sockaddr*)&ser_matrix_addr , sizeof(ser_matrix_addr)) == SOCKET_ERROR){
        std::cerr<<"server:绑定服务端端口和ip地址失败\n\n";
        closesocket(listen_client);
        WSACleanup();
        return -1;
    }
    if(listen(listen_client , SOMAXCONN) == SOCKET_ERROR){
        std::cerr<<"server:服务端无法监听客户端连接请求\n\n";
        closesocket(listen_client);
        WSACleanup();
        return -1;
    }
        sockaddr_in client_matrix;
        int cl_size = sizeof(client_matrix);
        SOCKET connect_fd = accept(listen_client , (sockaddr*)&client_matrix , &cl_size);
        if(connect_fd == INVALID_SOCKET){
            std::cerr<<"server:客户端请求连接失败\n\n";
            closesocket(listen_client);
            WSACleanup();
            return -1;
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

       //计算过后再发回去
       std::vector<float>res = {0.11,0.22,0.33};
       size_t bf_matrix_size = res.size(); 
        std::cout<<"res.size:"<<bf_matrix_size<<std::endl;
       send(connect_fd , (char*)&bf_matrix_size , sizeof(size_t) , 0);
       send(connect_fd , (char*)res.data() , bf_matrix_size*sizeof(float) , 0);
        std::cout << "Sent processed vector back to client\n";

    closesocket(listen_client);
    closesocket(connect_fd);
    WSACleanup();
    return 0;
}