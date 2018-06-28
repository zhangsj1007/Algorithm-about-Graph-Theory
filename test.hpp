//
//  test.hpp
//  GraphAlgorithm
//
//  Created by songjian zhang on 2017/7/17.
//  Copyright © 2017年 songjian zhang. All rights reserved.
//

#ifndef test_hpp
#define test_hpp

#include <stdio.h>

#include "graph.hpp"
#include "graph_algorithm.hpp"
#include "ford_fulkerson.hpp"

class Test{
public:
    Test(GraphType graphType, int matrix[][4], int n, int m);
    void TestPrintMatrix();
    void TestFordFulkerson();
    static void TestTwoDiviedMatch(int matrix[][3], int n, int m);
    static void TestPushRelabel();
    void TestDijkstraWithRugularPath();
private:
    Graph graph_;
    GraphAlgorithm graphAlgorithm_;
};

#endif /* test_hpp */
