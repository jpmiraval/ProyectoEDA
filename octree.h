#include <vector>
#include <iostream>
#include "CImg.h"

using namespace std;

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


    void split(int x1, int y1, int x2, int y2) {
        int m = floor((y2 - y1) / (x2 - x1));
        //Y = m . x + c
        int c = y1 -(m * x1);
        vector<punto *> puntos;
        llenar_puntos(root, puntos, m, c, x1, x2, y1, y2);

        cimg_library::CImg<unsigned char> img(512, 41);
        for (auto c : puntos) {
            cout << c->x << " " << c->y << " " << c->z << " " << c->color << endl;
            img(c->x, c->z) = c->color;
        }
        img.display();
    }


    void llenar_puntos(Node *node, vector<punto *>& puntos, int m, int c, int x1, int y1, int x2, int y2) {
        if (node->is_terminal) {
            for (int i = node->x0; i < node->xf; i++) {
                int x_temp = i;
                int y_temp = m * i + c;
                if (y_temp > node->y0 && y_temp < node->yf) {
                    cout << x_temp << " " << y_temp << " " << endl;
                    for (int j = node->z0; j < node->zf; j++){
                        //if (x_temp <= x2 and (x_temp >= x1)) {
                            //if(y_temp <= y2 and (y_temp >= y1))
                            puntos.push_back(new punto(x_temp, y_temp, j, node->color));
                        //}
                    }

                }
            }
            return;
        }

        for (int i = 0; i < 8; i++) {
            if (node->children[i]->x0 * m + c > node->yf || node->children[i]->xf * m + c < node->y0) {
                continue;
            } else {
                llenar_puntos(node->children[i], puntos, m, c, x1, y1, x2, y2);
            }
        }
        return;
    }
};
