#include <iostream>
#include <deque>

uint32_t next_rng(uint32_t current) {
   return ((1664525 * current) + 1013904223);
}

class minQueue {
public:
   std::deque<uint32_t> A;
   std::deque<uint32_t> M;
   minQueue() = default;
};

void minQueueEnqueue(minQueue* Q, uint32_t val, uint32_t* position) {
   Q->A.push_back(val);
   int aux = 0;
   while (!Q->M.empty() && Q->M.size() > aux && Q->M[Q->M.size() - 1 - aux] > val) {
       aux++;
       //Q->M.pop_back();
   }
   if (aux != 0) {
       Q->M = std::deque<uint32_t> (Q->M.begin(), Q->M.end() - aux);
       //Q->M.erase(Q->M.end() - aux,Q->M.end());
       Q->M.push_back(val);
       if (Q->M.size() == 1) {
           *position = Q->A.size() - 1;
       }
   } else {
       Q->M.push_back(val);
   }
}

void minQueueDequeue(minQueue* Q, uint32_t* position) {
   uint32_t temp = Q->A[0];
   Q->A.pop_front();
   if (temp == Q->M[0]) {
       Q->M.pop_front();
   }
   if (*position - 1 == 4294967295) {
       uint32_t aux = 0;
       for (int i = 0; i < Q->A.size(); ++i) {
           if (Q->A[i] == Q->M[0]) {
               *position = i;
           }
       }
   } else {
       *position = *position - 1;
   }
}

//uint32_t minQueueMin(minQueue* Q) {
//    uint32_t val = Q->M[0];
//    for (uint32_t i = 0; i < Q->A.size(); ++i) {
//        if (Q->A[i] == val) {
//            return i;
//        }
//    }
//    std::cout << "AAAAAAAAA" << std::endl;
//}

uint32_t burnIn(minQueue* Q, uint32_t size, uint32_t rng, uint32_t* position) {
   uint32_t temp = rng;
   for (int i = 0; i < size; ++i) {
       minQueueEnqueue(Q, temp, position);
       temp = next_rng(temp);
   }
   return temp;
}


void runOps(minQueue* Q, uint32_t N, uint32_t P, uint32_t rng, uint32_t* position) {
   uint32_t temp = rng;
   for (int i = 0; i < N; ++i) {
       if (temp % 10 < P) {
           temp = next_rng(temp);
           minQueueEnqueue(Q, temp, position);
       } else {
           minQueueDequeue(Q, position);
       }
       temp = next_rng(temp);
       std::cout << Q->M.size() << " " << *position << std::endl;
   }
}

int main() {
   uint32_t S;
   uint32_t B;
   uint32_t N;
   uint32_t P;
   std::cin >> S;
   std::cin >> B;
   std::cin >> N;
   std::cin >> P;

   uint32_t position = 0;
   auto* queue = new minQueue();
   S = burnIn(queue, B, S, &position);
   runOps(queue, N, P, S, &position);
   return 0;
}
