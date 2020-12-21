#include "CImg.h"
#include "octree.h"
#include "dirent.h"
#include <string>
#include <tuple>
#include <vector>
#include <algorithm>
#include <iostream>

string lista[40];
std::vector<std::vector<std::vector<std::tuple<int,int,int>>>> cubo = {};

using namespace std;
using namespace cimg_library;

void listdir(string initStrPath){
    struct dirent *dir;
    DIR *dp = opendir(initStrPath.c_str());
    if(dp){
        while((dir=readdir(dp)) != NULL){
            string strPath = "";
            if ( strcmp(dir -> d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0 )continue;
            if ( dir->d_type == DT_DIR){
                strPath = initStrPath + "/" + dir->d_name;
                listdir(strPath);
            }else{
                strPath = initStrPath + "/" + dir->d_name;
                string temp = strPath;
                size_t index = strPath.find('I');
                size_t index_2 = strPath.find('.');
                string number = strPath.substr(index+1, index+1-index_2);
                lista[stoi(number)] = strPath;
            }
        }
        closedir(dp);
    }
}

int main() {
    vector<vector<tuple<int,int,int>>> val;
    listdir("D:\\Universidad\\Ciclo 6\\EDA\\Proyecto Final\\Octree\\Octree\\datos cerebros\\paciente 1\\1");
    cubo.resize(40, val);

    for(auto c : lista){
        cout << c << endl;
    }
    int sizeInBytesCube = 0;
    int counter = 0;
    for (int i = 0; i < 40; i++){
        cubo[i].resize(512);
        CImg <float> img(lista[i].c_str());
        for(int j= 0; j< 512 ; j++ ){
            cubo[i][j].resize(512);
            for(int k = 0; k < 512; k++){
                std::get<0>(cubo[i][j][k]) = img(j, k, 0);
                std::get<1>(cubo[i][j][k]) = img(j, k, 1);
                std::get<2>(cubo[i][j][k]) = img(j, k, 2);
                sizeInBytesCube += sizeof(cubo[i][j][k]);
            }
        }
    }

    Octree octree;

    octree.insert(cubo);

    octree.split_costado(0, 0, 511, 20);
    //octree.split_costado(0, 0, 511, 39);
    //octree.split_costado(14, 10, 342, 11);
    //octree.split_costado(500, 10, 100, 6);
    //octree.split_costado(311, 7, 51, 4);
    //octree.split_costado(1, 20, 400, 19);

    get_Corte(cubo, 0, 0, 511, 20);
    //get_Corte(cubo, 0, 0, 511, 39);
    //get_Corte(cubo, 14, 10, 342, 11);
    //get_Corte(cubo, 500, 10, 100, 6);
    //get_Corte(cubo, 311, 7, 51, 4);
    //get_Corte(cubo, 1, 20, 400, 19);

    octree.split_frente(0, 40, 511, 0);

    cout << "Tamaño en Bytes del cubo = " << sizeInBytesCube << " bytes" << endl;
    cout << "Tamaño en Bytes del octree = " << octree.getsizeInBytes() << " bytes" << endl;

    return 0;
}
