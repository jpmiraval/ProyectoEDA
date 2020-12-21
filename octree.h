#include <vector>
#include <iostream>
#include <tuple>
#include "CImg.h"
using namespace std;

void get_Corte(vector<vector<vector<tuple<int, int, int>>>> cubo, int x1,int z1,int x2,int z2){

    double m;
    if(x2 == x1)m=0;
    else
        m = ( (double)z2 - (double)z1) / ((double)x2 - (double)x1);

    clock_t tStart, tEnd;
    double timeTakenInget_Corte;

    cimg_library::CImg<float> img2(511, 511, 1, 3);
    double c = z1 -(m * x1);

    tStart = clock();
    for(int i = 0; i < cubo.size(); i++){
        for(int j = 0; j < cubo[i].size(); j++){
            for(int k = 0; k < cubo[i][j].size(); k++){
                if(i == (int)(m*j+c)){
                        img2(j, k, 0) = get<0>(cubo[i][j][k]);
                        img2(j, k, 1) = get<1>(cubo[i][j][k]);
                        img2(j, k, 2) = get<2>(cubo[i][j][k]);
                }
            }
        }
    }
    tEnd = clock();

    timeTakenInget_Corte = double(tEnd - tStart) / CLOCKS_PER_SEC;
    cout << "Tiempo tomado por Get_Corte = " << timeTakenInget_Corte << " segundos" << endl;

    img2.display();
}


class Octree {

private:

    int sizeInBytes = 0;
    vector<vector<vector<tuple<int,int,int>>>> m;

    struct Node {

        int x0, xf, y0, yf, z0, zf;
        Node *children[8];
        tuple<int,int,int> color;
        bool is_terminal = false;

        Node(int x0, int xf, int y0, int yf, int z0, int zf) : x0{x0}, xf{xf}, y0{y0}, yf{yf}, z0{z0}, zf{zf} {

            for (int i = 0; i < 8; i++) {
                children[i] = nullptr;
            }

        }

    };

    Node *root;

public:

    int getsizeInBytes() { return sizeInBytes; }

    bool SameColor(Node *&n) {
        int umbral = 25;
        int r = 0, b = 0, g = 0;
        for (int k = n->z0; k < n->zf; k++) {
            for (int i = n->x0; i < n->xf; i++) {
                for (int j = n->y0; j < n->yf; j++) {
                    r += get<0>(m[k][i][j]);
                    g += get<1>(m[k][i][j]);
                    b += get<2>(m[k][i][j]);
                }
            }
        }


        for (int k = n->z0; k < n->zf; k++) {
            for (int i = n->x0; i < n->xf; i++) {
                for (int j = n->y0; j < n->yf; j++) {
                    if (abs(get<0>(m[k][i][j]) - r) > umbral || abs(get<1>(m[k][i][j]) - g) > umbral ||
                        abs(get<2>(m[k][i][j]) - b) > umbral)
                        return false;
                }
            }
        }
        return true;
    }


    void insert(Node *&n) {
        if (SameColor(n)) {
            get<0>(n->color) = get<0>(m[n->z0][n->x0][n->y0]);
            get<1>(n->color) = get<1>(m[n->z0][n->x0][n->y0]);
            get<2>(n->color) = get<2>(m[n->z0][n->x0][n->y0]);
            n->is_terminal = true;

            sizeInBytes += sizeof(this);
            return;
        }
        int midX = (n->xf + n->x0) / 2;
        int midY = (n->yf + n->y0) / 2;
        int midZ = (n->zf + n->z0) / 2;
        
        n->children[0] = new Node(n->x0, midX, n->y0, midY, n->z0, midZ); insert(n->children[0]);
        n->children[1] = new Node(midX, n->xf, n->y0, midY, n->z0, midZ); insert(n->children[1]);
        n->children[2] = new Node(n->x0, midX, midY, n->yf, n->z0, midZ); insert(n->children[2]);
        n->children[3] = new Node(midX, n->xf, midY, n->yf, n->z0, midZ); insert(n->children[3]);
        n->children[4] = new Node(n->x0, midX, n->y0, midY, midZ, n->zf); insert(n->children[4]);
        n->children[5] = new Node(midX, n->xf, n->y0, midY, midZ, n->zf); insert(n->children[5]);
        n->children[6] = new Node(n->x0, midX, midY, n->yf, midZ, n->zf); insert(n->children[6]);
        n->children[7] = new Node(midX, n->xf, midY, n->yf, midZ, n->zf); insert(n->children[7]);

        sizeInBytes += sizeof(this);
    }

    void insert(vector<vector<vector<tuple<int, int, int>>>> &m) {
        this->m = m;
        root = new Node(0, m[0].size() - 1, 0, m[0][0].size() - 1, 0, m.size() - 1);
        insert(root);
    }

    struct punto {
        int x, y, z;
        tuple<int,int,int> color;

        punto(int x, int y) : x(x), y(y) {};

        punto(int x, int y, int z, tuple<int, int, int> color) : x(x), y(y), z(z), color(color) {};

        void colored(tuple<int,int,int> color) { this->color = color; };
    };

    void split_frente(int y1, int z1, int y2, int z2) {
        double m;
        if(y2 == y1)m=0;
        else
            m = ( (double)z2 - (double)z1) / ((double)y2 - (double)y1);

        double c = z1 -(m * y1);
        vector<punto *> puntos;
        llenar_puntos_frente(root, puntos, m, c, y1, y2, z1, z2);

        cimg_library::CImg<float> img(511, 40, 1, 3);
        for (auto c : puntos) {
            img(c->x, c->z, 0) = get<0>(c->color);
            img(c->x, c->z, 1) = get<1>(c->color);
            img(c->x, c->z, 2) = get<2>(c->color);
        }
        img.display();
    }

    void llenar_puntos_frente(Node *node, vector<punto *>& puntos, double m, float c, int y1, int z1, int y2, int z2) {
        if (node->is_terminal) {
            for (int i = node->y0; i < node->yf; i++) {
                int y_temp = i;
                int z_temp = m * i + c;
                if (z_temp >= node->z0 && z_temp <= node->zf) {
                    for (int j = node->x0; j < node->xf; j++){
                        puntos.push_back(new punto(j, y_temp, z_temp, node->color));
                    }

                }
            }
            return;
        }

        for (int i = 0; i < 8; i++) {
            if(m >= 0){
                if (node->children[i]->y0 * m + c > node->zf || node->children[i]->yf * m + c < node->z0) {
                    continue;
                } else {
                    llenar_puntos_frente(node->children[i], puntos, m, c, y1, z1, y2, z2);
                }
            }
            else{
                if (node->children[i]->y0 * m + c < node->z0 || node->children[i]->yf * m + c > node->zf) {
                    continue;
                } else {
                    llenar_puntos_frente(node->children[i], puntos, m, c, y1, z1, y2, z2);
                }
            }
        }
        return;
    }


    void split_costado(int x1, int z1, int x2, int z2) {
        double m;
        if(x2 == x1)m=0;
        else
        m = ( (double)z2 - (double)z1) / ((double)x2 - (double)x1);

        double c = z1 -(m * x1);
        vector<punto *> puntos;

        clock_t tStart, tEnd;
        double tiempoTomado;
        tStart = clock();
        llenar_puntos_costado(root, puntos, m, c, x1, z1, x2, z2);
        tEnd = clock();

        tiempoTomado = double(tEnd - tStart) / CLOCKS_PER_SEC;
        cout << "Tiempo tomado por Split Costado = " << tiempoTomado << " segundos" << endl;

        cimg_library::CImg<float> img(511, 511, 1, 3);
        for (auto c : puntos) {
            img(c->x, c->y, 0) = get<0>(c->color);
            img(c->x, c->y, 1) = get<1>(c->color);
            img(c->x, c->y, 2) = get<2>(c->color);
        }
        img.display();
    }


    void llenar_puntos_costado(Node *node, vector<punto *>& puntos, double m, float c, int x1, int z1, int x2, int z2) {
        if (node->is_terminal) {
            for (int i = node->x0; i < node->xf; i++) {
                int x_temp = i;
                int z_temp = m * i + c;
                if (z_temp >= node->z0 && z_temp <= node->zf) {
                    for (int j = node->y0; j < node->yf; j++){
                        puntos.push_back(new punto(x_temp, j, z_temp, node->color));
                    }

                }
            }
            return;
        }

        for (int i = 0; i < 8; i++) {
            if(m >= 0){
                if (node->children[i]->x0 * m + c > node->zf || node->children[i]->xf * m + c < node->z0) {
                    continue;
                } else {
                    llenar_puntos_costado(node->children[i], puntos, m, c, x1, z1, x2, z2);
                }
            }
            else{
                if (node->children[i]->x0 * m + c < node->z0 || node->children[i]->xf * m + c > node->zf) {
                    continue;
                } else {
                    llenar_puntos_costado(node->children[i], puntos, m, c, x1, z1, x2, z2);
                }
            }
        }
        return;
    }
};
