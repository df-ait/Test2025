#pragma once
#include <bits/stdc++.h>
#include "Matrix.h"
#include "../../../json.hpp"

class Base_Model{
 public:
   virtual ~Base_Model(){};
   //forward会返回长为10的向量
   virtual Matrix<float> forward(Matrix<float>& index) = 0;
   virtual Matrix<double> forward(Matrix<double>& index) = 0;
   // virtual Matrix<int> forward(Matrix<int>& index) = 0;
   // virtual Matrix<long> forward(Matrix<long>& index) = 0;
   //virtual std::shared_ptr<Base_Model> forward(std::shared_ptr<Base_Model> index) = 0;
};

template<class T>
class Model : public Base_Model{
 private:
    Matrix<T> weight_1;
    Matrix<T> bias_1;
    Matrix<T> weight_2;
    Matrix<T> bias_2;
 public:
    Model() = default;
    Model(Matrix<T> weight_1_ , Matrix<T> bias_1_ , Matrix<T> weight_2_ , Matrix<T> bias_2_):weight_1(weight_1_) , bias_1(bias_1_) , weight_2(weight_2_) , bias_2(bias_2_){}
    //传入文件名，构造model<T>
    Model(const std::string& folder ,const nlohmann::json& j){
      std::string w1_ = folder +"/fc1.weight";
      std::string b1_ = folder +"/fc1.bias";
      std::string w2_ = folder +"/fc2.weight";
      std::string b2_ = folder +"/fc2.bias";

      weight_1 = read_files(w1_ , j["fc1.weight"][0] , j["fc1.weight"][1]);
      bias_1 = read_files(b1_ , j["fc1.bias"][0] , j["fc1.bias"][1]);
      weight_2 = read_files(w2_ , j["fc2.weight"][0] , j["fc2.weight"][1]);
      bias_2 = read_files(b2_ , j["fc2.bias"][0] , j["fc2.bias"][1]);
      std::cout<<"w1:\n";
      show(weight_1);
      std::cout<<"b1:\n";
      show(bias_1);
      std::cout<<"w2:\n";
      show(weight_2);
      std::cout<<"b2:\n";
      show(bias_2);
    }
    //辅助读取矩阵
    Matrix<T> read_files(const std::string& filename , int line ,int column){
      std::ifstream file(filename , std::ios::in | std::ios::binary);
      if(!file){
         std::cout<<"File--"<<filename<<" failed to open\n\n";
      }
      Matrix<T> res(line , column);
      file.read(reinterpret_cast<char*>(res.matrix.data()), line*column*sizeof(T));

      return res;
    }

    Matrix<float> forward(Matrix<float>& index)override {
      //先将矩阵和weight_1做矩阵乘法，得到长宽为1 * 500的临时矩阵
      Matrix<float> index_1 = index*(Matrix<float>)weight_1;
      //再和bias1做加法（逐元素相加），得到长宽为1 * 500的临时矩阵
      Matrix<float> index_2 = (Matrix<float>)bias_1+index_1;
      //relu函数，得到长宽为1 * 500的临时矩阵
      Matrix<float> index_3 = Relu(index_2);
      //再和weight2做矩阵乘法，得到1 * 10的临时矩阵
      Matrix<float> index_4 = index_3*(Matrix<float>)weight_2;
      //再和bias2做矩阵加法，得到1 * 10的矩阵
      Matrix<float> index_5 = (Matrix<float>)bias_2+index_4;
      //最后经过softmax函数，返回长为10的向量
      Matrix<float> res = Softmax(index_5);
      return res;
   }

   Matrix<double> forward(Matrix<double>& index)override {
      Matrix<double> index_1 = index*(Matrix<double>)weight_1;
      Matrix<double> index_2 = (Matrix<double>)bias_1+index_1;
      Matrix<double> index_3 = Relu(index_2);
      Matrix<double> index_4 = index_3*(Matrix<double>)weight_2;
      Matrix<double> index_5 = (Matrix<double>)bias_2+index_4;
      Matrix<double> res = Softmax(index_5);
      return res;
   }

   void show(Matrix<T>& index){
      for(auto i = 0 ; i<index.line ; i++){
            for(auto j = 0 ; j<index.column; j++){
                std::cout<<index.matrix[i][j]<<" ";
            }
            std::cout<<"\n";
      }
      std::cout<<"\n\n";
   }

    Matrix<T> Softmax(Matrix<T>& A);
    Matrix<T> Relu(Matrix<T>& A);
};