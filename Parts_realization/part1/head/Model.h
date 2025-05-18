#pragma once
#include <bits/stdc++.h>
#include "Matrix.h"

class Base_Model{
 public:
   virtual ~Base_Model(){};
   
   // template<class T>
   // std::shared_ptr<Base_Model> create(Matrix<T>w1 , Matrix<T>w2 , Matrix<T>b1 , Matrix<T>b2){
   //    return std::make_shared<Base_Model>(w1 , b1 , w2 , b2);
   // }
   
   //virtual std::shared_ptr<Matrix<float>>forward(std::shared_ptr<Matrix<float>>& index) = 0;  
   virtual std::shared_ptr<Matrix_Base>forward(std::shared_ptr<Base_Model>& index) = 0;  
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
    //auto forward(Matrix<T>& introduc_M);
    Matrix<Matrix_Base> forward(std::shared_ptr<Matrix<T>>& index) {
      // Matrix<T>* introduc_M = dynamic_cast<Matrix<T>*>(index.get());
      // if(index == nullptr){
      //    std::cerr<<"基类指针转换异常\n\n";
      // }
      //先将矩阵和weight_1做矩阵乘法，得到长宽为1 * 500的临时矩阵
      Matrix<T> index_1 = index->matrix_multipy(weight_1);
      //再和bias1做加法（逐元素相加），得到长宽为1 * 500的临时矩阵
      Matrix<T> index_2 = bias_1.matrix_add(index_1);
      //relu函数，得到长宽为1 * 500的临时矩阵
      Matrix<T> index_3 = Relu(index_2);
      //再和weight2做矩阵乘法，得到1 * 10的临时矩阵
      Matrix<T> index_4 = index_3.matrix_multipy(weight_2);
      //再和bias2做矩阵加法，得到1 * 10的矩阵
      Matrix<T> index_5 = bias_2.matrix_add(index_4);
      //最后经过softmax函数，返回长为10的向量
      Matrix<T> res = Softmax(index_5);
      auto result = std::make_shared<Matrix_Base>(res);
      return result;
   }


    Matrix<T> Softmax(Matrix<T>& A);
    
    Matrix<T> Relu(Matrix<T>& A);
};