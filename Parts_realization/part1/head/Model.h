#pragma once
#include <bits/stdc++.h>
#include "Matrix.h"
class Model{
 private:
    Matrix weight_1;
    Matrix bias_1;
    Matrix weight_2;
    Matrix bias_2;
 public:
    Model(Matrix weight_1_ , Matrix bias_1_ , Matrix weight_2_ , Matrix bias_2_):weight_1(weight_1_) , bias_1(bias_1_) , weight_2(weight_2_) , bias_2(bias_2_){}
    auto forward(Matrix& introduc_M);
    Matrix Relu(Matrix& A);
    Matrix Softmax(Matrix& A);
};