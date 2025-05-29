#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include "Parts_realization/head/Model.h"
#include "Parts_realization/head/Matrix.h"
#include <bits/stdc++.h>
#include <ws2tcpip.h>
#pragma comment(lib , "ws2_32.lib")

//全局变量 绘画画布，绘画状态，上一个点的坐标, 窗口名字
cv::Mat cavan;
bool is_draw = false ,is_running = true;
cv::Point prePoint(-1 , -1);
std::string windowsname = "img";

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

void draw(int event ,int x ,int y ,int flags ,void* prama);
void create_socket(Matrix<float>& tranfer);
void process_img();

int main(){
    //要有一个循环不断地更新画布情况，才能看出来图像变化
    cv::namedWindow(windowsname);
    //创建一个空白画布
    cavan = cv::Mat(500 , 500 , CV_8UC3 , cv::Scalar(255 ,255 ,255));
    static std::string text = "Prediction:";
    cv::putText(cavan , text , cv::Point(25 , 25) , CV_FONT_HERSHEY_SIMPLEX , 1, cv::Scalar(0 , 255 , 0) , 2);
    cv::putText(cavan , "Press 'c' to clear" , cv::Point(25 , 465) , CV_FONT_HERSHEY_SIMPLEX , 1, cv::Scalar(0 , 255 , 0) , 2);
    cv::putText(cavan , "Press 'a' to commit" , cv::Point(25 , 485) , CV_FONT_HERSHEY_SIMPLEX , 1, cv::Scalar(0 , 255 , 0) , 2);
    cv::setMouseCallback(windowsname , draw , &cavan);
    //std::thread process = std::thread(process_img);
    while (1)
    {
        auto key = cv::waitKey(10);
        if(key == 27) {
            is_running = false;
            break;//按下Esc键就退出
        }
        if(key == 'c'||key == 'C'){
            cavan = cv::Mat(500 , 500 , CV_8UC3 , cv::Scalar(255 ,255 ,255));
            text = "Prediction:";
            cv::putText(cavan , text , cv::Point(25 , 25) , CV_FONT_HERSHEY_SIMPLEX , 1, cv::Scalar(0 , 255 , 0) , 2);
            cv::putText(cavan , "Press 'c' to clear" , cv::Point(25 , 465) , CV_FONT_HERSHEY_SIMPLEX , 1, cv::Scalar(0 , 255 , 0) , 2);
            cv::putText(cavan , "Press 'a' to commit" , cv::Point(25 , 485) , CV_FONT_HERSHEY_SIMPLEX , 1, cv::Scalar(0 , 255 , 0) , 2);
        }       
        if(key == 'a'||key == 'A'){
            process_img();
        }
        // process_img();
        // std::thread(process_img).detach();
        cv::imshow(windowsname , cavan);
    }
    //process.join();
    cv::destroyAllWindows();
    return 0;
}

void draw(int event ,int x ,int y ,int flags ,void* prama){
    if(event == cv::EVENT_LBUTTONDOWN){
        //鼠标左键按下时
        is_draw = true;
        prePoint = cv::Point(x , y);//储存第一个点
    }
    else if(event == cv::EVENT_MOUSEMOVE && is_draw){
        //当鼠标移动并且属于绘画状态
        //和前一个点连起来画成直线
        cv::line(cavan , prePoint , cv::Point(x , y) , cv::Scalar(0 , 0 , 0) , 50 , cv::LINE_AA);
        prePoint = cv::Point(x , y);
    }else if(event == cv::EVENT_LBUTTONUP){
        //当左键起来的时候，绘画结束，要处理最后一个点
        cv::line(cavan , prePoint , cv::Point(x , y) , cv::Scalar(0 , 0 , 0) , 50 , cv::LINE_AA);
        prePoint = cv::Point(-1 ,-1);
        is_draw = false;
    }
}

void create_socket(Matrix<float>& tranfer){
    WSAData net;
    if(WSAStartup(MAKEWORD(2,2) , &net) != 0){
        std::cerr<<"client:加载套接字库失败\n\n";
        WSACleanup();
        return;
    }
    SOCKET se_matrix = socket(AF_INET , SOCK_STREAM , 0);
    if(se_matrix == INVALID_SOCKET){
        std::cerr<<"client:创建套接字失败\n\n";
        WSACleanup();
        return;
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
            return;
    } 
    std::cout<<"client:成功连接到服务器端\n";
    std::vector<float>befor_cal =  tranfer.matrix[0];
    size_t tranfer_m_size = befor_cal.size();

    send(se_matrix, (char*)&tranfer_m_size, sizeof(size_t), 0);
    //再把矩阵的数据传进去
    send(se_matrix , (char*)befor_cal.data() , sizeof(float)*tranfer_m_size , 0);
    //同样的先接收矩阵大小，再接收元素
    size_t beforcal_size;
    recv(se_matrix , (char*)&beforcal_size ,sizeof(size_t) , 0);
    std::vector<float>after(beforcal_size);
    recv(se_matrix , (char*)after.data() , sizeof(float)*beforcal_size , 0);
    // std::cout << "通过forward计算后返回的矩阵: ";
    // for (float f : after) {
    //     std::cout << f << " ";
    // }
    // std::cout << "\n";
    auto max_it = std::max_element(after.begin() , after.end());
    auto max_index = std::distance(after.begin() , max_it);
    std::string text = "Prediction:" + std::to_string(max_index);
    std::cout<<text<<"\n";
    cv::putText(cavan , text , cv::Point(25 , 25) , CV_FONT_HERSHEY_SIMPLEX , 1, cv::Scalar(0 , 255 , 0) , 2);
    closesocket(se_matrix);
    WSACleanup();
    return;
}

void process_img(){
    //std::lock_guard<std::mutex> lock(mtx);
    //std::lock_guard<std::mutex> lock(mtx);
    cv::Mat img = cavan.clone();
    cv::Mat img_gray;
    //先读取成灰度图，再进行缩小图片
    cvtColor(img , img_gray , cv::COLOR_BGR2GRAY);
    cv::Mat img_gray_s;
    cv::resize(img_gray , img_gray_s , cv::Size(28 ,28) , 0 , 0 ,cv::INTER_AREA);
    std::vector<std::vector<float>>res;
    std::vector<float>rows;
    for(int i = 0 ; i < img_gray_s.rows ; i++){
        for(int j = 0 ; j < img_gray_s.cols ; j++){
            int px = static_cast<int>(img_gray_s.at<uchar>(i , j));
            rows.push_back((float)px/255);
        }
    }
    res.push_back(rows);
    std::cout<<"矩阵元素个数:"<<rows.size()<<"\n";
    //return Matrix<float>(res);
    Matrix<float>tranfer(res);
    create_socket(tranfer);
    //std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}