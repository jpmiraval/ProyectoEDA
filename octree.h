#include <vector>
#include <iostream>
#include "CImg.h"

using namespace std;

void get_Corte(vector<vector<vector<int>>> cubo, int x1,int z1,int x2,int z2){

    double m;
    if(x2 == x1)m=0;
    else
        m = ( (double)z2 - (double)z1) / ((double)x2 - (double)x1);
    printf("%f",m);
    //X = m . Z + c

    cimg_library::CImg<unsigned char> img2(512, 512);
    double c = z1 -(m * x1);
    for(int i = 0; i < cubo.size(); i++){
        for(int j = 0; j < cubo[i].size(); j++){
            for(int k = 0; k < cubo[i][j].size(); k++){
                //cout << i << " " << (int)m*j+c << endl;
                if(i == (int)(m*j+c)){
                        cout << "Hola";
                        //cout << c->x << " " << c->y << " " << c->z << " " << c->color << endl;
                        img2(j, k) = cubo[i][j][k];
                }
            }
        }
    }
    img2.display();
}


class Octree {

private:


    vector<vector<vector<int>>> m;

    struct Node {

        int x0, xf, y0, yf, z0, zf;
        Node *children[8];
        int color;
        bool is_terminal = false;

        Node(int x0, int xf, int y0, int yf, int z0, int zf) : x0{x0}, xf{xf}, y0{y0}, yf{yf}, z0{z0}, zf{zf} {

            for (int i = 0; i < 8; i++) {
                children[i] = nullptr;
            }

        }

    };

    Node *root;

public:

    /*
	bool SameColor(Node* &n, vector<vector<vector<int>>> m) {
		int color = m[n->zf][n->x0][n->y0];

		for (int k = n->z0; k < n->zf-1; k++) {
			for (int i = n->x0; i < n->xf-1; i++) {
				for (int j = n->y0; j < n->yf-1; j++) {
					if (m[k][i][j] != color) {
						return false;
					}
				}
			}
		}

		return true;
	}

	void insert(Node* &n, vector<vector<vector<int>>>& m) {
        std::cout << n->x0 << " " << n->xf << " " <<    n-> y0 <<" " << n->yf << " " << n->z0 << " " << n->zf << "\n";
		if (SameColor(n, m)) {
			n->color = m[n->zf][n->x0][n->y0];
			n->is_terminal = true;
			return;
		}
		int midX = (n->xf+n->x0)/2;
		int midY = (n->yf+n->y0)/2;
		int midZ = (n->zf+n->z0)/2;
        n->children[0] = new Node(n->x0, midX, n->y0, midY, n->z0, midZ);
        n->children[1] = new Node(midX, n->xf, n->y0, midY, n->z0, midZ);
        n->children[2] = new Node(n->x0, midX, midY, n->yf, n->z0, midZ);
        n->children[3] = new Node(midX, n->xf, midY, n->yf, n->z0, midZ);
        n->children[4] = new Node(n->x0, midX, n->y0, midY, midZ, n->zf);
        n->children[5] = new Node(midX, n->xf, n->y0, midY, midZ, n->zf);
        n->children[6] = new Node(n->x0, midX, midY, n->yf, midZ, n->zf);
        n->children[7] = new Node(midX, n->xf, midY, n->yf, midZ, n->zf);
		insert(n->children[0], m);
		insert(n->children[1], m);
		insert(n->children[2], m);
		insert(n->children[3], m);
		insert(n->children[4], m);
		insert(n->children[5], m);
		insert(n->children[6], m);
		insert(n->children[7], m);

	}

	void insert(vector<vector<vector<int>>>& m) {
        root = new Node(0, m[0].size()-1, 0, m[0][0].size()-1, 0, m.size()-1);
        insert(root, m);
	}*/

    bool SameColor(Node *&n) {
        int color = m[n->z0][n->x0][n->y0];
        for (int k = n->z0; k < n->zf; k++) {
            for (int i = n->x0; i < n->xf; i++) {
                for (int j = n->y0; j < n->yf; j++) {
                    if (m[k][i][j] != color) {
                        return false;
                    }
                }
            }
        }
        return true;
    }


    void insert(Node *&n) {
        if (SameColor(n)) {
            n->color = m[n->z0][n->x0][n->y0];
            n->is_terminal = true;
            return;
        }
        int midX = (n->xf + n->x0) / 2;
        int midY = (n->yf + n->y0) / 2;
        int midZ = (n->zf + n->z0) / 2;
        /*if (n->x0 != midX && n->y0 != midY && n->z0 != midZ) {*/ n->children[0] = new Node(n->x0, midX, n->y0, midY,
                                                                                             n->z0, midZ);
        insert(n->children[0]); /*}*/
        /*if (midX != n->xf && n->y0 != midY && n->z0 != midZ) {*/ n->children[1] = new Node(midX, n->xf, n->y0, midY,
                                                                                             n->z0, midZ);
        insert(n->children[1]); /*}*/
        /*if (n->x0 != midX && midY != n->yf && n->z0 != midZ) {*/ n->children[2] = new Node(n->x0, midX, midY, n->yf,
                                                                                             n->z0, midZ);
        insert(n->children[2]); /*}*/
        /*if (midX != n->xf && midY != n->yf && n->z0 != midZ) {*/ n->children[3] = new Node(midX, n->xf, midY, n->yf,
                                                                                             n->z0, midZ);
        insert(n->children[3]); /*}*/
        /*if (n->x0 != midX && n->y0 != midY && midZ != n->zf) {*/ n->children[4] = new Node(n->x0, midX, n->y0, midY,
                                                                                             midZ, n->zf);
        insert(n->children[4]); /*}*/
        /*if (midX != n->xf && n->y0 != midY && midZ != n->zf) {*/ n->children[5] = new Node(midX, n->xf, n->y0, midY,
                                                                                             midZ, n->zf);
        insert(n->children[5]); /*}*/
        /*if (n->x0 != midX && midY != n->yf && midZ != n->zf) {*/ n->children[6] = new Node(n->x0, midX, midY, n->yf,
                                                                                             midZ, n->zf);
        insert(n->children[6]); /*}*/
        /*if (midX != n->xf && midY != n->yf && midZ != n->zf) {*/ n->children[7] = new Node(midX, n->xf, midY, n->yf,
                                                                                             midZ, n->zf);
        insert(n->children[7]); /*}*/
    }

    void insert(vector<vector<vector<int>>> &m) {
        this->m = m;
        root = new Node(0, m[0].size() - 1, 0, m[0][0].size() - 1, 0, m.size() - 1);
        insert(root);
    }

    struct punto {
        int x, y, z;
        int color;

        punto(int x, int y) : x(x), y(y) {};

        punto(int x, int y, int z, int color) : x(x), y(y), z(z), color(color) {};

        void colored(int color) { this->color = color; };
    };

    void split_frente(int y1, int z1, int y2, int z2) {
        double m;
        if(y2 == y1)m=0;
        else
            m = ( (double)z2 - (double)z1) / ((double)y2 - (double)y1);
        printf("%f",m);
        //X = m . Z + c

        double c = z1 -(m * y1);
        vector<punto *> puntos;
        llenar_puntos_frente(root, puntos, m, c, y1, y2, z1, z2);

        cimg_library::CImg<unsigned char> img(511, 40);
        for (auto c : puntos) {
            //cout << c->x << " " << c->y << " " << c->z << " " << c->color << endl;
            img(c->x, c->z) = c->color;
        }
        img.display();
    }

    void llenar_puntos_frente(Node *node, vector<punto *>& puntos, double m, float c, int y1, int z1, int y2, int z2) {
        if (node->is_terminal) {
            for (int i = node->y0; i < node->yf; i++) {
                int y_temp = i;
                int z_temp = m * i + c;
                if (z_temp >= node->z0 && z_temp <= node->zf) {
                    //cout << x_temp << " " << z_temp << " " << endl;
                    for (int j = node->x0; j < node->xf; j++){
                        //if (x_temp <= x2 and (x_temp >= x1)) {
                        //if(y_temp <= y2 and (y_temp >= y1))
                        puntos.push_back(new punto(j, y_temp, z_temp, node->color));
                        //}
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
        printf("%f",m);
        //X = m . Z + c

        double c = z1 -(m * x1);
        vector<punto *> puntos;
        llenar_puntos_costado(root, puntos, m, c, x1, z1, x2, z2);

        cimg_library::CImg<unsigned char> img(512, 512);
        for (auto c : puntos) {
            //cout << c->x << " " << c->y << " " << c->z << " " << c->color << endl;
            img(c->x, c->y) = c->color;
        }
        img.display();
    }


    void llenar_puntos_costado(Node *node, vector<punto *>& puntos, double m, float c, int x1, int z1, int x2, int z2) {
        if (node->is_terminal) {
            for (int i = node->x0; i < node->xf; i++) {
                int x_temp = i;
                int z_temp = m * i + c;
                if (z_temp >= node->z0 && z_temp <= node->zf) {
                    //cout << x_temp << " " << z_temp << " " << endl;
                    for (int j = node->y0; j < node->yf; j++){
                        //if (x_temp <= x2 and (x_temp >= x1)) {
                            //if(y_temp <= y2 and (y_temp >= y1))
                            puntos.push_back(new punto(x_temp, j, z_temp, node->color));
                        //}
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
