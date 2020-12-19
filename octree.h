#include <vector>
using namespace std;

class Octree {

private:

	struct Node {

		int x0, xf, y0, yf, z0, zf;
		Node* children[8];
		int color;
		bool is_terminal = false;

		Node(int x0, int xf, int y0, int yf, int z0, int zf) : x0{x0}, xf{xf}, y0{y0}, yf{yf}, z0{z0}, zf{zf} {
			
			for (int i = 0; i < 4; i++) {
				children[i] = nullptr;
			}

		}

	};

	Node* root;

public:

	bool SameColor(Node* &n, vector<vector<vector<int>>> m) {
		int color = m[n->x0][n->y0][n->zf];
		for (int k = n->z0; k < n->zf; k++) {
			for (int i = n->x0; i < n->xf; i++) {
				for (int j = n->y0; j < n->yf; j++) {
					if (m[i][j][k] != color) {
						return false;
					}
				}
			}
		}
		return true;
	}

	void insert(Node* &n, vector<vector<vector<int>>> m) {
		if (SameColor(n, m)) {
			n->color = m[n->x0][n->y0][n->zf];
		}
		int midX = (n->xf+n->x0)/2;
		int midY = (n->yf+n->y0)/2;
		int midZ = (n->zf+n->z0)/2;
		n->children[0] = new Node(n->x0, midX, n->y0, midY, n->z0, midZ);
		n->children[1] = new Node(midX+1, n->xf, n->y0, midY, n->z0, midZ);
		n->children[2] = new Node(n->x0, midX, midY+1, n->yf, n->z0, midZ);
		n->children[3] = new Node(midX+1, n->xf, midY+1, n->yf, n->z0, midZ);
		n->children[4] = new Node(n->x0, midX, n->y0, midY, midZ+1, n->zf);
		n->children[5] = new Node(midX+1, n->xf, n->y0, midY, midZ+1, n->zf);
		n->children[6] = new Node(n->x0, midX, midY+1, n->yf, midZ+1, n->zf);
		n->children[7] = new Node(midX+1, n->xf, midY+1, n->yf, midZ+1, n->zf);
		insert(n->children[0], m);
		insert(n->children[1], m);
		insert(n->children[2], m);
		insert(n->children[3], m);
		insert(n->children[4], m);
		insert(n->children[5], m);
		insert(n->children[6], m);
		insert(n->children[7], m);
	}

	void insert(vector<vector<vector<int>>> m) {
		root = new Node(0, m.size(), 0, m[0].size(), 0, m[0][0].size());
		insert(root, m);
	}

};
