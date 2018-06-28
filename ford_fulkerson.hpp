//
//  ford_fulkerson.hpp
//  GraphAlgorithm
//
//  Created by songjian zhang on 2017/7/18.
//  Copyright © 2017年 songjian zhang. All rights reserved.
//

#ifndef ford_fulkerson_hpp
#define ford_fulkerson_hpp

#include <stdio.h>
#include "graph.hpp"

#include "boost/function.hpp"
#include "boost/bind.hpp"



class FordFulkerson{
    typedef  boost::function<void(int)> Handle;
public:
    FordFulkerson(Graph& graph);
    int EdmondsKarpMaxFlow(uint32_t s, uint32_t t);
    static int CalculateTwoDividedMatch(int matrix[][3], int vernum, int arcnum);
    int32_t** GetResidualNetwork();
    int32_t** GetFlowNetwork();
    ~FordFulkerson();
private:
    //由于在计算flow矩阵的时候，使用到了负数，所以这里构造int32_t的matrix
    int32_t** CopyGraph(uint32_t** matrix);
    int32_t** GenerateMatrix();
    void ResidualNetwork(int32_t** graph, int32_t** f, int32_t** r);
    bool AugmentPath(int32_t** r, int32_t s, int32_t t, int* parent);
    int MinimumAugment(int32_t** r, uint32_t t, int* parent);
    void InitZero(int* matrix, int n);
    void InitZero(bool* matrix, int n);
    void PrintMatrix(int32_t** matrix, int n);
    void CalculateFlowNetwork(int32_t** graph, int32_t** r);
    void Bfs(int32_t** matrix, int s, Handle handler);
    void ReachedSet(int n, bool* visited);
    void DeleteMatrix(int32_t** matrix, uint32_t n);
private:
    Graph& graph_;
    int32_t** residualNetwork_ = NULL;
    int32_t** flowNetwork_ = NULL;
};

#endif /* ford_fulkerson_hpp */
