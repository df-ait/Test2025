#include "../head/Model.h"
#include "../head/Matrix.h"
#include <bits/stdc++.h>
#include "../../../json.hpp"

// template<class T>
// std::shared_ptr<Base_Model> Model<T>::forward(std::shared_ptr<Base_Model>& index)
// {
//     Matrix<T>* introduc_M = dynamic_cast<Matrix<T>*>(index.get());
//     if(index == nullptr){
//         std::cerr<<"基类指针转换异常\n\n";
//     }
//     //先将矩阵和weight_1做矩阵乘法，得到长宽为1 * 500的临时矩阵
//     Matrix<T> index_1 = introduc_M->matrix_multipy(weight_1);
//     //再和bias1做加法（逐元素相加），得到长宽为1 * 500的临时矩阵
//     Matrix<T> index_2 = bias_1.matrix_add(index_1);
//     //relu函数，得到长宽为1 * 500的临时矩阵
//     Matrix<T> index_3 = Relu(index_2);
//     //再和weight2做矩阵乘法，得到1 * 10的临时矩阵
//     Matrix<T> index_4 = index_3.matrix_multipy(weight_2);
//     //再和bias2做矩阵加法，得到1 * 10的矩阵
//     Matrix<T> index_5 = bias_2.matrix_add(index_4);
//     //最后经过softmax函数，返回长为10的向量
//     Matrix<T> res = Softmax(index_5);
//     return std::make_shared<Base_Model>(res);
// }

// template<typename T>
// std::shared_ptr<Base_Model> Model<T>::forward(std::shared_ptr<Base_Model>& index){
//       Matrix<T>* introduc_M = dynamic_cast<Matrix<T>*>(index.get());
//       if(index == nullptr){
//          std::cerr<<"基类指针转换异常\n\n";
//       }
//       //先将矩阵和weight_1做矩阵乘法，得到长宽为1 * 500的临时矩阵
//       Matrix<T> index_1 = introduc_M->matrix_multipy(weight_1);
//       //再和bias1做加法（逐元素相加），得到长宽为1 * 500的临时矩阵
//       Matrix<T> index_2 = bias_1.matrix_add(index_1);
//       //relu函数，得到长宽为1 * 500的临时矩阵
//       Matrix<T> index_3 = Relu(index_2);
//       //再和weight2做矩阵乘法，得到1 * 10的临时矩阵
//       Matrix<T> index_4 = index_3.matrix_multipy(weight_2);
//       //再和bias2做矩阵加法，得到1 * 10的矩阵
//       Matrix<T> index_5 = bias_2.matrix_add(index_4);
//       //最后经过softmax函数，返回长为10的向量
//       Matrix<T> res = Softmax(index_5);
//       return std::make_shared<Model<T>>(res);
//    }

template<typename T>
Matrix<T> Model<T>::Softmax(Matrix<T>& A){
    double E;
    //先计算出分母，
    for(auto i = 0 ; i< A.line ; i++){
        for(auto j = 0 ; j< A.column ; j++){
            E +=exp(A.matrix[i][j]);
        }
    }

    for(auto i = 0 ; i< A.line ; i++){
        for(auto j = 0 ; j< A.column ; j++){
            double calcul = exp(A.matrix[i][j]) / E;
            A.matrix[i][j] = calcul;
        }
    }
    return A;
   }

template<typename T>
Matrix<T> Model<T>::Relu(Matrix<T>& A){
    for(auto i = 0 ; i< A.line ; i++){
        for(auto j = 0 ; j< A.column ; j++){
            A[i][j] = A[i][j]>0?A[i][j]:0;
        }
    }
    return A;
   }