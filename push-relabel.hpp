//
//  push-relabel.hpp
//  GraphAlgorithm
//
//  Created by songjian zhang on 2017/7/24.
//  Copyright © 2017年 songjian zhang. All rights reserved.
//

#ifndef push_relabel_hpp
#define push_relabel_hpp

#include <stdio.h>
#include <cstdint>

struct Point{
    int id;
    int height;
    uint32_t flow;
};

class PushRelabel{
public:
    PushRelabel(uint32_t* matrix , int n);
    int CalculateFlow();
    void PrintGraph();
    ~PushRelabel();
private:
    void InitPoint();
private:
    int n_;
    uint32_t** matrix_ = NULL;
    Point* points_ = NULL;
};


#endif /* push_relabel_hpp */
