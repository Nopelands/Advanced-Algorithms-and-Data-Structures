#include <cstdint>
#include <iostream>
#include <vector>
#include <tuple>

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
   uint32_t prty;
   Node* par;
   std::vector<Node*> chd;
   Node(uint32_t k, uint32_t p) {
       key = k;
       prty = p;
       chd.insert(chd.begin(), nullptr);
       chd.insert(chd.begin() + 1, nullptr);
       par = nullptr;
   }
};

class Treap {
public:
   Node* root{};
   Treap() = default;
};

Node* treapMerge(Node *rootx, Node *rooty) {
   if (rootx == nullptr) {
       return rooty;
   } else if (rooty == nullptr) {
       return rootx;
   } else if (rootx->prty > rooty->prty) {
       rootx->chd[1] = treapMerge(rootx->chd[1], rooty);
       return rootx;
   } else {
       rooty->chd[0] = treapMerge(rootx, rooty->chd[0]);
       return rooty;
   }
}

std::tuple<Node *, Node *> treapSplit(Node *root, uint32_t key) {
   if (root == nullptr) {
       return std::make_tuple(nullptr, nullptr);
   } else if (key == root->key) {
       return std::make_tuple(root->chd[0],root->chd[1]);
   } else if (key < root->key) {
       Node* lt;
       std::tie(lt, root->chd[0]) = treapSplit(root->chd[0], key);
       return std::make_tuple(lt, root);
   } else {
       Node* gt;
       std::tie(root->chd[1], gt) = treapSplit(root->chd[1], key);
       return std::make_tuple(root, gt);
   }
}

std::tuple<Node*, Node*> treapFind(Node* root, uint32_t key, int *depth) {
   Node* par = nullptr;
   Node* cur = root;
   while (cur != nullptr) {
       if (key == cur->key) {
           return std::make_tuple(par, cur);
       } else if (key < cur->key) {
           *depth = *depth + 1;
           par = cur;
           cur = cur->chd[0];
       } else {
           *depth = *depth + 1;
           par = cur;
           cur = cur->chd[1];
       }
   }
   *depth = -1;
   return std::make_tuple(par, cur);
}

int treeQuery(uint32_t key, Treap* tree) {
   int depth = 0;
   if (tree->root == nullptr) {
       return -1;
   }
   treapFind(tree->root, key, &depth);
   return depth;
}

Node* treapDelete(Node* root, uint32_t key, int* depth) {
   if (root == nullptr) {
       *depth = -1;
       return nullptr;
   } else if (key == root->key) {
       return treapMerge(root->chd[0], root->chd[1]);
   } else if (key < root->key) {
       *depth = *depth + 1;
       root->chd[0] = treapDelete(root->chd[0], key, depth);
       return root;
   } else {
       *depth = *depth + 1;
       root->chd[1] = treapDelete(root->chd[1], key, depth);
       return root;
   }
}


int treeDelete(uint32_t key, Treap *tree) {
   int depth = 0;
   if (tree->root == nullptr) {
       return -1;
   }
   tree->root = treapDelete(tree->root, key, &depth);
   return depth;
}

Node* treeInsert(Node* X, Node* root, int* depth) {
   if (root == nullptr) {
       return X;
   } else if (X->prty > root->prty) {
       std::tie(X->chd[0], X->chd[1]) = treapSplit(root, X->key);
       return X;
   } else if (X->key < root->key) {
       *depth = *depth + 1;
       root->chd[0] = treeInsert(X, root->chd[0], depth);
       return root;
   } else {
       *depth = *depth + 1;
       root->chd[1] = treeInsert(X, root->chd[1], depth);
       return root;
   }
}

int treeInsertWard(Rng* rng, uint32_t key, Treap* tree) {
   if (tree->root == nullptr) {
       tree->root = new Node(key, rng->next());
       return 0;
   } else {
       int depth = 0;
       if (treeQuery(key, tree) == -1) {
           tree->root = treeInsert(new Node(key, rng->next()), tree->root, &depth);
           return depth;
       } else {
           return -1;
       }
   }
}

void burnIn(Treap* tree, Rng* rng, uint32_t burnInSize, uint32_t universe) {
   for (int i = 0; i < burnInSize; ++i) {
       uint32_t key = rng->next() % universe;
       treeInsertWard(rng, key, tree);
   }
}

void simulate(Treap* tree, Rng* rng, uint32_t simSize, uint32_t universe, uint32_t insertFreq, uint32_t findFreq, uint32_t printFreq, uint32_t delFreq) {
   for (int i = 0; i < simSize; ++i) {
       bool printFlag = i % printFreq == 0;
       uint32_t X = rng->next() % (insertFreq + delFreq + findFreq);
       if (X < insertFreq) { // insert
           uint32_t key = rng->next() % universe;
           int depth = treeInsertWard(rng, key, tree);
           if (printFlag) {
               std::cout << "I " << key << " " << depth << std::endl;
           }
       } else if (X < insertFreq + delFreq) { // delete
           uint32_t key = rng->next() % universe;
           int depth = treeDelete(key, tree);
           if (printFlag) {
               std::cout << "D " << key << " " << depth << std::endl;
           }
       } else { // query
           uint32_t key = rng->next() % universe;
           int depth = treeQuery(key, tree);
           if (printFlag) {
               std::cout << "Q " << key << " " << depth << std::endl;
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
   uint32_t D;
   uint32_t Q;
   uint32_t P;
   std::cin >> S;
   std::cin >> U;
   std::cin >> B;
   std::cin >> N;
   std::cin >> I;
   std::cin >> D;
   std::cin >> Q;
   std::cin >> P;

   Rng* rng = new Rng(S);
   Treap* tree = new Treap();
   burnIn(tree, rng, B, U);
   simulate(tree, rng, N, U, I, Q, P, D);
   return 0;
};
