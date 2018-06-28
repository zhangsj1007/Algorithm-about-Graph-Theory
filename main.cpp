//
//  main.cpp
//  GraphAlgorithm
//
//  Created by songjian zhang on 2017/7/2.
//  Copyright © 2017年 songjian zhang. All rights reserved.
//

#include <iostream>
#include "graph.hpp"
#include "graph_algorithm.hpp"
#include "union_find.hpp"
#include "test.hpp"

using namespace std;

//WithoutWeightWithoutDirectionCase
int case1[][2] = {
    {1, 2},
    {1, 4},
    {1, 5},
    {2, 3},
    {2, 4},
    {3, 4},
    {4, 5}
};

//WithWeightWithDirectionCase
int case2[][3] = {
    {1, 2, 3},
    {1, 4, 5},
    {1, 5, 15},
    {2, 3, 9},
    {2, 4, 7},
    {3, 4, 11},
    {4, 5, 13}
};

//WithWeightWithoutDirectionCase
int case3[][3] = {
    {1, 2, 3},
    {1, 4, 5},
    {1, 5, 15},
    {2, 3, 17},
    {2, 4, 7},
    {3, 4, 11},
    {4, 5 ,13}
};

//WithWeightWithDirectionCase
int case4[][3] = {
    {1, 2, 3},
    {1, 4, 5},
    {2, 3, 9},
    {3, 4, 11},
    {4, 5, 13}
};

//WithWeightWithDirectionCase, test Ford-Fulkerson
int case5[][3] = {
    {1, 2, 1},
    {1, 4, 4},
    {2, 3, 1},
    {2, 5, 3},
    {3, 2, 1},
    {3, 6, 1},
    {4, 3, 3},
    {4, 5, 1},
    {5, 4, 1},
    {5, 6, 4}
};

//WithWeightWithDirectionCase, test two-divided-match use Ford-Furkerson
int case6[][3] = {
    {1, 6, 1},
    {2, 6, 1},
    {2, 7, 1},
    {3, 7, 1},
    {4, 6, 1},
    {4, 8, 1},
    {4, 9, 1},
    {5, 8, 1},
    {5, 9, 1}
};

//WithWeightWithDirectionCase and with grade
int case7[][4] = {
    {1, 2, 3, 5},
    {1, 3, 5, 3},
    {2, 4, 2, 3},
    {2, 5, 8, 8},
    {3, 2, 5, 3}
};

int main(int argc, const char * argv[]) {
    Test test(GraphType::WithWeightWithDirection, case7, 5, 5);
    test.TestDijkstraWithRugularPath();
    
    return 0;
}
