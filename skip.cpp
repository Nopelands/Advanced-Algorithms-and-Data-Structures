#include <iostream>
#include <vector>

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
   //uint32_t value;
   int height;
   std::vector<Node*> next;
   Node(int h) {
       height = h;
       for (int i = 0; i < height; ++i) {
           next.push_back(nullptr);
       }
   }
};

int heightGen(int height, Rng* rng) {
   int nodeHeight = 1;
   int maxHeight = height + 1;
   while ((rng->next() % 100) < 50 && nodeHeight < maxHeight) {
       nodeHeight++;
   }
   return nodeHeight;
}

class SkipList {
public:
   Node* head;
   int listHeight;
   SkipList(int lHeight) {
       head = new Node(lHeight);
       head->key = 0;
       listHeight = lHeight;
       for (int i = 0; i < lHeight; ++i) {
           head->next.push_back(nullptr);
       }
   }

   std::vector<Node*> precursors(uint32_t key, int* visits) {
       std::vector<Node*> pointers;
       for (int i = 0; i < listHeight; ++i) {
           pointers.push_back(nullptr);
       }
       Node* cur = head;
       for (int l = listHeight - 1; l >= 0; --l) {
           while (cur->next[l] != nullptr && cur->next[l]->key < key) {
               cur = cur->next[l];
               *visits = *visits + 1;
           }
           pointers[l] = cur;
       }
       return pointers;
   };

   int listInsert(uint32_t key, Rng* rng) {
       int lol = 0;
       std::vector<Node*> pointers = precursors(key, &lol);
       if (pointers[0]->next[0] != nullptr && pointers[0]->next[0]->key == key) {
           //pointers[0]->next[0]->value = val;
           return 0;
       }
       int nodeHeight = heightGen(listHeight, rng);
       if (nodeHeight > listHeight) {
           for (int i = listHeight; i <= nodeHeight; ++i) {
               head->next.insert(head->next.begin() + i, nullptr);
               pointers.insert(pointers.begin() + i, head);
           }
           listHeight = nodeHeight;
           head->height = nodeHeight;
       }
       Node* node = new Node(nodeHeight);
       node->key = key;
       //node->value = val;
       for (int l = 0; l <= nodeHeight - 1; ++l) {
           node->next[l] = pointers[l]->next[l];
           pointers[l]->next[l] = node;
       }
       return 1;
   }

   int listDelete(uint32_t key) {
       int lol = 0;
       std::vector<Node*> pointers = precursors(key, &lol);
       if (pointers[0]->next[0] == nullptr || pointers[0]->next[0]->key != key) {
           return 0;
       }
       Node* node = pointers[0]->next[0];
       for (int l = 0; l <= node->height - 1; ++l) {
           pointers[l]->next[l] = node->next[l];
           while (listHeight > 1 && head->next[listHeight - 1] == nullptr) {
               listHeight = listHeight - 1;
               head->height = head->height - 1;
           }
       }
       return 1;
   }

   Node* listFind(uint32_t key, bool printFlag) {
       int visits = 1;
       std::vector<Node*> pointers = precursors(key, &visits);
       if (printFlag) {
           std::cout << "F " << visits;
       }
       if (pointers[0]->next[0] != nullptr && pointers[0]->next[0]->key == key) {
           return pointers[0]->next[0];
       } else {
           return nullptr;
       }
   }
};

void burnIn(SkipList* list, uint32_t size, Rng* rng, uint32_t universe) {
   for (int i = 0; i < size; ++i) {
       list->listInsert(rng->next() % universe, rng);
   }
}

void simulate(SkipList* list, uint32_t size, Rng* rng, uint32_t universe, uint32_t find, uint32_t insert, uint32_t del, uint32_t printFreq) {
   for (int i = 0; i < size; ++i) {
       bool printFlag = i % printFreq == 0;
       uint32_t op = rng->next() % (find + insert + del);
       if (op < find) {
           Node* node = list->listFind(rng->next() % universe, printFlag);
           if (printFlag) {
               if (node != nullptr){
                   std::cout << " " << node->height << std::endl;
               } else {
                   std::cout << " 0" << std::endl;
               }
           }
       } else if (op < find + insert) {
           int temp = list->listInsert(rng->next() % universe, rng);
           if (printFlag) {
               std::cout << "I " << temp << std::endl;
           }
       } else {
           int temp = list->listDelete(rng->next() % universe);
           if (printFlag) {
               std::cout << "D " << temp << std::endl;
           }
       }
   }
}

int main() {
   uint32_t S;
   uint32_t U;
   uint32_t B;
   uint32_t N;
   uint32_t F;
   uint32_t I;
   uint32_t D;
   uint32_t P;
   std::cin >> S;
   std::cin >> U;
   std::cin >> B;
   std::cin >> N;
   std::cin >> F;
   std::cin >> I;
   std::cin >> D;
   std::cin >> P;

   Rng* rng = new Rng(S);
   auto* list = new SkipList(1);
   burnIn(list, B, rng, U);
   simulate(list, N, rng, U, F, I, D, P);
   return 0;
}
