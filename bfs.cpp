#include "bfs.h"
 
#include <list>
#include <set>
#include <unordered_set>
#include <algorithm>

std::shared_ptr<const state> bfs(std::shared_ptr<const state> root){
 
    std::unordered_set<unsigned long long> visit;
 
    std::vector<std::shared_ptr<const state>> queue;
    std::vector<std::shared_ptr<const state>> queue2;
 
    std::shared_ptr<const state> goalState = nullptr;
 
    queue.push_back(root);
    while(!queue.empty()){
#pragma omp parallel for
            for (long long unsigned int i = 0; i < queue.size(); i++) {
                std::shared_ptr<const state> vert = queue[i];
                bool ifn = true;
#pragma omp critical
                {
                    ifn = visit.find(vert->get_identifier()) == visit.end();
 
                }
                if(ifn) {
                    std::vector<std::shared_ptr<const state>> neighbors = vert->next_states();
#pragma omp critical
                    {
                        visit.insert(vert->get_identifier());
                    }
//#pragma omp parallel for schedule(dynamic)
                    for (long long unsigned int j = 0; j < neighbors.size(); j++) {
                        std::shared_ptr<const state> neighb = neighbors[j];
                        //if(visit.find(neighb) == visit.end()){
//#pragma omp cancellation point for
                        //if (neighb->is_goal()) {
                            //goalState = neighb;
//#pragma omp cancel for
                        //}
#pragma omp critical
                        {
                            queue2.push_back(neighb);
                        }
                    }
                }
            }
            for(long long unsigned int o = 0; o < queue2.size(); o++){
                if(queue2[o]->is_goal()) {
                    if (goalState != nullptr) {
                        if (goalState->get_identifier() > queue2[o]->get_identifier()){
                            goalState = queue2[o];
                        }
                    } else {
                        goalState = queue2[o];
                    }
                }
            }
            if(goalState != nullptr){
                return goalState;
            }
 
            queue.clear();
            queue = queue2;
            queue2.clear();
    }
    return goalState;
}
