#include <cstdint>
#include <iostream>
#include <vector>
#include <tuple>
#include <bits/stdc++.h>

class Graph {
public:
   int vNum;
   std::vector<int> vDegree;
   std::vector<std::vector<int>> vSucessors;
   explicit Graph(int n) {
       vNum = n;
   }
};

void DFS_visit(Graph* graph, int u, std::vector<bool>* vis, std::vector<int>* ord) {
   vis->at(u) = true;
   for (int v = 0; v < graph->vSucessors[u].size(); ++v) {
       if (not (*vis)[graph->vSucessors[u][v]]) {
           DFS_visit(graph, graph->vSucessors[u][v], vis, ord);
       }
   }
   ord->push_back(u);
}

std::vector<int> DFS_post_order(Graph* graph) {
   std::vector<bool> vis;
   for (int i = 0; i < graph->vNum; ++i) {
       vis.push_back(false);
   }
   std::vector<int> ord;
   for (int u = 0; u < graph->vNum; ++u) {
       if (!vis[u]) {
           DFS_visit(graph, u, &vis, &ord);
       }
   }
   return ord;
}

Graph* transpose(Graph* graph) {
   Graph* graphTransposed = new Graph(graph->vNum);
   std::vector<int> aux;
   for (int i = 0; i < graphTransposed->vNum; ++i) {
       graphTransposed->vSucessors.push_back(aux);
   }
   for (int u = 0; u < graph->vNum; ++u) {
       for (int i = 0; i < graph->vSucessors[u].size(); ++i) {
           graphTransposed->vSucessors[graph->vSucessors[u][i]].push_back(u);
       }
   }
   return graphTransposed;
}

std::vector<std::vector<int>> findSCC(Graph* graph) {
   std::vector<int> F = DFS_post_order(graph);
   Graph* graphTransposed = transpose(graph);
   std::vector<bool> vis;
   for (int i = 0; i < graph->vNum; ++i) {
       vis.push_back(false);
   }
   std::vector<std::vector<int>> SCC;
   for (int i = F.size() - 1; i >= 0; --i) {
       int u = F[i];
       if (!vis[u]) {
           std::vector<int> ord;
           DFS_visit(graphTransposed, u, &vis, &ord);
           SCC.push_back(ord);
       }
   }
   return SCC;
}

bool compareVector(std::vector<int> v1, std::vector<int> v2) {
   return v1[0] < v2[0];
}

std::vector<std::vector<int>> sortOutput(std::vector<std::vector<int>> input) {
   std::vector<std::vector<int>> aux;
   for (int i = 0; i < input.size(); ++i) {
       std::sort(input[i].begin(), input[i].end());
   }
   for (int i = input.size() - 1; i >=0; --i) {
       aux.push_back(input[i]);
   }
   std::sort(aux.begin(), aux.end(), compareVector);
   return aux;
}

std::vector<std::vector<int>> simulate(Graph* graph) {
   return sortOutput(findSCC(graph));
}

int main() {
   int C;
   std::cin >> C;
   for (int i = 0; i < C; ++i) {
       int N;
       std::cin >> N;
       Graph* graph = new Graph(N);
       for (int j = 0; j < N; ++j) {
           int why;
           std::cin >> why;
           int D;
           std::cin >> D;
           char y;
           std::cin >> y;
           graph->vDegree.push_back(D);
           std::vector<int> sucessors;
           for (int k = 0; k < D; ++k) {
               int W;
               std::cin >> W;
               sucessors.push_back(W);
           }
           graph->vSucessors.push_back(sucessors);
       }
       std::vector<std::vector<int>> SCC = simulate(graph);
       std::cout << SCC.size() << std::endl;
       for (int j = 0; j < SCC.size(); ++j) {
           for (int k = 0; k < SCC[j].size(); ++k) {
               if (k != SCC[j].size() - 1) {
                   std::cout << SCC[j][k] << " ";
               } else {
                   std::cout << SCC[j][k];
               }
           }
           std::cout << std::endl;
       }
       std::cout << std::endl;
   }
   return 0;
};
