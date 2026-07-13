#pragma once
#include <bits/stdc++.h>
using namespace std;

#define _usdint_ unsigned int

class indexGraph { /* directed graph */
private:
    _usdint_ n; /* the number of nodes */
    _usdint_ m; /* the number of roads */
    _usdint_ max_id; /* max id ever assigned */
    struct Node { /* node of graph */
        _usdint_ id;
        vector<Node*> adj; /* adjacency list (roads from this to adj[i]) */
        vector<int> wet; /* wet[i] is the weight from this to adj[i] */
        vector<Node*> radj; /* reverse adjacency list (roads from radj[i] to this) */
        vector<int> rwet; /* rwet[i] is the weight from radj[i] to this */
    };
    map<_usdint_,Node*> nodes; /* map from node_id to Node* */
    set<pair<_usdint_,_usdint_>> edges; /* set of edges for O(1) edge existence check */
    bool nege_flag; /* negative edge flag */
public:
    /* for standard input content */
    struct ewl { /* ewl: edges weight line */
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

    indexGraph() {
        n=0,m=0,max_id=0,nege_flag=0;
        nodes={};
        edges={};
    }
    ~indexGraph() {
        for(pair<const _usdint_,Node*> &p:(nodes)) {
            delete p.second;
        }
    }
    indexGraph& clear() {
        for(pair<const _usdint_,Node*> &p:(nodes)) {
            delete p.second;
        }
        nodes.clear();
        edges.clear();
        n=0,m=0,max_id=0,nege_flag=0;
        return *this;
    }

    indexGraph& build(_usdint_ a,_usdint_ b,vector<ewl> &vec) {
        clear();
        n=a,m=b,max_id=a;
        for(_usdint_ i=1;i<=n;i++) {
            Node* node=new Node;
            node->id=i;
            nodes[i]=node;
        }
        for(ewl &aline:(vec)) {
            Node* from_node=nodes[aline.from];
            Node* to_node=nodes[aline.to];
            from_node->adj.push_back(to_node);
            from_node->wet.push_back(aline.w);
            to_node->radj.push_back(from_node);
            to_node->rwet.push_back(aline.w);
            edges.insert({aline.from,aline.to});
            if(aline.w<0) {
                nege_flag=1;
            }
        }
        return *this;
    }

    bool empty() {
        return (n == 0);
    }
    _usdint_ node_count() {
        return n;
    }
    _usdint_ edge_count() {
        return m;
    }
    _usdint_ get_maxid() {
        return max_id;
    }
    
    bool node_valid(_usdint_ node_id) { /* check if node is valid */
        return (nodes.count(node_id)) && (nodes[node_id] != nullptr);
    }
    bool edge_valid(_usdint_ from,_usdint_ to) { /* check if edge exists */
        if((!node_valid(from)) || (!node_valid(to))) {
            return 0;
        }
        return (edges.count({from,to}));
    }

    bool adjoin(_usdint_ f,_usdint_ t) { /* check if two nodes are adjacent (either direction) */
        if((!node_valid(f)) || (!node_valid(t))) {
            return 0;
        }
        return (edges.count({f,t})) || (edges.count({t,f}));
    }
    vector<_usdint_> get_adjoin(_usdint_ node_id) {
        vector<_usdint_> res = {};
        if(!node_valid(node_id)) {
            return res;
        }
        Node* node=nodes[node_id];
        map<_usdint_,bool> hash={};
        for(Node* v:(node->adj)) {
            if(!hash.count(v->id)) {
                res.push_back(v->id);
                hash[v->id]=true;
            }
        }
        for(Node* v:(node->radj)) {
            if(!hash.count(v->id)) {
                res.push_back(v->id);
                hash[v->id]=true;
            }
        }
        return res;
    }
    vector<_usdint_> get_adjoin_asf(_usdint_ f) {
        vector<_usdint_> res = {};
        if(!node_valid(f)) {
            return res;
        }
        Node* node=nodes[f];
        for(Node* v:(node->adj)) {
            res.push_back(v->id);
        }
        return res;
    }
    vector<_usdint_> get_adjoin_ast(_usdint_ t) {
        vector<_usdint_> res = {};
        if(!node_valid(t)) {
            return res;
        }
        Node* node=nodes[t];
        for(Node* v:(node->radj)) {
            res.push_back(v->id);
        }
        return res;
    }

    _usdint_ out_deg(_usdint_ node_id) {
        if(!node_valid(node_id)) {
            return 0;
        }
        return (nodes[node_id]->adj.size());
    }
    _usdint_ in_deg(_usdint_ node_id) {
        if(!node_valid(node_id)) {
            return 0;
        }
        return (nodes[node_id]->radj.size());
    }
    void recheck_nege() {
        nege_flag=0;
        for(pair<const _usdint_,Node*> &p:(nodes)) {
            Node* nd=p.second;
            for(int w:(nd->wet)) {
                if(w<0) {
                    nege_flag=1;
                    return;
                }
            }
        }
    }

    indexGraph& add_node() { /* add a node */
        n++;
        max_id++;
        Node* node=new Node;
        node->id=max_id;
        nodes[max_id]=node;
        return *this;
    }
    indexGraph& del_node(_usdint_ node_id,bool resort_nodes=false) {
        if(!node_valid(node_id)) {
            return *this;
        }
        Node* del_node=nodes[node_id];
        _usdint_ removed_edges = 0;
        /* remove edges from this node */
        for(Node* v:(del_node->adj)) {
            edges.erase({node_id,v->id});
            for(size_t i=0;i<v->radj.size();i++) {
                if(v->radj[i]==del_node) {
                    v->radj.erase(v->radj.begin()+i);
                    v->rwet.erase(v->rwet.begin()+i);
                    removed_edges++;
                    break;
                }
            }
        }
        /* remove edges to this node */
        for(Node* v:(del_node->radj)) {
            edges.erase({v->id,node_id});
            for(size_t i=0;i<v->adj.size();i++) {
                if(v->adj[i]==del_node) {
                    v->adj.erase(v->adj.begin()+i);
                    v->wet.erase(v->wet.begin()+i);
                    removed_edges++;
                    break;
                }
            }
        }
        delete del_node;
        nodes.erase(node_id);
        n--;
        m -= removed_edges;
        if(resort_nodes) resort();
        recheck_nege();
        return *this;
    }
    indexGraph& resort() { /* renumber all nodes to contiguous ids */
        vector<Node*> sorted = {};
        for(map<_usdint_,Node*>::iterator it=nodes.begin();it!=nodes.end();++it) {
            sorted.push_back(it->second);
        }
        for(size_t i=0;i<sorted.size();i++) {
            for(size_t j=i+1;j<sorted.size();j++) {
                if(sorted[i]->id>sorted[j]->id) {
                    Node* tmp=sorted[i];
                    sorted[i]=sorted[j];
                    sorted[j]=tmp;
                }
            }
        }
        /* build old-to-new id mapping */
        map<_usdint_,_usdint_> id_map={};
        for(size_t i=0;i<sorted.size();i++) {
            id_map[sorted[i]->id]=(_usdint_)(i+1);
        }
        /* update edges */
        set<pair<_usdint_,_usdint_>> new_edges={};
        for(const pair<_usdint_,_usdint_> &e:(edges)) {
            new_edges.insert({id_map[e.first],id_map[e.second]});
        }
        edges=new_edges;
        /* rebuild nodes map */
        nodes.clear();
        for(size_t i=0;i<sorted.size();i++) {
            sorted[i]->id=(_usdint_)(i+1);
            nodes[i+1]=sorted[i];
        }
        max_id=n;
        return *this;
    }

    indexGraph& add_edge(_usdint_ from,_usdint_ to,int weight) { /* add a edges */
        if((!node_valid(from)) || (!node_valid(to))) {
            return *this;
        }
        if(edges.count({from,to})) {
            Node* from_node=nodes[from];
            Node* to_node=nodes[to];
            for(size_t i=0;i<from_node->adj.size();i++) {
                if(from_node->adj[i]==to_node) {
                    from_node->wet[i]=weight;
                    /* find correct index in to_node's radj */
                    for(size_t j=0;j<to_node->radj.size();j++) {
                        if(to_node->radj[j]==from_node) {
                            to_node->rwet[j]=weight;
                            break;
                        }
                    }
                    return *this;
                }
            }
        }
        Node* from_node=nodes[from];
        Node* to_node=nodes[to];
        from_node->adj.push_back(to_node);
        from_node->wet.push_back(weight);
        to_node->radj.push_back(from_node);
        to_node->rwet.push_back(weight);
        edges.insert({from,to});
        m++;
        if(weight<0) {
            nege_flag=1;
        }
        return *this;
    }
    indexGraph& del_edge(_usdint_ from,_usdint_ to) {
        if(!edge_valid(from,to)) {
            return *this;
        }
        Node* from_node=nodes[from];
        Node* to_node=nodes[to];
        for(size_t i=0;i<from_node->adj.size();i++) {
            if(from_node->adj[i]==to_node) {
                from_node->adj.erase(from_node->adj.begin()+i);
                from_node->wet.erase(from_node->wet.begin()+i);
                break;
            }
        }
        for(size_t i=0;i<to_node->radj.size();i++) {
            if(to_node->radj[i]==from_node) {
                to_node->radj.erase(to_node->radj.begin()+i);
                to_node->rwet.erase(to_node->rwet.begin()+i);
                break;
            }
        }
        edges.erase({from,to});
        m--;
        recheck_nege();
        return *this;
    }
    indexGraph& mod_edge(_usdint_ from,_usdint_ to,int new_weight) { /* change edge weight */
        if(!edge_valid(from,to)) {
            return *this;
        }
        Node* from_node=nodes[from];
        Node* to_node=nodes[to];
        for(size_t i=0;i<from_node->adj.size();i++) {
            if(from_node->adj[i]==to_node) {
                from_node->wet[i]=new_weight;
                to_node->rwet[i]=new_weight;
                break;
            }
        }
        recheck_nege();
        return *this;
    }

    int get_edge_weight(_usdint_ from,_usdint_ to) { /* get edge weight */
        if(!edge_valid(from,to)) {
            return 0;
        }
        Node* from_node=nodes[from];
        Node* to_node=nodes[to];
        for(size_t i=0;i<from_node->adj.size();i++) {
            if(from_node->adj[i]==to_node) {
                return from_node->wet[i];
            }
        }
        return 0;
    }

    bool has_negative_edge() const { /* check if any edge has negative weight */
        return nege_flag;
    }

    _usdint_ area_count() {
        _usdint_ cnt=0;
        map<_usdint_,bool> vised={};
        for(pair<const _usdint_,Node*> &p:(nodes)) {
            vised[p.first]=false;
        }
        for(pair<const _usdint_,Node*> &p:(nodes)) {
            _usdint_ sid=p.first;
            if(vised[sid]) {
                continue;
            }
            cnt++;
            stack<_usdint_> stk={};
            stk.push(sid);
            vised[sid]=true;
            while(!stk.empty()) {
                _usdint_ u=stk.top(); stk.pop();
                Node* un=nodes[u];
                for(Node* v:(un->adj)) {
                    if((node_valid(v->id))&&(!vised[v->id])) {
                        vised[v->id]=true;
                        stk.push(v->id);
                    }
                }
                for(Node* v:(un->radj)) {
                    if((node_valid(v->id))&&(!vised[v->id])) {
                        vised[v->id]=true;
                        stk.push(v->id);
                    }
                }
            }
        }
        return cnt;
    }


    vector<_usdint_> bellman_ford(_usdint_ source,_usdint_ target) {
        vector<_usdint_> res={};
        if((!node_valid(source))||(!node_valid(target))) {
            return res;
        }
        const int INF=1e9;
        map<_usdint_,int> dist={};
        map<_usdint_,_usdint_> pre={};
        for(pair<const _usdint_,Node*> &p:(nodes)) {
            dist[p.first]=INF;
            pre[p.first]=0;
        }
        dist[source]=0;
        for(_usdint_ iter=0;iter<n-1;iter++) {
            bool changed=false;
            for(const pair<_usdint_,_usdint_> &e:(edges)) {
                _usdint_ u=e.first,v=e.second;
                if((!node_valid(u))||(!node_valid(v))) {
                    continue;
                }
                Node* un=nodes[u];
                int w=0;
                for(size_t i=0;i<un->adj.size();i++) {
                    if(un->adj[i]->id==v) {
                        w=un->wet[i];
                        break;
                    }
                }
                if(dist[u]+w<dist[v]) {
                    dist[v]=dist[u]+w;
                    pre[v]=u;
                    changed=true;
                }
            }
            if(!changed) {
                break;
            }
        }
        if(dist[target]==INF) {
            return res;
        }
        _usdint_ cur=target;
        while(cur!=source) {
            res.push_back(cur);
            cur=pre[cur];
        }
        res.push_back(source);
        reverse(res.begin(),res.end());
        return res;
    }

    vector<_usdint_> dijkstra(_usdint_ source,_usdint_ target) {
        vector<_usdint_> res={};
        if((!node_valid(source))||(!node_valid(target))) {
            return res;
        }
        const int INF=1e9;
        map<_usdint_,int> dist;
        map<_usdint_,_usdint_> pre;
        map<_usdint_,bool> vised;
        for(pair<const _usdint_,Node*> &p:(nodes)) {
            dist[p.first]=INF;
            pre[p.first]=0;
            vised[p.first]=false;
        }
        priority_queue<pair<int,_usdint_>,vector<pair<int,_usdint_>>,greater<pair<int,_usdint_>>> pq;
        dist[source]=0;
        pq.push({0,source});
        while(!pq.empty()) {
            pair<int,_usdint_> top=pq.top(); pq.pop(); int d=top.first; _usdint_ u=top.second;
            if(vised[u]) {
                continue;
            }
            vised[u]=true;
            if(u==target) {
                break;
            }
            Node* un=nodes[u];
            for(size_t i=0;i<un->adj.size();i++) {
                _usdint_ v=un->adj[i]->id;
                if(!node_valid(v)) {
                    continue;
                }
                if(dist[u]+un->wet[i]<dist[v]) {
                    dist[v]=dist[u]+un->wet[i];
                    pre[v]=u;
                    pq.push({dist[v],v});
                }
            }
        }
        if(dist[target]==INF) {
            return res;
        }
        _usdint_ cur=target;
        while(cur!=source) {
            res.push_back(cur);
            cur=pre[cur];
        }
        res.push_back(source);
        reverse(res.begin(),res.end());
        return res;
    }

    vector<_usdint_> BFS(_usdint_ origin) {
        vector<_usdint_> res={};
        if(!node_valid(origin)) {
            return res;
        }
        map<_usdint_,bool> vised;
        for(pair<const _usdint_,Node*> &p:(nodes)) {
            vised[p.first]=false;
        }
        queue<_usdint_> q;
        q.push(origin);
        vised[origin]=true;
        while(!q.empty()) {
            _usdint_ u=q.front(); q.pop();
            res.push_back(u);
            Node* un=nodes[u];
            for(Node* v:(un->adj)) {
                if((node_valid(v->id))&&(!vised[v->id])) {
                    vised[v->id]=true;
                    q.push(v->id);
                }
            }
        }
        return res;
    }
    vector<_usdint_> DFS(_usdint_ origin) {
        vector<_usdint_> res={};
        if(!node_valid(origin)) {
            return res;
        }
        map<_usdint_,bool> vised;
        for(pair<const _usdint_,Node*> &p:(nodes)) {
            vised[p.first]=false;
        }
        stack<_usdint_> s;
        s.push(origin);
        while(!s.empty()) {
            _usdint_ u=s.top(); s.pop();
            if(vised[u]) {
                continue;
            }
            vised[u]=true;
            res.push_back(u);
            Node* un=nodes[u];
            for(Node* v:(un->adj)) {
                if((node_valid(v->id))&&(!vised[v->id])) {
                    s.push(v->id);
                }
            }
        }
        return res;
    }
};
