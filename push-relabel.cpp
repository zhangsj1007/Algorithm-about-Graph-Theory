//
//  push-relabel.cpp
//  GraphAlgorithm
//
//  Created by songjian zhang on 2017/7/24.
//  Copyright © 2017年 songjian zhang. All rights reserved.
//

#include "push-relabel.hpp"
#include <algorithm>
#include <iostream>

PushRelabel::PushRelabel(uint32_t* matrix, int n) : n_(n){
    matrix_ = new uint32_t*[n+1];
    for (int i = 0; i <= n; i++){
        matrix_[i] = new uint32_t[n+1];
    }
    for (int i = 0; i <= n; i++){
        for (int j = 0; j <= n; j++){
            matrix_[i][j] = 0;
        }
    }
    for (int i = 1; i <= n; i++){
        for (int j = 1; j <= n; j++){
            matrix_[i][j] = matrix[(i-1)*n + j-1];
        }
    }
    points_ = new Point[n+1];
    InitPoint();
}

int PushRelabel::CalculateFlow(){
    int maxFlow = 0;
    points_[1].height = n_;
    for (int i = 2; i <= n_; i++){
        if (matrix_[1][i] > 0){
            points_[i].flow = matrix_[1][i];
            points_[1].flow -= matrix_[1][i];
            matrix_[i][1] = matrix_[1][i];
            matrix_[1][i] = 0;
        }
    }
    
    while(1){
        bool finished = true;
        for (int u = 2; u < n_; u++){
            if (points_[u].flow > 0){
                finished = false;
                bool relable = true;
                for (int v = 1; v <= n_ && points_[u].flow > 0; v++){
                    if (matrix_[u][v] > 0 && points_[u].height > points_[v].height){
                        relable = false;
                        int bottleneck = std::min(points_[u].flow, matrix_[u][v]);
                        points_[u].flow -= bottleneck;
                        points_[v].flow += bottleneck;
                        matrix_[u][v] -= bottleneck;
                        matrix_[v][u] += bottleneck;
                    }
                }
                if (relable){
                    points_[u].height += 1;
                }
            }
        }
        if (finished){
            for (int i = 1; i <= n_; i++){
                if (matrix_[n_][i] > 0){
                    maxFlow += matrix_[n_][i];
                }
            }
            break;
        }
    }
    std::cout << "Max Flow : " << maxFlow << std::endl;
    return maxFlow;
}

void PushRelabel::PrintGraph(){
    for (int i = 1; i<= n_; i++){
        for (int j = 1; j <= n_; j++){
            std::cout << matrix_[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

PushRelabel::~PushRelabel(){
    for (int i = 0; i <= n_; i++){
        delete [] matrix_[i];
    }
    delete [] matrix_;
    delete [] points_;
}

void PushRelabel::InitPoint(){
    for (int i = 1; i <= n_; i++){
        points_[i].id = i;
        points_[i].height = 0;
        points_[i].flow = 0;
    }
}
