#pragma once
#include <bits/stdc++.h>
using namespace std;

#define _usdint_ unsigned int

class indexGraph {
/* directed graph */
public:
    /* the number of nodes */
    _usdint_ n;
    /* the number of roads */
    _usdint_ m;
    /* adjacency list (roads from i to adj[i][j])*/
    vector<vector<_usdint_>> adj;
    /* table of weights for the edges */
    map<pair<_usdint_,_usdint_>,int> wet;
    /* for standard input content*/
    struct ewl {
        _usdint_ from;
        _usdint_ to;
        int w; /* weight */
        ewl(_usdint_ a,_usdint_ b) {
            from=a,to=b,w=1;
        }
        ewl(_usdint_ a,_usdint_ b,int c) {
            from=a,to=b,w=c;
        }
    };
    /* tag for undirected opereation */
    struct TAG4UO {} Tag;
    indexGraph() {
        n=0,m=0;
        adj={};
        wet={};
    }
    indexGraph(_usdint_ a,_usdint_ b) {
        n=a,m=b;
        adj.resize(n+1,vector<_usdint_>{});
        wet={};
    }
    bool build(_usdint_ a,_usdint_ b,vector<ewl> &vec) {
        n=a,m=b;
        adj.resize(n+1,vector<_usdint_>{});
        wet={};
        for(ewl aline:vec) {
            adj[aline.from].push_back(aline.to);
            wet[{aline.from,aline.to}]=aline.w;
        }
        return 1;
    }
    bool adjoin(_usdint_ f,_usdint_ t) {
        return (wet.count({f,t}) || wet.count({t,f}));
    }
    bool fromto(_usdint_ f,_usdint_ t) {
        return wet.count({f,t});
    }
    vector<_usdint_> get_adjoin(_usdint_ f) {
        return adj[f];
    }
    
    
    vector<_usdint_> BFS(_usdint_ origin) {
        /* directed */
    }
    vector<_usdint_> BFS(_usdint_ origin.TAG4UO _tag) {
        /* undirected */
    }
    vector<_usdint_> DFS(_usdint_ origin) {
        /* directed */
    }
    vector<_usdint_> DFS(_usdint_ origin.TAG4UO _tag) {
        /* undirected */
    }
};

