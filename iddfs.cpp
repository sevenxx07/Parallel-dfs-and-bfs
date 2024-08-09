#include "iddfs.h"
 
#include <map>
#include <list>
#include <unordered_set>
#include <iostream>
#include <unordered_set>
#include <experimental/iterator>

std::shared_ptr<const state> dls_seq(std::shared_ptr<const state> root, int level) {
    if(root->is_goal()){
        return root;
    }
    if (level <= 0){
        return nullptr;
    }
    std::vector<std::shared_ptr<const state>> neighbors = root->next_states();
    std::shared_ptr<const state> goalState;
    std::vector<std::shared_ptr<const state>> goalStates(neighbors.size());
    for(long long unsigned int j = 0; j < neighbors.size(); j++) {
        std::shared_ptr<const state> neighb = neighbors[j];
        goalStates[j] = dls_seq(neighb, level-1);
    }
    for(long long unsigned int p = 0; p < goalStates.size(); p++){
        if(goalStates[p] != nullptr){
            if(goalState != nullptr){
                if(goalState->get_identifier() > goalStates[p]->get_identifier()){
                    goalState = goalStates[p];
                }
            }else{
                goalState = goalStates[p];
            }
        }
    }
    return goalState;
}
 
std::shared_ptr<const state> dls(std::shared_ptr<const state> root, int level, std::unordered_set<unsigned long long>& visit) {
    if(level <= 3) {
        return dls_seq(root, level);
    }
    std::vector<std::shared_ptr<const state>> neighbors = root->next_states();
    std::vector<std::shared_ptr<const state>> goalStates(neighbors.size());
    std::vector<std::unordered_set<unsigned long long>> visits(neighbors.size());
    std::shared_ptr<const state> goalState = nullptr;
    for(long long unsigned int j = 0; j < neighbors.size(); j++) {
        std::shared_ptr<const state> neihgb = neighbors[j];
        std::shared_ptr<const state> mgoal;
        bool ifn = true;
#pragma omp critical
        {
            ifn = visit.find(neihgb->get_identifier()) == visit.end();
 
        }
        if (ifn) {
#pragma omp critical
            {
                visit.insert(neihgb->get_identifier());
                visits[j] = visit;
 
            }
#pragma omp task shared(goalStates)
            {
                goalStates[j] = dls(neihgb, level - 1, visits[j]);
            }
        }
    }
#pragma omp taskwait
    for(long long unsigned int p = 0; p < goalStates.size(); p++){
        if(goalStates[p] != nullptr){
            if(goalState != nullptr){
                if(goalState->get_identifier() > goalStates[p]->get_identifier()){
                    goalState = goalStates[p];
                }
            }else{
                goalState = goalStates[p];
            }
        }
    }
    for(std::unordered_set<unsigned long long> vi : visits){
        for(unsigned long long v : vi){
            visit.insert(v);
        }
    }
    return goalState;
}
 
std::shared_ptr<const state> iddfs(std::shared_ptr<const state> root) {
    std::shared_ptr<const state> goalState = nullptr;
    int level = 0;
    while(goalState == nullptr) {
        level++;
        //printf("%d ", level);
        std::unordered_set<unsigned long long> *visit = new std::unordered_set<unsigned long long>;
#pragma omp parallel
    {
#pragma omp single
        {
            goalState = dls(root, level, *visit);
        }
    };
 
    }
    return goalState;
}
/*std::shared_ptr<const state> dls(std::shared_ptr<const state> root, int level) {
    if(root->is_goal()){
        return root;
    }
    if (level <= 0){
        return nullptr;
    }
    std::vector<std::shared_ptr<const state>> neighbors = root->next_states();
    std::shared_ptr<const state> goalState;
//#pragma omp parallel for
    for(long long unsigned int j = 0; j < neighbors.size(); j++) {
        std::shared_ptr<const state> neighb = neighbors[j];
        goalState = dls(neighb, level-1);
//#pragma omp cancellation point for
        if(goalState != nullptr){
            return goalState;
//#pragma omp cancel for
        }
    }
    if(goalState != nullptr) {
        return goalState;
    }else{
        return nullptr;
    }
}
 
 
std::shared_ptr<const state> iddfs(std::shared_ptr<const state> root) {
    std::shared_ptr<const state> goalState = nullptr;
    int level = 0;
    while(goalState == nullptr){
        level++;
        //if(level == 1) {
        goalState = dls(root, level);*/
        /*}else{
            std::vector<std::shared_ptr<const state>> neighbors = root->next_states();
            std::vector<std::shared_ptr<const state>> goalStates;
#pragma omp parallel
            #pragma omp single
                    for(long long unsigned int j = 0; j < neighbors.size(); j++){
                        std::shared_ptr<const state> neighb = neighbors[j];
                        #pragma omp task
                        goalStates[j] = dls(neighb, level-1);
                    }
                    #pragma omp taskwait
                    for(long long unsigned int o = 0; o < goalStates.size(); o++){
                        if (goalStates[o] != nullptr && goalState != nullptr) {
                            if (goalState->get_identifier() > goalStates[o]->get_identifier()){
                                goalState = goalStates[o];
                            }
                        } else if(goalStates[o] != nullptr){
                            goalState = goalStates[o];
                        }
                    }
 
        }*/
        //printf("level %d done\n", level);
        //goalState = dls(root, level);
    /*}
    return goalState;
}*/
