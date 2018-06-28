//
//  union_find.cpp
//  GraphAlgorithm
//
//  Created by songjian zhang on 2017/7/11.
//  Copyright © 2017年 songjian zhang. All rights reserved.
//

#include "union_find.hpp"


UnionFind::UnionFind(int n) : count_(n){
    id_ = new int[n];
    sz_ = new int[n];
    
    for (int i = 0; i < n; i++){
        id_[i] = i;
    }
    
    for (int i = 0; i < n; i++){
        sz_[i] = 1;
    }
    
}

UnionFind::~UnionFind(){
    delete[] id_;
    delete[] sz_;
}

int UnionFind::Find(int a){
    /*while(id_[a] != id_[id_[a]]){
        id_[a] = id_[id_[a]];
    }*/
    //Compress path
    while(a != id_[a]){
        id_[a] = id_[id_[a]];
        a = id_[a];
    }
    
    return a;
}

void UnionFind::Union(int a, int b){
    int parent1 = id_[a];
    int parent2 = id_[b];
    
    if (parent1 == parent2){
        return;
    }
    
    if (sz_[parent1] >= sz_[parent2]){
        sz_[parent1] += sz_[parent2];
        id_[parent2] = id_[parent1];
    }else{
        sz_[parent2] += sz_[parent1];
        id_[parent1] = id_[parent2];
    }
}

bool UnionFind::IsConnected(int a, int b){
    return Find(a) == Find(b);
}

int UnionFind::count(){
    return count_;
}
