#include <bits/stdc++.h>
#include <winsock2.h>
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

int main(){
    WSAData net;
    if(WSAStartup(MAKEWORD(2,2) , &net) != 0){
        std::cerr<<"client:加载套接字库失败\n\n";
        WSACleanup();
        return -1;
    }
    SOCKET se_matrix = socket(AF_INET , SOCK_STREAM , 0);
    if(se_matrix == INVALID_SOCKET){
        std::cerr<<"client:创建套接字失败\n\n";
        WSACleanup();
        return -1;
    }
    sockaddr_in cit_matrix_addr;
    cit_matrix_addr.sin_family = AF_INET;
    cit_matrix_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &cit_matrix_addr.sin_addr);
    // SOCKET connect_fd = connect(se_matrix , (sockaddr*)&cit_matrix_addr , sizeof(cit_matrix_addr));
    if(connect(se_matrix , (sockaddr*)&cit_matrix_addr , sizeof(cit_matrix_addr)) == SOCKET_ERROR){
            std::cerr<<"client:客户端请求连接失败\n\n";
            closesocket(se_matrix);
            WSACleanup();
            return -1;
    } 
   
        std::cout<<"client:成功连接到服务器端\n";


        std::vector<float>befor_cal = {0.1,0.2,0.3};

        std::cout << "Sending vector to server: ";
        for (float f : befor_cal) {
            std::cout << f << " ";
        }
        std::cout << "\n";
        //先传入矩阵大小
        size_t tranfer_m_size = befor_cal.size();

        send(se_matrix, (char*)&tranfer_m_size, sizeof(size_t), 0);

        send(se_matrix , (char*)befor_cal.data() , sizeof(float)*tranfer_m_size , 0);
    
        size_t beforcal_size;
        recv(se_matrix , (char*)&beforcal_size ,sizeof(size_t) , 0);
        std::vector<float>after(beforcal_size);
        recv(se_matrix , (char*)after.data() , sizeof(float)*beforcal_size , 0);
   
        std::cout << "Received processed vector from server: ";
        for (float f : befor_cal) {
            std::cout << f << " ";
        }
        std::cout << "\n";

    
    closesocket(se_matrix);
    WSACleanup();
    return 0;
}