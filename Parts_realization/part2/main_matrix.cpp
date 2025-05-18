#include <bits/stdc++.h>
#include "../part1/head/Matrix.h"
#include "../part1/head/Model.h"
#include "json.hpp"

template<class T>
Matrix<T> read_files(const std::string& filename , int line , int column){
    std::ifstream file(filename , std::ios::binary | std::ios::in);
    if(!file.is_open()){
        std::cerr<<"Failed to open the* "<<filename<<" *file\n\n";
        return Matrix<T>(0,0);
    }
    Matrix<T> index(line , column);
    while(!file.eof()){
        for(auto i = 0 ; i<line ; i++){
            for(auto j = 0 ; j<column ; j++){
                file.read((char*)&index.matrix[i][j] , sizeof(T));
            }
        }
    }
    file.close();
    return index;
}

template<class T>
std::shared_ptr<Base_Model> read_folder(const std::string& folder , nlohmann::json& j){
    std::string w1 = folder +"/fc1.weight";
    std::string b1 = folder +"/fc1.bias";
    std::string w2 = folder +"/fc2.weight";
    std::string b2 = folder +"/f2c.bias";
    std::string json = folder + "/meta.json";

    Matrix<T> only; 

    Matrix<T> w1 = read_matrix<T>(w1, j["fc1.weight"][0] , j["fc1.weight"][1]);
    std::cout<<"w1:\n";
    test_push(w1);

    Matrix<T> b1 = read_matrix<T>(b1 , j["fc1.bias"][0] , j["fc1.bias"][1]);
    std::cout<<"b1:\n";
    test_push(b1);

    Matrix<T> w2 = read_matrix<T>(w2 , j["fc2.weight"][0] , j["fc2.weight"][1]);
    std::cout<<"w2:\n";
    test_push(w2);
    
    Matrix<T> b2 = read_matrix<T>(b2 , j["fc2.bias"][0] , j["fc2.bias"][1]);
    std::cout<<"b2:\n";
    test_push(b2);

    Model only(w1 , b1 , w2 , b2);
    return std::make_shared<Base_Model>Matrix(only);
}

template<class T>
void test_push(Matrix<T>& index){
    for(auto i = 0 ; i<index.line ; i++){
            for(auto j = 0 ; j<index.column; j++){
                std::cout<<index.matrix[i][j]<<" ";
            }
            std::cout<<"\n";
    }
    std::cout<<"\n\n";
}

int main(){
    std::shared_ptr<Base_Model>first;
    std::string folder;
    first = std::make_shared<Base_Model>();
    std::cout<<"Pls enter the folder path:";
    std::cin>>folder;

    nlohmann::json j;
    std::ifstream fin(folder + "/meta.json");
    j = nlohmann::json::parse(fin);
    std::shared_ptr<Base_Model> only;
    if(j["type"] == "fp64"){
        only = read_folder<double>(folder , j);
    }
    else if(j["type"] == "int32_t"){
        only = read_folder<int>(folder , j);
    }
    else if(j["type"] == "int64_t"){
        only = read_folder<long>(folder , j);
    }
    else if(j["type"] == "fp32"){
        only = read_folder<float>(folder , j);
    }
    else{
        std::cout<<"!Type was not detected\n\n";
    }
    
    return 0;
}