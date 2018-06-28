//
//  test.cpp
//  GraphAlgorithm
//
//  Created by songjian zhang on 2017/7/17.
//  Copyright © 2017年 songjian zhang. All rights reserved.
//

#include "test.hpp"
#include <iostream>
#include "push-relabel.hpp"

int matrix[6][6] = {
    {0, 10, 10, 0, 0, 0},
    {0, 0, 2, 8, 4, 0},
    {0, 0, 0, 9, 0, 0},
    {0, 0, 0, 0, 9, 10},
    {0, 0, 0, 0, 0, 10},
    {0, 0, 0, 0, 0, 0}
};

Test::Test(GraphType graphType, int matrix[][4], int n, int m):graph_(graphType, n, m), graphAlgorithm_(){
    graph_.Init();
    for(int i = 0; i < m; i++){
        graph_.InsertArc(matrix[i][0], matrix[i][1], matrix[i][2], matrix[i][3]);
    }
}

void Test::TestPrintMatrix(){
    graph_.GetMtrix();
    graph_.PrintMatrix();
}

void Test::TestFordFulkerson(){
    /*FordFulkerson fF(graph_);
    int result = fF.EdmondsKarpMaxFlow(1, 6);
    std::cout << "result = " <<  result << std::endl;
}

void Test::TestTwoDiviedMatch(int matrix[][3], int n, int m){
    int result = FordFulkerson::CalculateTwoDividedMatch(matrix, n, m);
    std::cout << "result = " <<  result << std::endl;
}

void Test::TestPushRelabel(){
    PushRelabel pushRelable((int*)matrix, 6);
    pushRelable.PrintGraph();
    pushRelable.CalculateFlow();
    pushRelable.PrintGraph();*/
}

void Test::TestDijkstraWithRugularPath(){
    graphAlgorithm_.DijkstraWithRugularPath(&graph_);
}
