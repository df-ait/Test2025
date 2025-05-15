#pragma once
#include <bits/stdc++.h>

class Matrix{
 private:
    
    //判断矩阵a,b是否可以满足*this x other(内标相等才可相乘)
    bool is_multipy(Matrix& other){
        int line_other = other.line , line_this = this->line;
        int column_other = other.column , column_this = this->column;
        if(column_this!=line_other) return false;
        if(other.matrix.empty()&&this->matrix.empty()) return false;
        return true;
    }
    //判断两矩阵是否可以相加
    bool is_add(Matrix& other){
        int line_other = other.line , line_this = this->line;
        int column_other = other.column , column_this = this->column;
        if(line_other!=line_this||column_other!=column_this) return false;
        if(other.matrix.empty()&&this->matrix.empty()) return false;
        return true;
    }
 public:
    int line;
    int column;
    std::vector<std::vector<float>>matrix;
    Matrix(std::vector<std::vector<float>>&a):matrix(a),line(a.size()),column(a[0].size()){}
    Matrix(int a , int b):line(a) , column(b){
        matrix.resize(a , std::vector<float>(b,0.0));
    }
    // //矩阵相乘函数
    Matrix matrix_multipy(Matrix& other);
    // //矩阵相加函数
    Matrix matrix_add(Matrix& other);
};