#pragma once
#include <bits/stdc++.h>
#include "Matrix.h"

template<class T>
class Model{
 private:
    Matrix<T> weight_1;
    Matrix<T> bias_1;
    Matrix<T> weight_2;
    Matrix<T> bias_2;
 public:
    Model(Matrix<T> weight_1_ , Matrix<T> bias_1_ , Matrix<T> weight_2_ , Matrix<T> bias_2_):weight_1(weight_1_) , bias_1(bias_1_) , weight_2(weight_2_) , bias_2(bias_2_){}
    auto forward(Matrix<T>& introduc_M);
    Matrix<T> Relu(Matrix<T>& A);
    Matrix<T> Softmax(Matrix<T>& A);
};