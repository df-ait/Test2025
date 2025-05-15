#include <bits/stdc++.h>
#include "../part1/head/Matrix.h"
#include "../part1/head/Model.h"
#include "json.hpp"

Matrix read_matrix(const std::string& filename , int line , int column){
    std::ifstream file(filename , std::ios::binary | std::ios::in);
    if(!file.is_open()){
        std::cerr<<"Failed to open the* "<<filename<<" *file\n\n";
        return Matrix(0,0);
    }
    Matrix index(line , column);
    while(!file.eof()){
        for(auto i = 0 ; i<line ; i++){
            for(auto j = 0 ; j<column ; j++){
                file.read((char*)&index.matrix[i][j] , sizeof(float));
            }
        }
    }
    file.close();
    return index;
}

void test_push(Matrix& index){
    for(auto i = 0 ; i<index.line ; i++){
            for(auto j = 0 ; j<index.column; j++){
                std::cout<<index.matrix[i][j]<<" ";
            }
            std::cout<<"\n";
    }
    std::cout<<"\n\n";
}

int main(){
    nlohmann::json j;
    std::ifstream fin("D:/Test2025/mnist-fc/meta.json");
    j = nlohmann::json::parse(fin);
    
    Matrix w1 = read_matrix("D:/Test2025/mnist-fc/fc1.weight" , j["fc1.weight"][0] , j["fc1.weight"][1]);
    std::cout<<"w1:\n";
    test_push(w1);

    Matrix b1 = read_matrix("D:/Test2025/mnist-fc/fc1.bias" , j["fc1.bias"][0] , j["fc1.bias"][1]);
    std::cout<<"b1:\n";
    test_push(b1);

    Matrix w2 = read_matrix("D:/Test2025/mnist-fc/fc2.weight" , j["fc2.weight"][0] , j["fc2.weight"][1]);
    std::cout<<"w2:\n";
    test_push(w2);
    
    Matrix b2 = read_matrix("D:/Test2025/mnist-fc/fc2.bias" , j["fc2.bias"][0] , j["fc2.bias"][1]);
    std::cout<<"b2:\n";
    test_push(b2);

    Model only(w1 , b1 , w2 , b2);
    return 0;
}