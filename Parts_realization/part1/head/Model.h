#pragma once
#include <bits/stdc++.h>
#include "Matrix.h"

class Base_Model{
   virtual ~Base_Model(){};
   std::shared_ptr<Base_Model> scan_files(const std::string& filename);
   template<class T>
   std::shared_ptr<T> create(Matrix<T>w1 , Matrix<T>w2 , Matrix<T>b1 , Matrix<T>b2){
      return std::make_shared<Model<T>>(w1 , b1 , w2 , b2);
   } 
};

template<class T>
class Model : public Base_Model{
 private:
    Matrix<T> weight_1;
    Matrix<T> bias_1;
    Matrix<T> weight_2;
    Matrix<T> bias_2;
 public:
    Model(){};
    Model(Matrix<T> weight_1_ , Matrix<T> bias_1_ , Matrix<T> weight_2_ , Matrix<T> bias_2_):weight_1(weight_1_) , bias_1(bias_1_) , weight_2(weight_2_) , bias_2(bias_2_){}
    auto forward(Matrix<T>& introduc_M);
    Matrix<T> Relu(Matrix<T>& A);
    Matrix<T> Softmax(Matrix<T>& A);
};