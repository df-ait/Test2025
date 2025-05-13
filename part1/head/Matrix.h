#pragma once
#include <bits/stdc++.h>

class Matrix{
 private:
    int line;
    int column;
    std::vector<std::vector<float>>matrix;
    //判断矩阵a,b是否可以满足axb(内标相等才可相乘)
    bool is_multipy(std::vector<std::vector<float>>&a , std::vector<std::vector<float>>&b){
        int line_a = a.size() , line_b = b.size();
        int column_a = a[0].size() , column_b = b[0].size();
        if(column_a!=line_b) return false;
        if(a.empty()&&b.empty()) return false;
        return true;
    }
    //判断两矩阵是否可以相加
    bool is_add(std::vector<std::vector<float>>&a , std::vector<std::vector<float>>&b){
        int line_a = a.size() , line_b = b.size();
        int column_a = a[0].size() , column_b = b[0].size();
        if(line_a!=line_b||column_a!=column_b) return false;
        if(a.empty()&&b.empty()) return false;
        return true;
    }
 public:
    Matrix(std::vector<std::vector<float>>&a):matrix(a),line(a.size()),column(a[0].size()){}
    Matrix(int a , int b):line(a) , column(b){
        matrix.resize(a , std::vector<float>(b,0.0));
    }
    //矩阵相乘函数
    std::vector<std::vector<float>> matrix_multipy(std::vector<std::vector<float>>&a , std::vector<std::vector<float>>&b);
    //矩阵相加函数
    std::vector<std::vector<float>> matrix_add(std::vector<std::vector<float>>&a , std::vector<std::vector<float>>&b);
    //动态调整矩阵大小函数
    void reset_matrix(int a , int b){
        matrix.resize(a,std::vector<float>(b,0.0));
    }
};