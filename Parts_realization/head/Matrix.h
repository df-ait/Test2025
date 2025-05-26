#pragma once
#include <bits/stdc++.h>

template<class T>
class Matrix{
 private:
    //判断矩阵a,b是否可以满足*this x other(内标相等才可相乘)
    bool is_multipy(const Matrix& other)const{
        int line_other = other.line , line_this = this->line;
        int column_other = other.column , column_this = this->column;
        if(column_this!=line_other) return false;
        if(other.matrix.empty()&&this->matrix.empty()) return false;
        return true;
    }
    //判断两矩阵是否可以相加
    bool is_add(const Matrix& other)const{
        int line_other = other.line , line_this = this->line;
        int column_other = other.column , column_this = this->column;
        if(line_other!=line_this||column_other!=column_this) return false;
        if(other.matrix.empty()&&this->matrix.empty()) return false;
        return true;
    }

 public:
    int line;
    int column;
    std::vector<std::vector<T>>matrix;
    Matrix() = default;
    Matrix(Matrix<T>& a):matrix(a.matrix) , line(a.line) , column(a.column){}
    Matrix(std::vector<std::vector<T>>&a):matrix(a),line(a.size()),column(a[0].size()){}
    Matrix(int a , int b):line(a) , column(b){
        matrix.resize(a , std::vector<T>(b,0));
    }
    //类型转换函数
    template<typename U>
    Matrix(const Matrix<U>& other) : line(other.line), column(other.column) {
        matrix.resize(line, std::vector<T>(column));
        for (int i = 0; i < line; ++i) {
            for (int j = 0; j < column; ++j) {
                matrix[i][j] = static_cast<T>(other.matrix[i][j]);
            }
        }
    }

    //矩阵相乘函数
    Matrix<T> Multipy(const Matrix<T>& other)const
    {
        unsigned int threads = std::thread::hardware_concurrency();
        // std::cout<<"CPU核心数为:"<<threads<<"\n";
        if(!is_multipy(other)) return *this;//两矩阵无法相乘的情况
        int line_this = this->line;
        int common = other.line;//或者是this->column
        int column_other = other.column; 
        //向上取整好一点，避免漏掉
        int thread_num = (column_other + threads -1) / threads;//计算每个线程应该分到多少行矩阵来计算
        // std::cout<<"每个线程要处理的列数为:"<<thread_num<<"\n";
        // std::cout<<"矩阵总列数为:"<<column_other<<"\n";
        //两矩阵外标为相乘后结果矩阵的行列,每次计算，this都只有一行
        std::vector<std::thread>work;
        std::vector<std::vector<T>> res(line_this , std::vector<T>(column_other,0));
        for(int i = 0 ; i< threads ; i++){
            int start = i*thread_num;
            int end = std::min(start+thread_num , column_other);
            if(start>=end) break;
            // std::cout<<"time:"<<i<<"start:"<<start<<"end:"<<end<<"\n\n";
            work.emplace_back([&res , line_this , common , &other , this , start , end](){
                for(auto m = 0; m < line_this ; ++m){//控制a的行数
                    for(auto j = start;j < end ;++j){//控制b的列数
                        T sum = 0;
                        for(auto k = 0 ; k< common ; ++k){//控制a的列数/b的行数
                            sum += this->matrix[m][k] * other.matrix[k][j];
                        }
                        res[m][j] = sum;
                    }
                }
            });
        }
        for(auto &th :work){
            th.join();
        }
        //多线程之前为6ms,多线程之后为1ms
        // for(auto i = 0; i < line_this ; i++){//控制a的行数
        //     for(auto j = 0;j < column_other ;j++){//控制b的列数
        //         for(auto k = 0 ; k< common ; k++){//控制a的列数/b的行数
        //             res[i][j] += this->matrix[i][k] * other.matrix[k][j];
        //         }
        //     }
        // }
        return Matrix(res);
    }   

    // //矩阵相加函数
    Matrix<T> add(const Matrix<T>& other)const
    {
        unsigned int threads = std::thread::hardware_concurrency();
        if(!is_add(other)) return other;
        int line = other.line;
        int column = other.column;
        int thread_num = (column + threads -1)/threads;
        // std::cout<<"每个线程要处理的列数为:"<<column<<"\n";
        // std::cout<<"矩阵总列数为:"<<line<<"\n";
        std::vector<std::vector<T>> res(line , std::vector<T>(column,0));
        for(auto i = 0 ; i < line ; i++){
            for(auto j = 0 ; j < column ; j++){
                res[i][j] = this->matrix[i][j] + other.matrix[i][j];
            }
        }
        return Matrix(res);
    }
};