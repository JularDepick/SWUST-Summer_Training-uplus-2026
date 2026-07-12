#pragma once
#include <bits/stdc++.h>
using namespace std;

#include "../assist.hpp"
#include "../indexGraph.hpp"

#define TESTDATA_DIR "./input/"
#define INEXT ".in.txt"
#define REPORT_DIR "./report/"
#define OUTEXT ".out.txt"

/* time clock */
TIMER tmc;
/* generally for no oversize */
indexGraph g;
ifstream fin;
ofstream fout;

void run2report(string name) {
    fin.open(TESTDATA_DIR+name+INEXT,ios::in);
    fout.open(REPORT_DIR+name+OUTEXT,ios::out);

    /* read n m */
    int Nnode,Medge;
    fin>>Nnode>>Medge;

    /* read edge lines until blank line or wl/wr */
    vector<indexGraph::ewl> finlines={};
    string line;
    int wl=1,wr=1;
    bool past_edges=false;
    vector<int> tail_vals={};
    while(getline(fin,line)) {
        if(line.empty()) {
            past_edges=true;
            continue;
        }
        istringstream iss(line);
        int aa,bb,cc;
        if(past_edges) {
            /* wl or wr line */
            if(iss>>aa) {
                tail_vals.push_back(aa);
            }
        } else {
            if(iss>>aa>>bb>>cc) {
                finlines.push_back({(_usdint_)aa,(_usdint_)bb,cc});
            } else {
                past_edges=true;
                if(iss>>aa) {
                    tail_vals.push_back(aa);
                }
            }
        }
    }
    if(tail_vals.size()>=2) {
        wl=tail_vals[tail_vals.size()-2];
        wr=tail_vals[tail_vals.size()-1];
    } else if(tail_vals.size()==1) {
        wl=tail_vals[0];
    }
    fin.close();
    cout<<"["<<name<<"] n="<<Nnode<<" m="<<Medge<<" w["<<wl<<","<<wr<<"]"<<endl;

    double total_dt=0;

    /* build_dt */
    cout<<"  build..."<<flush;
    tmc.start();
    g.build(Nnode,Medge,finlines);
    tmc.stop();
    fout<<"build_dt="<<fixed<<setprecision(3)<<tmc.dt()<<"ms"<<endl;
    total_dt+=tmc.dt();
    fout<<"size_bytes="<<g.size()<<endl;
    fout<<"size_kb="<<fixed<<setprecision(3)<<(g.size()/1000.0)<<"KB"<<endl;
    cout<<" done ("<<tmc.dt()<<"ms)"<<endl;

    _usdint_ oldn=g.node_count();
    _usdint_ oldm=g.edge_count();

    /* endouble_dt */
    cout<<"  endouble..."<<flush;
    g.build(Nnode,Medge,finlines);
    tmc.start();
    g.add_nodes(oldn);
    set<pair<_usdint_,_usdint_>> used_edges={};
    for(indexGraph::ewl &e:(finlines)) {
        used_edges.insert({e.from,e.to});
    }
    for(_usdint_ i=0;i<oldm;) {
        _usdint_ from=(_usdint_)rdint.random(1,2*oldn,(int)(i+oldn+oldm));
        _usdint_ to=from;
        while(to==from) {
            to=(_usdint_)rdint.random(1,2*oldn,(int)(i+to+3));
        }
        if(used_edges.count({from,to})) {
            continue;
        }
        g.add_edge(from,to,1);
        used_edges.insert({from,to});
        i++;
    }
    tmc.stop();
    fout<<"endouble_dt="<<fixed<<setprecision(3)<<tmc.dt()<<"ms"<<endl;
    total_dt+=tmc.dt();
    cout<<" done ("<<tmc.dt()<<"ms)"<<endl;

    /* getdegs_dt */
    cout<<"  getdegs..."<<flush;
    g.build(Nnode,Medge,finlines);
    tmc.start();
    for(_usdint_ i=1;i<=g.node_count();i++) {
        g.out_deg(i);
        g.in_deg(i);
    }
    tmc.stop();
    fout<<"getdegs_dt="<<fixed<<setprecision(3)<<tmc.dt()<<"ms"<<endl;
    total_dt+=tmc.dt();
    cout<<" done ("<<tmc.dt()<<"ms)"<<endl;

    /* areacount_dt */
    cout<<"  areacount..."<<flush;
    g.build(Nnode,Medge,finlines);
    tmc.start();
    g.area_count();
    tmc.stop();
    fout<<"areacount_dt="<<fixed<<setprecision(3)<<tmc.dt()<<"ms"<<endl;
    total_dt+=tmc.dt();
    cout<<" done ("<<tmc.dt()<<"ms)"<<endl;

    /* adjion_dt */
    cout<<"  adjion..."<<flush;
    g.build(Nnode,Medge,finlines);
    tmc.start();
    for(_usdint_ i=1;i<=g.node_count();i++) {
        g.get_adjoin(i);
    }
    tmc.stop();
    fout<<"adjion_dt="<<fixed<<setprecision(3)<<tmc.dt()<<"ms"<<endl;
    total_dt+=tmc.dt();
    cout<<" done ("<<tmc.dt()<<"ms)"<<endl;

    /* adjionasf_dt */
    cout<<"  adjionasf..."<<flush;
    g.build(Nnode,Medge,finlines);
    tmc.start();
    for(_usdint_ i=1;i<=g.node_count();i++) {
        g.get_adjoin_asf(i);
    }
    tmc.stop();
    fout<<"adjionasf_dt="<<fixed<<setprecision(3)<<tmc.dt()<<"ms"<<endl;
    total_dt+=tmc.dt();
    cout<<" done ("<<tmc.dt()<<"ms)"<<endl;

    /* adjionast_dt */
    cout<<"  adjionast..."<<flush;
    g.build(Nnode,Medge,finlines);
    tmc.start();
    for(_usdint_ i=1;i<=g.node_count();i++) {
        g.get_adjoin_ast(i);
    }
    tmc.stop();
    fout<<"adjionast_dt="<<fixed<<setprecision(3)<<tmc.dt()<<"ms"<<endl;
    total_dt+=tmc.dt();
    cout<<" done ("<<tmc.dt()<<"ms)"<<endl;

    /* dfs_dt */
    cout<<"  dfs..."<<flush;
    g.build(Nnode,Medge,finlines);
    tmc.start();
    for(_usdint_ i=1;i<=g.node_count();i++) {
        g.DFS(i);
    }
    tmc.stop();
    fout<<"dfs_dt="<<fixed<<setprecision(3)<<tmc.dt()<<"ms"<<endl;
    total_dt+=tmc.dt();
    cout<<" done ("<<tmc.dt()<<"ms)"<<endl;

    /* bfs_dt */
    cout<<"  bfs..."<<flush;
    g.build(Nnode,Medge,finlines);
    tmc.start();
    for(_usdint_ i=1;i<=g.node_count();i++) {
        g.BFS(i);
    }
    tmc.stop();
    fout<<"bfs_dt="<<fixed<<setprecision(3)<<tmc.dt()<<"ms"<<endl;
    total_dt+=tmc.dt();
    cout<<" done ("<<tmc.dt()<<"ms)"<<endl;

    /* dijkstra_dt */
    cout<<"  dijkstra..."<<flush;
    g.build(Nnode,Medge,finlines);
    vector<_usdint_> all_nodes={};
    for(_usdint_ i=1;i<=g.node_count();i++) {
        all_nodes.push_back(i);
    }
    tmc.start();
    for(int k=0;k<50;k++) {
        _usdint_ src=(_usdint_)rdint.random(0,(int)all_nodes.size()-1,k);
        _usdint_ tgt=(_usdint_)rdint.random(0,(int)all_nodes.size()-1,k+1000);
        if(src==tgt) {
            tgt=(tgt+1)%all_nodes.size();
        }
        g.dijkstra(all_nodes[src],all_nodes[tgt]);
    }
    tmc.stop();
    fout<<"dijkstra_dt="<<fixed<<setprecision(3)<<tmc.dt()<<"ms"<<endl;
    total_dt+=tmc.dt();
    cout<<" done ("<<tmc.dt()<<"ms)"<<endl;

    /* bellmanford_dt */
    cout<<"  bellmanford..."<<flush;
    g.build(Nnode,Medge,finlines);
    tmc.start();
    for(int k=0;k<50;k++) {
        _usdint_ src=(_usdint_)rdint.random(0,(int)all_nodes.size()-1,k+2000);
        _usdint_ tgt=(_usdint_)rdint.random(0,(int)all_nodes.size()-1,k+3000);
        if(src==tgt) {
            tgt=(tgt+1)%all_nodes.size();
        }
        g.bellman_ford(all_nodes[src],all_nodes[tgt]);
    }
    tmc.stop();
    fout<<"bellmanford_dt="<<fixed<<setprecision(3)<<tmc.dt()<<"ms"<<endl;
    total_dt+=tmc.dt();
    cout<<" done ("<<tmc.dt()<<"ms)"<<endl;

    /* clear_dt */
    cout<<"  clear..."<<flush;
    tmc.start();
    g.clear();
    tmc.stop();
    fout<<"clear_dt="<<fixed<<setprecision(3)<<tmc.dt()<<"ms"<<endl;
    total_dt+=tmc.dt();
    cout<<" done ("<<tmc.dt()<<"ms)"<<endl;

    fout<<"total_dt="<<fixed<<setprecision(3)<<total_dt<<"ms"<<endl;

    fout<<"n="<<Nnode<<endl;
    fout<<"m="<<Medge<<endl;
    fout<<"wl="<<wl<<endl;
    fout<<"wr="<<wr<<endl;

    fout.close();
    fin.close();
}

int main() {
    filesystem::path dir=REPORT_DIR;
    if(!filesystem::exists(dir)) {
        filesystem::create_directories(dir);
    }
    g.clear();
    /* batch 01-05: m=n, w[1,1] */
    run2report("01");
    run2report("02");
    run2report("03");
    run2report("04");
    run2report("05");

    /* batch 11-15: m=2n, w[1,1] */
    run2report("11");
    run2report("12");
    run2report("13");
    run2report("14");
    run2report("15");

    /* batch 21-25: m=3n, w[1,1] */
    run2report("21");
    run2report("22");
    run2report("23");
    run2report("24");
    run2report("25");

    /* batch 31-35: m=4n, w[1,1] */
    run2report("31");
    run2report("32");
    run2report("33");
    run2report("34");
    run2report("35");

    /* batch 41-45: m=5n, w[1,1] */
    run2report("41");
    run2report("42");
    run2report("43");
    run2report("44");
    run2report("45");

    /* batch 51-55: m=n, w[1,10] */
    run2report("51");
    run2report("52");
    run2report("53");
    run2report("54");
    run2report("55");

    /* batch 61-65: m=2n, w[1,20] */
    run2report("61");
    run2report("62");
    run2report("63");
    run2report("64");
    run2report("65");

    /* batch 71-75: m=3n, w[1,50] */
    run2report("71");
    run2report("72");
    run2report("73");
    run2report("74");
    run2report("75");

    /* batch 81-85: m=4n, w[1,100] */
    run2report("81");
    run2report("82");
    run2report("83");
    run2report("84");
    run2report("85");

    /* batch 91-95: m=5n, w[1,100] */
    run2report("91");
    run2report("92");
    run2report("93");
    run2report("94");
    run2report("95");

    /* batch a1-a5: m=5n, w[-100,100] */
    run2report("a1");
    run2report("a2");
    run2report("a3");
    run2report("a4");
    run2report("a5");

    pause();
    return 0;
}
