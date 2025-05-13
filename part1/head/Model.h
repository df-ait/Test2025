#pragma once
#include <bits/stdc++.h>
#include "Matrix.h"
class Model{
 private:
    Matrix weight_1 = Matrix(784,500);
    Matrix bias_1 = Matrix(1,500);
    Matrix weight_2 = Matrix(500,10);
    Matrix bias_2 = Matrix(1,10);
 public:
    auto forward();
};