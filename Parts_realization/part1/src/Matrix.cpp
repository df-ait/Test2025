#include <bits/stdc++.h>
#include "../head/Matrix.h"

template<class T>
Matrix<T> Matrix<T>::matrix_multipy(Matrix<T>& other)
{
    if(!is_multipy(other)) return other;//两矩阵无法相乘的情况
    int line_this = this->line;
    int common = other.line;//或者是this->column
    int column_other = other.column; 

    //两矩阵外标为相乘后结果矩阵的行列
    std::vector<std::vector<float>> res(line_this , std::vector<float>(column_other,0.0));

    for(auto i = 0; i < line_this ; i++){//控制a的行数
        for(auto j = 0;j < column_other ;j++){//控制b的列数
            for(auto k = 0 ; k< common ; k++){//控制a的列数/b的行数
                res[i][j] = this->matrix[i][k] * other.matrix[k][j];
            }
        }
    }
    return Matrix(res);
}

template<class T>
Matrix<T> Matrix<T>::matrix_add(Matrix<T>& other)
{
    if(!is_add(other)) return other;
    int line = other.line;
    int column = other.column;
    std::vector<std::vector<float>> res(line , std::vector<float>(column,0.0));

    for(auto i = 0 ; i < line ; i++){
        for(auto j = 0 ; j < column ; j++){
            res[i][j] = this->matrix[i][j] + other.matrix[i][j];
        }
    }
    return Matrix(res);
}