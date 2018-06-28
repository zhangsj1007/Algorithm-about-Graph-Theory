//
//  union_find.hpp
//  GraphAlgorithm
//
//  Created by songjian zhang on 2017/7/11.
//  Copyright © 2017年 songjian zhang. All rights reserved.
//

#ifndef union_find_hpp
#define union_find_hpp

#include <stdio.h>

class UnionFind{
public:
    UnionFind(int n);
    virtual ~UnionFind();
    int Find(int a);
    void Union(int a, int b);
    bool IsConnected(int a, int b);
    int count();
private:
    int* id_ = NULL;
    int* sz_ = NULL;
    int count_;
};

#endif /* union_find_hpp */
