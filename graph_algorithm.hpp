//
//  GraphAlgorithm.hpp
//  GraphAlgorithm
//
//  Created by songjian zhang on 2017/7/4.
//  Copyright © 2017年 songjian zhang. All rights reserved.
//

#ifndef GraphAlgorithm_hpp
#define GraphAlgorithm_hpp

#include <stdio.h>
#include "graph.hpp"
#include <vector>
#include <queue>
#include <map>

class GraphAlgorithm{
public:
    
    /*struct Arc{
        uint32_t a;
        uint32_t b;
        uint32_t weigh;
        uint32_t grade;
        uint32_t id;
        Arc(uint32_t a, uint32_t b, uint32_t weight, uint32_t g = 0): a(a), b(b), weigh(weight), grade(g){
        
        }
        inline bool operator > (const Arc& arc) const {
            return weigh > arc.weigh;
        }
    };*/
    
    using iPair = std::pair<int, int>;
    using PriorityQueue=std::priority_queue<std::pair<int, ArcNode*>, std::vector<std::pair<int, ArcNode*>>, std::greater<std::pair<int, ArcNode*>>>;
    //vertex, arc, distance
    using DistanceMap = std::map<uint32_t, std::map<uint32_t, uint32_t>>;
    void Bfs(Graph* graph, int start);
    void Dfs(Graph* graph, int start);
    void Dijkstra(Graph* graph, int start, int end);
    //priority_queue + Dijkstra
    void Dijkstra2(Graph* graph, int start, int end);
    //priority_queue + Dijkstra + regular path + full pathes
    void DijkstraWithRugularPath(Graph* graph);
    void BiDijkstra(Graph* graph, int start, int end);
    //priority_queue + Dijkstra
    void BiDijkstra2(Graph* graph, int start, int end);
    //priority_queue + Prime
    void Prime(Graph* graph);
    //priority_queue + Prime
    void Kruskal(Graph* graph);
    void KahnTopological(Graph* graph);
private:
    void Visit(int vernode);
    void DfsHelper(Graph* graph, int start, bool* Visited);
    int DijkstraInit(Graph* graph, int start, std::vector<int>& dist, std::vector<int>& parent);
    int DijkstraUpdate(Graph* graph, int minVer, std::vector<int>& dist, std::vector<bool>& joinSet, std::vector<int>& parent);
    int DijkstraUpdate(Graph* graph, int ver, std::priority_queue<iPair, std::vector<iPair>, std::greater<iPair>>& priorQue, std::vector<int>& dist, std::vector<int>& parent, std::vector<bool> join);
    ArcNode* DijkstraUpdate2(Graph* graph, ArcNode* select, PriorityQueue& priorQue, DistanceMap& dists, ArcNode* parent[]);
    void PrintPath(std::vector<int>& parent, int start, int end);
    void PrintPath(int start, int end, int minver, std::vector<int>& parent1, std::vector<int>& parent2);
};

#endif /* GraphAlgorithm_hpp */
