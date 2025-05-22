#include <bits/stdc++.h>
#include "head/Matrix.h"
#include "head/Model.h"
#include "../json.hpp"

template<typename T>
void show(Matrix<T>& index){
      for(auto i = 0 ; i<index.line ; i++){
            for(auto j = 0 ; j<index.column; j++){
                std::cout<<index.matrix[i][j]<<" ";
            }
            std::cout<<"\n";
      }
      std::cout<<"\n\n";
   }

int main(){
    std::string folder;
    std::cout<<"Pls enter the folder path:";
    std::cin>>folder;

    nlohmann::json j;
    std::ifstream fin(folder+"/meta.json");
    j = nlohmann::json::parse(fin);

    std::cout<<"json path is"<<folder+"/meta.json\n\n";
    std::shared_ptr<Base_Model> only;
    if(j["type"] == "fp32"){
        std::cout<<"is float\n\n";
        only = std::make_shared<Model<float>>(folder , j);
    }
    else if(j["type"] == "fp64"){
        std::cout<<"is double\n\n";
        only = std::make_shared<Model<double>>(folder , j);
    }
    // else if(j["type"] == "int8"){
    //     only = std::make_shared<Model<int>>(folder , j);
    // }
    // else if(j["type"] == "int32_t"||j["type"] == "int64_t"){
    //     only = std::make_shared<Model<long>>(folder , j);
    // }
    else{
        std::cout<<"Has no match type\n\n";
    }

    /*传入一个矩阵*/
}