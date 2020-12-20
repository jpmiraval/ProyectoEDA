#include "CImg.h"
#include "octree.h"
#include <dirent.h>
#include <string>
#include <vector>
#include <iostream>

std::string lista[41] = {};
std::vector<std::vector<std::vector<int>>> cubo = {};

using namespace std;
using namespace cimg_library;



void listdir(string initStrPath){
    struct dirent *dir;
    int counter = 0;
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
                lista[counter] = strPath;
                counter+=1;
            }
        }
        closedir(dp);
    }
}


CImg<int> binarize(CImg<float> &img, int umbral) {
    CImg <int> R(img.width(),img.height());
    for(int i=0;i< img.width();i++) {
        for (int j = 0; j < img.height(); j++) {
            int r = img(i, j, 0);
            int g = img(i, j, 1);
            int b = img(i, j, 2);
            if ((r + g + b)*10 > umbral) {
                R(i, j) = 1;
            } else {
                R(i, j) = 0;
            }
        }
    }
    return R;
}
/*
int QuadTree::getBasicColor(int color){
   if(color >= 190){
       return 255;
   }
   else if(color < 190 && color >= 63){
       return 125;
   }
   else {
       return 0;
   }
}



CImg<int> QuadTree::binarizeColors(CImg<float>& img){
   CImg<int> imgResult(img.width(),img.height(), 1, 3, 0);
   cimg_forXY(img, x, y){
           int R = (int)img(x, y, 0, 0);
           int G = (int)img(x, y, 0, 1);
           int B = (int)img(x, y, 0, 2);
           int rnew = getBasicColor(R);
           int gnew = getBasicColor(G);
           int bnew = getBasicColor(B);
           imgResult(x, y, 0, 0) = rnew;
           imgResult(x, y, 0, 1) = gnew;
           imgResult(x, y, 0, 2) = bnew;
       }
   return imgResult;
}*/


int main() {
    vector<vector<int>> val;
    listdir("/home/jpalexander1706/CLionProjects/octree/datoscerebros/paciente1/1");
    cubo.resize(41,val);
    /*
    for(auto c : lista){
        cout << c << endl;
    }*/
    int counter = 0;
    for (int i = 0; i < 41; i++){
        cubo[i].resize(512);
        CImg <float> img(lista[i].c_str());
        CImg <int> img_F = binarize(img, 3825);
        for(int j= 0; j< 512 ; j++ ){
            cubo[i][j].resize(512);
            for(int k = 0; k < 512; k++){
                cubo[i][j][k] = img_F(j,k);
            }
        }
    }

    Octree octree;

    octree.insert(cubo);
    octree.split(100, 200, 300, 400);

    return 0;
}
