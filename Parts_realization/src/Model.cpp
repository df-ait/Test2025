#include "../head/Model.h"
#include "../head/Matrix.h"
#include <bits/stdc++.h>
#include "../../../json.hpp"

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
            A.matrix[i][j] = A.matrix[i][j]>0?A.matrix[i][j]:0;
        }
    }
    return A;
}

template<class T>
void Model<T>::show(Matrix<T>& index){
      for(auto i = 0 ; i<index.line ; i++){
            for(auto j = 0 ; j<index.column; j++){
                std::cout<<index.matrix[i][j]<<" ";
            }
            std::cout<<"\n";
      }
      std::cout<<"\n\n";
   }
