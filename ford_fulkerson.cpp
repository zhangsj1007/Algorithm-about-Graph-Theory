//
//  ford_fulkerson.cpp
//  GraphAlgorithm
//
//  Created by songjian zhang on 2017/7/18.
//  Copyright © 2017年 songjian zhang. All rights reserved.
//

#include "ford_fulkerson.hpp"
#include <queue>
#include <algorithm>
#include <iostream>
#include <set>


FordFulkerson::FordFulkerson(Graph& graph): graph_(graph){

}

int FordFulkerson::EdmondsKarpMaxFlow(uint32_t s, uint32_t t){
    int sum = 0;
    int32_t** graph = CopyGraph(graph_.GetMtrix());
    int32_t** f = GenerateMatrix();
    int32_t** r = GenerateMatrix();
    ResidualNetwork(graph, f, r);
    //parents是用来回溯的
    int* parents = new int[graph_.vernum_+1];
    InitZero(parents, graph_.vernum_+1);
    while(AugmentPath(r, s, t, parents)){
        int cfp = MinimumAugment(r, t, parents);
        int ver = t;
        while (parents[ver] != 0){
            f[parents[ver]][ver] += cfp;
            //只有取反，在计算残差矩阵r的时候才会有tail到head的一条边
            f[ver][parents[ver]] = -f[parents[ver]][ver];
            ver = parents[ver];
        }
        sum += cfp;
        ResidualNetwork(graph, f, r);
        InitZero(parents, graph_.vernum_+1);
    }
    
    if (residualNetwork_ != NULL){
        delete[] residualNetwork_;
    }
    
    if (flowNetwork_ != NULL){
        delete [] flowNetwork_;
    }
    residualNetwork_ = CopyGraph((uint32_t**)r);
    flowNetwork_ = GenerateMatrix();
    CalculateFlowNetwork(graph, r);
    
    PrintMatrix(residualNetwork_, graph_.vernum_);
    PrintMatrix(flowNetwork_, graph_.vernum_);
    
    bool visited[graph_.vernum_+1];
    InitZero(visited, graph_.vernum_+1);
    
    Handle handler = boost::bind(&FordFulkerson::ReachedSet, this, _1, visited);
    
    Bfs(residualNetwork_, 1, handler);
    
    for (int i = 1; i <= graph_.vernum_; i++){
        if (visited[i]){
            std::cout << i << std::endl;
        }
    }
    
    DeleteMatrix(graph, graph_.vernum_+1);
    DeleteMatrix(r, graph_.vernum_+1);
    DeleteMatrix(f, graph_.vernum_+1);
    
    
    return sum;
}

int FordFulkerson::CalculateTwoDividedMatch(int matrix[][3], int vernum, int arcnum){
    std::set<int> hasIndegrees;
    std::set<int> hasOutdegrees;
    for(int i = 0; i < arcnum; i++){
        matrix[i][0] += 1;
        matrix[i][1] += 1;
        hasIndegrees.insert(matrix[i][0]);
        hasOutdegrees.insert(matrix[i][1]);
    }
    int vernumNew = vernum + 2;
    int arcnumNew = arcnum + vernum;
    Graph graph(GraphType::WithWeightWithDirection, vernumNew, arcnumNew);
    graph.Init();
    for (int ver : hasIndegrees){
        //grade default 0
        graph.InsertArc(1, ver, 1, 0);
    }
    for (int i = 0 ; i < arcnum; i++){
        graph.InsertArc(matrix[i][0], matrix[i][1], INT_MAX, 0);
    }
    for (int ver : hasOutdegrees){
        graph.InsertArc(ver, vernum+2, 1, 0);
    }
    FordFulkerson fF(graph);
    return fF.EdmondsKarpMaxFlow(1, vernum+2);
}

int32_t** FordFulkerson::GetResidualNetwork(){
    return residualNetwork_;
}

int32_t** FordFulkerson::GetFlowNetwork(){
    return flowNetwork_;
}

FordFulkerson::~FordFulkerson(){
    delete[] residualNetwork_;
    delete [] flowNetwork_;
}

int32_t** FordFulkerson::CopyGraph(uint32_t** matrix){
    int32_t** matrixNew = new int32_t* [graph_.vernum_ + 1];
    for (int i = 0; i <= graph_.vernum_; i++){
        matrixNew[i] = new int32_t[graph_.vernum_ + 1];
    }
    
    for (int i = 0; i <= graph_.vernum_; i++){
        for (int j = 0; j <= graph_.vernum_; j++){
            matrixNew[i][j] = matrix[i][j];
        }
    }
    return matrixNew;
}



int32_t** FordFulkerson::GenerateMatrix(){
    int32_t** matrixNew = new int32_t* [graph_.vernum_ + 1];
    for (int i = 0; i <= graph_.vernum_; i++){
        matrixNew[i] = new int32_t[graph_.vernum_ + 1];
    }
    for (int i = 0; i <= graph_.vernum_; i++){
        for (int j = 0; j <= graph_.vernum_; j++){
            matrixNew[i][j] = 0;
        }
    }
    return matrixNew;
}

void FordFulkerson::ResidualNetwork(int32_t** graph, int32_t** f, int32_t** r){
    for (int i = 1; i <= graph_.vernum_; i++){
        for (int j = 1; j <= graph_.vernum_; j++){
            r[i][j] = graph[i][j] - f[i][j];
        }
    }
}

bool FordFulkerson::AugmentPath(int32_t** r, int32_t s, int32_t t, int* parents){
    //compute residual matrix
    VerNode* vers = Graph::GenerateNodeArray((uint32_t**)r, graph_.vernum_);
    bool visited[graph_.vernum_+1];
    std::queue<VerNode, std::deque<VerNode> > q;
    bool suc = false;
    
    InitZero(visited, graph_.vernum_+1);
    visited[s] = true;
    q.push(vers[s]);
    //bfs finds t
    while(!q.empty()){
        VerNode ver = q.front();
        q.pop();
        ArcNode* arc = ver.firstArc;
        while (arc != NULL){
            if(!visited[arc->tail]) {
                q.push(vers[arc->tail]);
                parents[arc->tail] = ver.vertex;
                visited[arc->tail] = true;
                if (arc->tail == t){
                    suc = true;
                    break;
                }
            }
            arc = arc->nextArc;
        }
        if (suc){
            break;
        }
    }
    Graph::DestroyNodeArray(vers, graph_.vernum_);
    return suc;
}

int FordFulkerson::MinimumAugment(int32_t** r, uint32_t t, int* parents){
    uint32_t cfp = UINT_MAX;
    int parent = parents[t];
    while(parent != 0){
        //使得一条边的流量流尽
        cfp = std::min(cfp, (uint32_t)r[parent][t]);
        t = parent;
        parent = parents[t];
    }
    return cfp;
}

void FordFulkerson::InitZero(int* matrix, int n){
    for (int i = 0; i < n; i++){
        matrix[i] = 0;
    }
}

void FordFulkerson::InitZero(bool* matrix, int n){
    for(int i = 0; i < n; i++){
        matrix[i] = false;
    }
}

void FordFulkerson::PrintMatrix(int32_t** matrix, int n){
    for (int i = 1; i <= n; i++){
        for (int j = 1; j <= n; j++){
            std::cout << matrix[i][j] << "  ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void FordFulkerson::CalculateFlowNetwork(int32_t** graph, int32_t** r){
    for (int i = 1; i <= graph_.vernum_; i++){
        for (int j = 1; j <= graph_.vernum_; j++){
            flowNetwork_[i][j] = graph[i][j] - r[i][j];
        }
    }
}

void FordFulkerson::Bfs(int32_t** matrix, int s, Handle handler){
    VerNode* vers = Graph::GenerateNodeArray((uint32_t**)matrix, graph_.vernum_);
    bool visited[graph_.vernum_+1];
    std::queue<VerNode, std::deque<VerNode> > q;
    
    InitZero(visited, graph_.vernum_+1);
    visited[s] = true;
    handler(s);
    q.push(vers[s]);
    while(!q.empty()){
        VerNode ver = q.front();
        q.pop();
        ArcNode* arc = ver.firstArc;
        while (arc != NULL){
            if(!visited[arc->tail]) {
                q.push(vers[arc->tail]);
                handler(arc->tail);
                visited[arc->tail] = true;
            }
            arc = arc->nextArc;
        }
    }
    Graph::DestroyNodeArray(vers, graph_.vernum_);
}

void FordFulkerson::ReachedSet(int n, bool* visited){
    visited[n] = true;
}

void FordFulkerson::DeleteMatrix(int32_t** matrix, uint32_t n){
    for (uint32_t i = 0; i < n; i++){
        delete [] matrix[i];
    }
    delete [] matrix;
    matrix = NULL;
}
