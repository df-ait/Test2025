#include <opencv2/opencv.hpp>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

using namespace cv;
using namespace std;

// 全局变量
Mat canvas;  // 绘图画布
Mat probDisplay;  // 概率显示区域

Point previousPoint(-1, -1);
bool isDrawing = false;

mutex canvasMutex;

vector<float> digitProbabilities(10, 0.0f);  // 存储0-9的概率
atomic<bool> running(true);

// 鼠标回调函数
void onMouse(int event, int x, int y, int flags, void* param) {
    switch (event) {
        case EVENT_LBUTTONDOWN://左键按下 开始绘画
            isDrawing = true;
            previousPoint = Point(x, y);
            break;
            
        case EVENT_MOUSEMOVE:
            if (isDrawing && x < 600) {  // 限制在绘图区域
                lock_guard<mutex> lock(canvasMutex);
                if (previousPoint.x >= 0 && previousPoint.y >= 0) {
                    line(canvas, previousPoint, Point(x, y), Scalar(0, 0, 255), 2, LINE_AA);
                    previousPoint = Point(x, y);
                }
            }
            break;
            
        case EVENT_LBUTTONUP://左键起来
            isDrawing = false;
            previousPoint = Point(-1, -1);
            running = false;
            // 这里可以添加识别逻辑，更新digitProbabilities
            // 模拟随机概率更新
            for (int i = 0; i < 10; ++i) {
                digitProbabilities[i] = (rand() % 100) / 100.0f;
            }
            // 归一化
            float sum = accumulate(digitProbabilities.begin(), digitProbabilities.end(), 0.0f);
            for (auto& p : digitProbabilities) p /= sum;
            break;
    }
}

// 更新概率显示区域
void updateProbabilityDisplay() {
    probDisplay.setTo(Scalar(255, 255, 255));
    
    // 绘制坐标轴
    line(probDisplay, Point(20, 30), Point(20, 180), Scalar(0, 0, 0), 2);
    line(probDisplay, Point(20, 180), Point(200, 180), Scalar(0, 0, 0), 2);
    
    // 绘制柱状图
    float maxProb = *max_element(digitProbabilities.begin(), digitProbabilities.end());
    if (maxProb <= 0) maxProb = 1.0f;
    
    for (int i = 0; i < 10; ++i) {
        int barHeight = static_cast<int>((digitProbabilities[i] / maxProb) * 150);
        rectangle(probDisplay, 
                 Point(30 + i * 18, 180 - barHeight),
                 Point(45 + i * 18, 180),
                 Scalar(100, 100, 255), FILLED);
        rectangle(probDisplay, 
                 Point(30 + i * 18, 180 - barHeight),
                 Point(45 + i * 18, 180),
                 Scalar(0, 0, 0), 1);
        
        // 显示数字标签
        putText(probDisplay, to_string(i), 
                Point(35 + i * 18, 200), 
                FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 1);
        
        // 显示概率值
        if (digitProbabilities[i] > 0.05) {
            string probText = format("%.1f%%", digitProbabilities[i] * 100);
            putText(probDisplay, probText,
                    Point(30 + i * 18, 170 - barHeight),
                    FONT_HERSHEY_SIMPLEX, 0.3, Scalar(0, 0, 0), 1);
        }
    }
    
    // 添加标题
    putText(probDisplay, "Digit Probabilities", Point(30, 20), 
            FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 0, 0), 1);
}

// 显示线程
void displayThread() {
    while (running) {
        lock_guard<mutex> lock(canvasMutex);
        
        // 创建组合图像
        Mat display(480, 800, CV_8UC3, Scalar(255, 255, 255));
        
        // 放置绘图区域 (600x480)
        Mat canvasROI = display(Rect(0, 0, 600, 480));
        canvas.copyTo(canvasROI);
        
        // 更新概率显示
        updateProbabilityDisplay();
        
        // 放置概率显示区域 (200x480)
        Mat probROI = display(Rect(600, 0, 200, 480));
        probDisplay.copyTo(probROI);
        
        // 添加分隔线
        line(display, Point(600, 0), Point(600, 480), Scalar(200, 200, 200), 2);
        
        // 显示帮助文本
        putText(display, "Draw digits here", Point(20, 30), 
               FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 0, 255), 1);
        putText(display, "Press ESC to exit", Point(20, 60), 
               FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 0, 255), 1);
        
        imshow("Digit Drawing & Recognition", display);
        waitKey(30);
    }
}

int main() {
    // 初始化画布
    canvas = Mat::zeros(480, 600, CV_8UC3);
    canvas.setTo(Scalar(255, 255, 255));
    
    // 初始化概率显示区域
    probDisplay = Mat::zeros(480, 200, CV_8UC3);
    probDisplay.setTo(Scalar(255, 255, 255));
    
    // 创建窗口
    namedWindow("Digit Drawing & Recognition");
    setMouseCallback("Digit Drawing & Recognition", onMouse);
    
    // 启动显示线程
    thread display(displayThread);
    
    // 主循环
    while (true) {
        int key = waitKey(30);
        if (key == 27) {  // ESC键退出
            running = false;
            break;
        }
    }
    
    // 清理
    display.join();
    destroyAllWindows();
    
    return 0;
}