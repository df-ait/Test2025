#include <bits/stdc++.h>
#include "../head/Matrix.h"

std::vector<std::vector<float>> Matrix::matrix_multipy(std::vector<std::vector<float>> &a, std::vector<std::vector<float>> &b)
{
    if(!is_multipy(a,b)) return a;//两矩阵无法相乘的情况
    int line_a = a.size();
    int common = b.size();
    int column_b = b[0].size(); 

    //两矩阵外标为相乘后结果矩阵的行列
    std::vector<std::vector<float>> res(line_a , std::vector<float>(column_b,0.0));

    for(auto i = 0; i < line_a ; i++){//控制a的行数
        for(auto j = 0;j < column_b ;j++){//控制b的列数
            for(auto k = 0 ; k< common ; k++){//控制a的列数/b的行数
                res[i][j] = a[i][k] * b[k][j];
            }
        }
    }
     
    return res;
}
std::vector<std::vector<float>> Matrix::matrix_add(std::vector<std::vector<float>> &a, std::vector<std::vector<float>> &b)
{
    
    if(!is_add) return a;
    int line = a.size();
    int column = a[0].size();
    std::vector<std::vector<float>> res(line , std::vector<float>(column,0.0));

    for(auto i = 0 ; i < line ; i++){
        for(auto j = 0 ; j < column ; j++){
            res[i][j] = a[i][j] + b[i][j];
        }
    }

    return res;
}