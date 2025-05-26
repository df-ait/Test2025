#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>

cv::Mat canvas;
bool is_draw = false;
bool wait = false;
std::string windowsname = "matrix";
std::chrono::time_point<std::chrono::system_clock> last_write;
// template<typename T>
// void show(Matrix<T>& index){
//     std::cout<<"\n";
//       for(auto i = 0 ; i<index.line ; i++){
//             for(auto j = 0 ; j<index.column; j++){
//                 std::cout<<index.matrix[i][j]<<" ";
//             }
//             std::cout<<"\n";
//       }
//       std::cout<<"\n\n";
//    }


void read_img(){
    //先创建一个副本，记录此刻画布上的内容
    cv::Mat temp = canvas.clone();
    //将副本中存储的内容转换为灰度图
    cv::cvtColor(temp , temp , cv::COLOR_BGR2GRAY);
    cv::Mat new_im;
    cv::resize(temp , new_im , cv::Size(28 , 28) ,0 , 0);
    std::vector<std::vector<float>>res;
    std::vector<float>row;
    for(int i = 0 ; i < new_im.rows ; i++){
        for(int j = 0 ; j < new_im.cols ; j++){
            int px = static_cast<int>(new_im.at<uchar>(i , j));
            row.push_back((float)px/255);
        }
    }
     res.push_back(row);
     std::cout<<"此刻的矩阵:\n";
     for(auto i = 0 ; i<res.size() ; i++){
            for(auto j = 0 ; j<res[0].size(); j++){
                std::cout<<res[i][j]<<" ";
            }
            std::cout<<"\n";
      }
      std::cout<<"\n\n";
};

void onMouse(int event , int x ,int y ,int flags , void*userdata){
   if(event == cv::EVENT_LBUTTONDOWN){
        is_draw = true;
        cv::line(canvas , cv::Point(x,y) , cv::Point(x ,y) , cv::Scalar(40 , 81 , 127) , 10 ,cv::LINE_8);
        //read_img();
   }
    else if(event == cv::EVENT_MOUSEMOVE){
        if(is_draw){
            cv::line(canvas , cv::Point(x,y) , cv::Point(x ,y) , cv::Scalar(40 , 81 , 127) ,10 , cv::LINE_AA);
            //read_img();
        }   
    }
    else if(event == cv::EVENT_LBUTTONUP){
        if(is_draw){
            is_draw = false;
            last_write = std::chrono::system_clock::now();
            wait = true;
            cv::line(canvas , cv::Point(x,y) , cv::Point(x ,y) , cv::Scalar(40 , 81 , 127) ,10 , cv::LINE_AA);
            read_img();
        }   
    }
}

int main(){
    cv::namedWindow(windowsname);
    canvas = cv::Mat::ones(280 , 280 , CV_8UC3);//为了得到纯白画布
    canvas.setTo(cv::Scalar(255 ,255 ,255));
    cv::setMouseCallback(windowsname , onMouse , nullptr);
    cv::putText(canvas , "Draw a digital" ,cv::Point(10 , 10) ,cv::FONT_HERSHEY_SIMPLEX , 0.8 , cv::Scalar(0 ,0 ,255) , 2);
    while (1)
    {
        if(wait){
            auto now = std::chrono::system_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_write);
            if(duration.count() > 15000){
                read_img();
                wait = false;
            }
        }
        cv::imshow(windowsname , canvas);
        auto key = cv::waitKey(10);
        if(key == 27) break;
        else if(key == 'c'||key == 'C'){
            canvas = cv::Mat::ones(280 ,280 ,CV_8UC3);
            canvas.setTo(cv::Scalar(255 ,255 ,255));
            cv::putText(canvas , "Draw a digital" ,cv::Point(10 , 10) ,cv::FONT_HERSHEY_SIMPLEX , 0.8 , cv::Scalar(0 ,0 ,255) , 2);
        }
    }
}
