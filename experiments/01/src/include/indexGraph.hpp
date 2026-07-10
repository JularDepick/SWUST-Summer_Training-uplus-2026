#pragma once
#include <bits/stdc++.h>
using namespace std;

#include ".public.hpp"

template<typename T> 
class indexGraph {
/* directed graph */
private:
    /* the number of nodes */
    _usdint_ n;
    /* the number of roads */
    _usdint_ m;
    /* adjacency list (roads from i to adj[i][j])*/
    vector<vector<T>> adj;
public:
    indexGraph() {
        n=0,m=0;
        adj={};
    }
    indexGraph(_usdint_ a,_usdint_ b) {
        n=a,m=b;
        adj.resize(n+1,vector<T>{});
    }
};

