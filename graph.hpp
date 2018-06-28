//
//  Graph.hpp
//  GraphAlgorithm
//
//  Created by songjian zhang on 2017/7/4.
//  Copyright © 2017年 songjian zhang. All rights reserved.
//

#ifndef Graph_hpp
#define Graph_hpp

#include <stdio.h>
#include <vector>
#include <map>

#define MAXVER 100        //最大的顶点数

enum GraphType{
    WithoutWeightWithoutDirection = 0,
    WithoutWeightWithDirection,
    WithWeightWithDirection,
    WithWeightWithoutDirection
};

struct ArcNode{
    unsigned int head;
    unsigned int tail;
    unsigned int weight;
    unsigned int grade;
    unsigned int id;
    ArcNode* preArc;
    ArcNode* nextArc;
    ArcNode(unsigned int h, unsigned int t, unsigned int w, unsigned int g): head(h), tail(t), weight(w), grade(g), preArc(NULL), nextArc(NULL){
    
    }
    
    inline bool operator > (const ArcNode& arc) const {
        return weight > arc.weight;
    }
};

/*struct ArcNode{
    int vertex_;
    int weight_;
    int grade;
    ArcNode* nextArc_;
    ArcNode(int v, int w = 0): vertex_(v), weight_(w), grade(0), nextArc_(NULL){}
};*/

struct VerNode{
    int vertex;
    ArcNode* firstArc = NULL;
    //VerNode* parent_ = NULL;
};

class GraphAlgorithm;

class Graph{
    friend class GraphAlgorithm;
    friend class FordFulkerson;
public:
    Graph(GraphType graphType, uint32_t vernum_, uint32_t arcnum);
    void Init();
    void InsertArc(uint32_t h, uint32_t t, uint32_t w, uint32_t g);
    //void DeleteArc(int a, int b);
    uint32_t GetWeight(uint32_t h, uint32_t t);
    uint32_t** GetMtrix();
    void PrintGraph();
    void PrintMatrix();
    static VerNode* GenerateNodeArray(uint32_t** matrix, uint32_t n);
    static void DestroyNodeArray(VerNode* vers, int n);
private:
    void InsertArcHelp(uint32_t a, uint32_t b, uint32_t w, uint32_t g);
    static ArcNode* InsertArcHelp(VerNode* vers, uint32_t a, uint32_t b, uint32_t w, uint32_t g);
    
private:
    GraphType graphType_;
    uint32_t vernum_;
    uint32_t arcnum_;
    VerNode vers_[MAXVER];
    uint32_t count_ = 1;
    std::vector<ArcNode*> arcs_;
    std::map<uint32_t, std::map<uint32_t, uint32_t>> dists_;
    uint32_t** matrix_ = NULL;
};

#endif /* Graph_hpp */
