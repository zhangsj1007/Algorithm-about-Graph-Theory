//
//  GraphAlgorithm.cpp
//  GraphAlgorithm
//
//  Created by songjian zhang on 2017/7/4.
//  Copyright © 2017年 songjian zhang. All rights reserved.
//

#include "graph_algorithm.hpp"
#include "graph.hpp"
#include "union_find.hpp"
#include <queue>
#include <iostream>
#include <set>
#include <limits.h>
#include <stack>
#include <algorithm>
#include <map>



void GraphAlgorithm::Bfs(Graph* graph, int start){
    std::cout << "graph bfs result : ";
    bool* visited = new bool[graph->vernum_ + 1];
    int* parent = new int[graph->vernum_ + 1];
    std::queue<uint32_t> q;
    for (int i = 1; i <= graph->vernum_; i++){
        visited[i] = false;
    }
    
    for (int i = 1; i <= graph->vernum_; i++){
        parent[i] = -1;
    }
    
    Visit(start);
    visited[start] = true;
    q.push(start);
    while(!q.empty()){
        int vernode = q.front();
        ArcNode* arc = graph->vers_[vernode].firstArc;
        while(arc != NULL){
            if(!visited[arc->tail]){
                Visit(arc->tail);
                visited[arc->tail] = true;
                parent[arc->tail] = vernode;
                q.push(arc->tail);
            }
            arc = arc->nextArc;
        }
        q.pop();
    }
    delete[] visited;
    delete[] parent;
    std::cout << std::endl;
}

void GraphAlgorithm::Dfs(Graph* graph, int start){
    std::cout << "graph dfs result : ";
    bool* visited = new bool[graph->vernum_ + 1];
    
    for (int i = 1; i <= graph->vernum_; i++){
        visited[i] = false;
    }
    
    for (int i = start; i <= graph->vernum_; i++){
        DfsHelper(graph, i, visited);
    }
    for (int i = start-1; i>=1; i--){
        DfsHelper(graph, i, visited);
    }
    delete[] visited;
    std::cout << std::endl;
}


void GraphAlgorithm::Dijkstra(Graph* graph, int start, int end){
    std::cout << "Dijkstra Algorithm (" << start << "->" << end << "):";
    std::vector<int> dist(graph->vernum_ + 1, INT_MAX);
    std::vector<bool> joinSet(graph->vernum_ + 1, false);
    std::vector<int> parent(graph->vernum_ + 1, start);
    int minVer = 0;
    
    joinSet[start] = true;
    minVer = DijkstraInit(graph, start, dist, parent);
    joinSet[minVer] = true;
    while (minVer != end && minVer != 0){
        minVer = DijkstraUpdate(graph, minVer, dist, joinSet, parent);
    }
    
    if (minVer == end){
        PrintPath(parent, start, end);
        std::cout << ", shortest length = " << dist[end] << "." << std::endl;
    }else{
        std::cout << "Find shortest path failed." << std::endl;
    }
}

void GraphAlgorithm::Dijkstra2(Graph* graph, int start, int end){
    std::cout << "Accelerating Dijkstra Algorithm (" << start << "->" << end << ") : ";
    std::priority_queue<iPair, std::vector<iPair>, std::greater<iPair>> priorQue;
    std::vector<int> dist(graph->vernum_+1, INT_MAX);
    std::vector<int> parent(graph->vernum_+1, start);
    std::vector<bool> join(graph->vernum_+1, false);
    bool find = false;
    dist[start] = 0;
    join[start] = true;
    priorQue.push(std::make_pair(0, start));
    while(!priorQue.empty()){
        int ver = priorQue.top().second;
        priorQue.pop();
        if (join[ver]){
            continue;
        }
        join[ver] = true;
        if (ver == end){
            find = true;
            break;
        }
        DijkstraUpdate(graph, ver, priorQue, dist, parent, join);
    }
    if (find){
        PrintPath(parent, start, end);
        std::cout << ", shortest length = " << dist[end] << "." << std::endl;
    }else{
        std::cout << "Find shortest path failed." << std::endl;
    }
}

void GraphAlgorithm::DijkstraWithRugularPath(Graph* graph){
    PriorityQueue priorQue;
    ArcNode* parents[graph->vernum_+1];
    std::map<uint32_t, std::map<uint32_t, uint32_t>> dists = graph->dists_;
    //parents用来追踪path
    for (int i = 0; i <= graph->vernum_; i++){
        parents[i] = NULL;
    }
    //初始化原点的dists
    for (std::map<uint32_t,uint32_t>::iterator it = dists[1].begin(); it != dists[1].end(); it++){
        it->second = 0;
    }
    
    ArcNode dummy(0, 1, 0, 0);
    priorQue.push(std::make_pair(0, &dummy));
    while(!priorQue.empty()){
        ArcNode* arc = priorQue.top().second;
        priorQue.pop();
        DijkstraUpdate2(graph, arc, priorQue, dists, parents);
    }
    for (int i = 2; i <= 5; i++){
        std::cout << i << "'s path: ";
        ArcNode* p = parents[i];
        std::stack<ArcNode*> path;
        while (p->head != 0){
            path.push(p);
            p = p->preArc;
        }
        while(!path.empty()){
            ArcNode* q = path.top();
            path.pop();
            std::cout << q->head << "->";
        }
        std::cout << i << std::endl;
    }
    std::cout << std::endl;
}

void GraphAlgorithm::BiDijkstra(Graph* graph, int start, int end){
    std::cout << "BiDijkstra Algorithm (" << start << "->" << end << ") : ";
    
    // for start
    std::vector<int> dist1(graph->vernum_ + 1, INT_MAX);
    std::vector<bool> joinSet1(graph->vernum_ + 1, false);
    std::vector<bool> unJoinSet1(graph->vernum_ + 1, true);
    std::vector<int> parent1(graph->vernum_ + 1, start);
    int minVer1 = 0;
    
    joinSet1[start] = true;
    unJoinSet1[start] = false;
    minVer1 = DijkstraInit(graph, start, dist1, parent1);
    joinSet1[minVer1] = true;
    unJoinSet1[minVer1] = false;
    
    
    //for end
    std::vector<int> dist2(graph->vernum_ + 1, INT_MAX);
    std::vector<bool> joinSet2(graph->vernum_ + 1, false);
    std::vector<bool> unJoinSet2(graph->vernum_ + 1, true);
    std::vector<int> parent2(graph->vernum_ + 1, end);
    int minVer2 = 0;
    
    joinSet2[end] = true;
    unJoinSet2[end] = false;
    minVer2 = DijkstraInit(graph, end, dist2, parent2);
    joinSet2[minVer2] = true;
    unJoinSet2[minVer2] = false;
    
    int minVer = 0;
    bool find = false;
    
    if (minVer1 == end || minVer2 == start){
        int length = std::min(dist1[minVer1], dist2[minVer2]);
        std::cout << start << "->" << end << ", shortest length = " << length << "." << std::endl;
        return;
    }else if(minVer1 == minVer2){
        int length = dist1[minVer1] + dist2[minVer2];
        std::cout << start << "->" << minVer1 << "->" << end << ", shortest length = " << length << std::endl;
        return;
    }
    
    while(true){
        minVer1 = DijkstraUpdate(graph, minVer1, dist1, joinSet1, parent1);
        if (minVer1 == 0){
            find = false;
            break;
        }else if (joinSet2[minVer1]){
            //...
            find = true;
            minVer = minVer1;
            break;
        }
        minVer2 = DijkstraUpdate(graph, minVer2, dist2, joinSet2, parent2);
        if (minVer2 == 0){
            find = false;
            break;
        }else if (joinSet1[minVer2]){
            //...
            find = true;
            minVer = minVer2;
            break;
        }
    }
    
    if (find){
        PrintPath(start, end, minVer, parent1, parent2);
        std::cout << ", shortest length = " << dist1[minVer] + dist2[minVer] << std::endl;
    }else{
        std::cout << "Find shortest path failed." << std::endl;
    }
}

void GraphAlgorithm::BiDijkstra2(Graph* graph, int start, int end){
    std::cout << "Accelerating BiDijkstra Algorithm (" << start << "->" << end << ") : ";
    using iPair = std::pair<int, int>;
    std::priority_queue<iPair, std::vector<iPair>, std::greater<iPair>> priorQue1;
    std::priority_queue<iPair, std::vector<iPair>, std::greater<iPair>> priorQue2;
    std::vector<int> dist1(graph->vernum_+1, INT_MAX);
    std::vector<int> dist2(graph->vernum_+1, INT_MAX);
    std::vector<int> parent1(graph->vernum_+1, start);
    std::vector<int> parent2(graph->vernum_+1, end);
    std::vector<bool> joinSet1(graph->vernum_+1, false);
    std::vector<bool> joinSet2(graph->vernum_+1, false);
    bool find = false;
    int ver1 = 0;
    int ver2 = 0;
    int ver = 0;
    dist1[start] = 0;
    dist2[end] = 0;
    joinSet1[start] = true;
    joinSet2[end] = true;
    priorQue1.push(std::make_pair(0, start));
    priorQue2.push(std::make_pair(0, end));
    
    while(!priorQue1.empty() && !priorQue2.empty()){
        ver1 = priorQue1.top().second;
        priorQue1.pop();
        ver1 = DijkstraUpdate(graph, ver1, priorQue1, dist1, parent1);
        joinSet1[ver1]  = true;
        if (joinSet2[ver1]){
            ver = ver1;
            find = true;
            break;
        }
        ver2 = priorQue2.top().second;
        priorQue2.pop();
        ver2 = DijkstraUpdate(graph, ver2, priorQue2, dist2, parent2);
        joinSet2[ver2] = true;
        if (joinSet1[ver2]){
            ver = ver2;
            find = true;
            break;
        }
    }
    if (find){
        PrintPath(start, end, ver, parent1, parent2);
        std::cout << ", shortest length = " << dist1[ver] + dist2[ver] << std::endl;
    }else{
        std::cout << "Find shortest path failed." << std::endl;
    }
}

void GraphAlgorithm::Prime(Graph* graph){
    std::cout << "Prime Algorithm : ";
    std::vector<bool> joinSet(graph->vernum_ + 1, false);
    std::vector<int> dist(graph->vernum_ + 1, INT_MAX);
    std::vector<int> parent(graph->vernum_ + 1, 0);
    std::priority_queue<iPair, std::vector<iPair>, std::greater<iPair>> priorQue;
    bool find = true;
    
    dist[1] = 0;
    priorQue.push(std::make_pair(0, 1));
    while(!priorQue.empty()){
        int ver = priorQue.top().second;
        priorQue.pop();
        
        if (!joinSet[ver]){
            joinSet[ver] = true;
        }else{
            continue;
        }
        
        ArcNode* arc = graph->vers_[ver].firstArc;
        while(arc != NULL){
            if (dist[arc->tail] > arc->weight && !joinSet[arc->tail]){
                dist[arc->tail] = arc->weight;
                parent[arc->tail] = ver;
                priorQue.push(std::make_pair(arc->weight, arc->tail));
            }
            arc = arc->nextArc;
        }
    }
    
    for (int i = 1; i <= graph->vernum_; i++){
        if (!joinSet[i]){
            find = false;
        }
    }
    
    if (find){
        for (int i = 1; i <= graph->vernum_; i++){
            std::cout << i << "(" << parent[i] << ")" << " ";
        }
        std::cout << std::endl;
    }else{
        std::cout << "there doesn't exit a generated tree." << std::endl;
    }
}

void GraphAlgorithm::Kruskal(Graph* graph){
    std::cout << "Kruskal Algorithm : ";
    std::priority_queue<ArcNode*, std::vector<ArcNode*>, std::greater<ArcNode*>> priorQ;
    std::stack<std::pair<int, int>> tree;
    UnionFind uf(graph->vernum_);
    int count = graph->vernum_ - 1;
    for (int i = 1; i <= graph->vernum_; i++){
        for(ArcNode* arcNode = graph->vers_[i].firstArc; arcNode != NULL ; arcNode = arcNode->nextArc){
            priorQ.push(arcNode);
        }
    }
    
    tree.push(std::make_pair(0, 1));
    
    while(count > 0 && !priorQ.empty()){
        ArcNode* arc = priorQ.top();
        priorQ.pop();
        if (!uf.IsConnected(arc->head-1, arc->tail-1)){
            uf.Union(arc->head-1, arc->tail-1);
            tree.push(std::make_pair(arc->head, arc->tail));
            count--;
        }
    }
    if (count > 0){
        std::cout << "there doesn't exit a generated tree." << std::endl;
    }else{
        while(!tree.empty()){
            std::pair<int, int> branch = tree.top();
            tree.pop();
            std::cout << branch.second << "<->" << branch.first << "  ";
        }
        std::cout << std::endl;
    }
}

void GraphAlgorithm::KahnTopological(Graph* graph){
    std::cout << "KahnTopological Algorithm : ";
    std::queue<int> q;
    std::queue<int> setOf0degrees;
    std::vector<int> indegrees(graph->vernum_+1, 0);
    int count = graph->vernum_;
    indegrees[0] = INT_MAX;
    for (int i = 1; i <= graph->vernum_; i++){
        for (ArcNode* arc = graph->vers_[i].firstArc; arc != NULL; arc = arc->nextArc){
            indegrees[arc->tail]++;
        }
    }
    
    for (int i = 1; i <= graph->vernum_; i++){
        if (indegrees[i] == 0){
            setOf0degrees.push(i);
        }
    }

    while (!setOf0degrees.empty()){
        int ver = setOf0degrees.front();
        setOf0degrees.pop();
        q.push(ver);
        for (ArcNode* arc = graph->vers_[ver].firstArc; arc != NULL; arc = arc->nextArc){
            if (--indegrees[arc->tail] == 0){
                setOf0degrees.push(arc->tail);
            }
        }
        count--;
    }
    
    if (count == 0){
        std::cout << q.front();
        q.pop();
        while(!q.empty()){
            std::cout << " -> ";
            std::cout << q.front();
            q.pop();
        }
        std::cout << std::endl;
    }else{
        std::cout << "KahnTopological sort failed, there may be a recycle in the graph." << std::endl;
    }
}

void GraphAlgorithm::Visit(int vernode){
    std::cout << vernode << "  ";
}

void GraphAlgorithm::DfsHelper(Graph* graph, int start, bool* visited){
    if (!visited[start]){
        Visit(start);
        visited[start] = true;
        ArcNode* arc = graph->vers_[start].firstArc;
        while(arc != NULL){
            DfsHelper(graph, arc->tail, visited);
            arc = arc->nextArc;
        }
    }
}

int GraphAlgorithm::DijkstraInit(Graph* graph, int start, std::vector<int>& dist, std::vector<int>& parent){
    int min = INT_MAX;
    int minVer = 0;
    ArcNode* arc = graph->vers_[start].firstArc;
    while (arc != NULL){
        dist[arc->tail] = arc->weight;
        if (arc->weight < min){
            min = arc->weight;
            minVer = arc->tail;
        }
        arc = arc->nextArc;
    }
    return minVer;
}

int GraphAlgorithm::DijkstraUpdate(Graph* graph, int minVer, std::vector<int>& dist, std::vector<bool>& joinSet,
                                std::vector<int>& parent){
    int minVerNext = 0;
    int min = INT_MAX;
    for (int i = 1; i <= graph->vernum_; i++){
        if (joinSet[i]){
            continue;
        }
        
        int weight = graph->GetWeight(minVer, i);
        if (weight != INT_MAX){
            int updateWeight = graph->GetWeight(minVer, i) + dist[minVer];
            if (dist[i] > updateWeight){
                dist[i] = updateWeight;
                parent[i] = minVer;
            }

        }
        if (dist[i] < min){
            minVerNext = i;
            min = dist[i];
        }
    }
    joinSet[minVerNext] = true;
    return minVerNext;
}

int GraphAlgorithm::DijkstraUpdate(Graph* graph, int ver, std::priority_queue<iPair, std::vector<iPair>, std::greater<iPair>>& priorQue, std::vector<int>& dist, std::vector<int>& parent, std::vector<bool> join){
    ArcNode* arc = graph->vers_[ver].firstArc;
    while(arc != NULL){
        if (!join[arc->tail] && (dist[arc->tail] == INT_MAX || dist[arc->tail] > dist[ver] + arc->weight)){
            dist[arc->tail] = dist[ver] + arc->weight;
            priorQue.push(std::make_pair(dist[arc->tail], arc->tail));
            parent[arc->tail] = ver;
        }
        arc = arc->nextArc;
    }
    
    if (priorQue.empty()){
        return 0;
    }else{
        return priorQue.top().second;
    }
}

ArcNode* GraphAlgorithm::DijkstraUpdate2(Graph* graph, ArcNode* select, PriorityQueue& priorQue, DistanceMap& dists,
ArcNode* parents[]){
    int ver = select->tail;
    if (parents[ver] == NULL){
        parents[ver] = select;
    }

    ArcNode* arc = graph->vers_[ver].firstArc;
    while(arc != NULL){
        if (select->grade <= arc->grade){
            if (dists[arc->tail][arc->id] == INT_MAX || dists[arc->tail][arc->id] > dists[select->tail][select->id] + arc->weight){
                dists[arc->tail][arc->id] = dists[select->tail][select->id] + arc->weight;
                arc->preArc = select;
                priorQue.push(std::make_pair(arc->weight, arc));
            }
            
        }
        arc = arc->nextArc;
    }
    
    if (priorQue.empty()){
        return NULL;
    }else{
        return priorQue.top().second;
    }
}

void GraphAlgorithm::PrintPath(std::vector<int>& parent, int start, int end){
    std::stack<int> stk;
    int ver = end;
    while(parent[ver] != start){
        stk.push(parent[ver]);
        ver = parent[ver];
    }
    std::cout << start;
    while(!stk.empty()){
        std::cout << "->" << stk.top();
        stk.pop();
    }
    std::cout << "->" << end;
}

void GraphAlgorithm::PrintPath(int start, int end, int minVer, std::vector<int>& parent1, std::vector<int>& parent2){
    PrintPath(parent1, start, minVer);
    int ver = minVer;
    while(parent2[ver] != end){
        std::cout << "->" << parent2[ver];
        ver = parent2[ver];
    }
    std::cout << "->" << end;
}
