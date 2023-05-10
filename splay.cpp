#include <cstdint>
#include <iostream>

class Rng {
public:
   uint32_t seed;
   bool flag;
   uint32_t current;
   Rng(uint32_t s) {
       seed = s;
       flag = false;
   }
   uint32_t next() {
       if (flag) {
           current = (1664525 * current) + 1013904223;
           return current;
       } else {
           flag = true;
           current = seed;
           return seed;
       }
   }
};

class Node {
public:
   uint32_t key;
   Node* par;
   Node* left;
   Node* right;
   Node(uint32_t k) {
       key = k;
       left = nullptr;
       right = nullptr;
       par = nullptr;
   }
};

class SplayTree {
public:
   Node* root{};
   SplayTree() = default;
};

int treeInsert(uint32_t key, Node* node, int depth) {
   if (key < node->key) {
       if (node->left == nullptr) {
           node->left = new Node(key);
           node->left->par = node;
           return depth + 1;
       } else {
           return treeInsert(key, node->left, depth + 1);
       }
   } else if (key > node->key) {
       if (node->right == nullptr) {
           node->right = new Node(key);
           node->right->par = node;
           return depth + 1;
       } else {
           return treeInsert(key, node->right, depth + 1);
       }
   } else {
       return -1;
   }
}

int treeInsertWard(uint32_t key, SplayTree* tree) {
   if (tree->root == nullptr) {
       tree->root = new Node(key);
       return 0;
   } else {
       return treeInsert(key, tree->root, 0);
   }
}

Node* treeFind(uint32_t key, Node* node, int* depth) {
   if (key < node->key) {
       if (node->left == nullptr) {
           *depth = -1;
           return nullptr;
       } else {
           *depth = *depth + 1;
           return treeFind(key, node->left, depth);
       }
   } else if (key > node->key) {
       if (node->right == nullptr) {
           *depth = -1;
           return nullptr;
       } else {
           *depth = *depth + 1;
           return treeFind(key, node->right, depth);
       }
   } else {
       return node;
   }
}

Node* rotate_right(Node* node) {
   Node* P = node->par;
   Node* L = node->left;
   Node* LR = L->right;
   node->left = LR;
   if (LR != nullptr) {
       LR->par = node;
   }
   L->right = node;
   node->par = L;
   L->par = P;
   if (P != nullptr && P->left == node) {
       P->left = L;
   } else if (P != nullptr && P->right == node) {
       P->right = L;
   }
   return L;
}

Node* rotate_left(Node* node) {
   Node* P = node->par;
   Node* R = node->right;
   Node* RL = R->left;
   node->right = RL;
   if (RL != nullptr) {
       RL->par = node;
   }
   R->left = node;
   node->par = R;
   R->par = P;
   if (P != nullptr && P->left == node) {
       P->left = R;
   } else if (P != nullptr && P->right == node) {
       P->right = R;
   }
   return R;
}

Node* zig(Node* node) {
   Node* P = node->par;
   if (node == P->left) {
       return rotate_right(P);
   } else {
       return rotate_left(P);
   }
}

Node* zigzag(Node* node) {
   Node* P = node->par;
   Node* G = P->par;
   if (P == G->left) {
       if (node == P->left) {
           rotate_right(G);
           return rotate_right(P);
       } else {
           rotate_left(P);
           return rotate_right(G);
       }
   } else {
       if (node == P->right) {
           rotate_left(G);
           return rotate_left(P);
       } else {
           rotate_right(P);
           return rotate_left(G);
       }
   }
}

Node* splay(Node* node) {
   while (node->par != nullptr) {
       Node* G = node->par->par;
       if (G == nullptr) {
           zig(node);
       } else {
           zigzag(node);
       }
   }
   return node;
}

int treeQuery(uint32_t key, SplayTree* tree) {
   int depth = 0;
   if (tree->root != nullptr) {
       Node* X = treeFind(key, tree->root, &depth);
       if (X != nullptr) {
           tree->root = splay(X);
       }
       return depth;
   } else {
       return -1;
   }
}

void burnIn(SplayTree* tree, Rng* rng, uint32_t burnInSize, uint32_t universe) {
   for (int i = 0; i < burnInSize; ++i) {
       treeInsertWard(rng->next() % universe, tree);
   }
}

void simulate(SplayTree* tree, Rng* rng, uint32_t simSize, uint32_t universe, uint32_t insertFreq, uint32_t findFreq, uint32_t printFreq) {
   for (int i = 0; i < simSize; ++i) {
       bool printFlag = i % printFreq == 0;
       uint32_t X = rng->next();
       bool op = X % (insertFreq + findFreq) < insertFreq;
       if (op) {
           uint32_t k = rng->next() % universe;
           int depth = treeInsertWard(k, tree);
           if (printFlag) {
               std::cout << "I " << k << " " << depth << std::endl;
           }
       } else {
           uint32_t k = rng->next() % universe;
           int depth = treeQuery(k, tree);
           if (printFlag) {
               std::cout << "Q " << k << " " << depth << std::endl;
           }
       }
   }
}



int main() {
   uint32_t S;
   uint32_t U;
   uint32_t B;
   uint32_t N;
   uint32_t I;
   uint32_t Q;
   uint32_t P;
   std::cin >> S;
   std::cin >> U;
   std::cin >> B;
   std::cin >> N;
   std::cin >> I;
   std::cin >> Q;
   std::cin >> P;

   Rng* rng = new Rng(S);
   SplayTree* tree = new SplayTree();
   burnIn(tree, rng, B, U);
   simulate(tree, rng, N, U, I, Q, P);
   return 0;
};
