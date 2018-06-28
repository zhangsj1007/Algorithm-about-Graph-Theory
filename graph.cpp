//
//  Graph.cpp
//  GraphAlgorithm
//
//  Created by songjian zhang on 2017/7/4.
//  Copyright © 2017年 songjian zhang. All rights reserved.
//

#include "graph.hpp"
#include <iostream>
#include <limits.h>

using namespace std;

Graph::Graph(GraphType graphType, uint32_t vernum, uint32_t arcnum): graphType_(graphType), vernum_(vernum), arcnum_(arcnum){
    arcs_.resize(vernum+1);
}

void Graph::Init(){
    for (int i = 1; i <= vernum_; i++){
        vers_[i].vertex = i;
        vers_[i].firstArc = NULL;
    }
}

void Graph::InsertArc(uint32_t h, uint32_t t,  uint32_t w, uint32_t g){
    if (graphType_ == GraphType::WithoutWeightWithoutDirection){
        InsertArcHelp(h, t, 0, g);
        InsertArcHelp(t, h, 0, g);
    }else if (graphType_ == GraphType::WithoutWeightWithDirection){
        InsertArcHelp(h, t, 0, g);
    }else if (graphType_ == GraphType::WithWeightWithDirection){
        InsertArcHelp(h, t, w, g);
    }else{
        InsertArcHelp(h, t, w, g);
        InsertArcHelp(t, h, w, g);
    }
}

void Graph::InsertArcHelp(uint32_t a, uint32_t b, uint32_t w, uint32_t g){
    ArcNode* q = InsertArcHelp(vers_, a, b, w, g);
    arcs_[count_] = q;
    q->id = count_;
    dists_[b][count_] = INT_MAX;
    count_++;
}

ArcNode* Graph::InsertArcHelp(VerNode* vers, uint32_t a, uint32_t b, uint32_t w, uint32_t g){
    ArcNode* q = new ArcNode(a, b, w, g);
    if (vers[a].firstArc == NULL){
        vers[a].firstArc = q;
        return  q;
    }else{
        //边的插入条件：如果有重复边的条件，如果grade不相等，直接插入该边，如果grade相等，选取权重最小的边
        ArcNode* p = vers[a].firstArc;
        
        if (p->tail == b){
            if (p->grade != g){
                q->nextArc = p->nextArc;
                q->nextArc = p;
                return q;
            }else{
                if (p->weight < w){
                    p->weight = w;
                }
                delete q;
                return p;
            }
        }
        
        while(p->nextArc != NULL){
            if (p->nextArc->tail == b){
                if (p->nextArc->grade != g){
                    q->nextArc = p->nextArc->nextArc;
                    q->nextArc = p;
                    return q;
                }else{
                    if (p->nextArc->weight < w){
                        p->nextArc->weight = w;
                    }
                    delete  q;
                    return p;
                }
            }
            p = p->nextArc;
        }
        p->nextArc = q;
        return q;
    }
}

/*void Graph::DeleteArc(int a, int b){
    ArcNode* p = vers_[a].firstArc_;
    if (p->vertex_ == b){
        vers_[a].firstArc_ = p->nextArc_;
        delete p;
        return;
    }
    while(p->nextArc_ != NULL){
        if (p->nextArc_->vertex_ == b){
            ArcNode* q = p->nextArc_;
            p->nextArc_ = q->nextArc_;
            delete q;
            return;
        }
    }
}*/

uint32_t Graph::GetWeight(uint32_t h, uint32_t t){
    ArcNode* arc = vers_[h].firstArc;
    while(arc != NULL){
        if (arc->tail == t){
            return arc->weight;
        }
        arc = arc->nextArc;
    }
    return INT_MAX;
}

uint32_t** Graph::GetMtrix(){
    if (matrix_ != NULL){
        return matrix_;
    }
    
    matrix_ = new uint32_t*[vernum_+1];
    
    for (int i = 0; i <= vernum_; i++){
        matrix_[i] = new uint32_t[vernum_+1];
    }
    
    for(int i = 0; i <= vernum_; i++){
        for (int j = 0; j <= vernum_; j++){
            matrix_[i][j] = 0;
        }
    }
    
    for (int i = 1; i <= vernum_; i++){
        ArcNode* arc = vers_[i].firstArc;
        while(arc != NULL){
            matrix_[i][arc->tail] = arc->weight;
            arc = arc->nextArc;
        }
    }
    
    return matrix_;
}

void Graph::PrintGraph(){
    for (unsigned i = 1; i <= vernum_; i++){
        cout << vers_[i].vertex;
        ArcNode* p = vers_[i].firstArc;
        while(p != NULL){
            cout << "->(" << p->tail << "," << p->weight << ")";
            p = p->nextArc;
        }
        cout << "->NULL" << endl;
    }
}


void Graph::PrintMatrix(){
    for (unsigned i = 1; i <= vernum_ ; i++){
        for (unsigned j = 1; j <= vernum_; j++){
            std::cout << matrix_[i][j] << "  ";
        }
        std::cout << std::endl;
    }
}

VerNode* Graph::GenerateNodeArray(uint32_t** matrix, uint32_t n){
    VerNode* vers = new VerNode[n+1];
    
    for (int i = 1; i <= n; i++){
        vers[i].vertex = i;
        vers[i].firstArc = NULL;
    }
    //忽略0
    for (int i = 1; i <= n; i++){
        for (int j = 1; j<= n; j++){
            if (i == j || matrix[i][j] == 0){
                continue;
            }
            //由matrix->vers,默认使用grade=0
            InsertArcHelp(vers, i, j, 0, matrix[i][j]);
        }
    }
    return vers;
}

void Graph::DestroyNodeArray(VerNode* vers, int n){
    for (int i = 1; i <= n; i++){
        ArcNode* arc = vers[i].firstArc;
        while(arc != NULL){
            ArcNode* nextArc = arc->nextArc;
            delete arc;
            arc = nextArc;
        }
    }
    delete[] vers;
}
