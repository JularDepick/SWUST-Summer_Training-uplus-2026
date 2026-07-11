#pragma once
#include <bits/stdc++.h>
using namespace std;

#include "../assist.hpp"

#define REPORT_DIR "./report/"
#define INPUT_DIR "./input/"
#define DOCSMD_DIR "./docsmd/"
#define INEXT ".in.txt"

vector<string> metrics={
    "build_dt","size_bytes","size_kb",
    "endouble_dt","getdegs_dt","areacount_dt",
    "adjion_dt","adjionasf_dt","adjionast_dt",
    "dfs_dt","bfs_dt",
    "dijkstra_dt","bellmanford_dt",
    "clear_dt","total_dt"
};

map<string,string> read_meta(string name) {
    map<string,string> meta={};
    string inpath=INPUT_DIR+name+INEXT;
    ifstream fin(inpath);
    if(!fin.is_open()) {
        return meta;
    }
    int nn,mm;
    fin>>nn>>mm;
    meta["n"]=to_string(nn);
    meta["m"]=to_string(mm);
    /* skip edge lines */
    unsigned int a,b;
    int c;
    while(fin>>a>>b>>c) {}
    /* try read wl wr at end */
    int wl=0,wr=0;
    if(fin>>wl>>wr) {
        meta["wl"]=to_string(wl);
        meta["wr"]=to_string(wr);
    }
    fin.close();
    return meta;
}

void collect() {
    filesystem::path dir=REPORT_DIR;
    if(!filesystem::exists(dir)) {
        cout<<"- report directory not found"<<endl;
        return;
    }
    filesystem::path out_dir=DOCSMD_DIR;
    if(!filesystem::exists(out_dir)) {
        filesystem::create_directories(out_dir);
    }

    vector<pair<string,map<string,string>>> all_data={};
    for(const filesystem::directory_entry &entry:(filesystem::directory_iterator(dir))) {
        string fname=entry.path().filename().string();
        if(fname.size()<8) {
            continue;
        }
        string case_name=fname;
        if(case_name.size()>=9 && case_name.substr(case_name.size()-8)==".out.txt") {
            case_name=case_name.substr(0,case_name.size()-8);
        }
        ifstream fin(entry.path());
        map<string,string> kv={};
        string line;
        while(getline(fin,line)) {
            size_t pos=line.find('=');
            if(pos!=string::npos) {
                string key=line.substr(0,pos);
                string val=line.substr(pos+1);
                kv[key]=val;
            }
        }
        fin.close();
        map<string,string> meta=read_meta(case_name);
        for(pair<const string,string> &p:(meta)) {
            kv[p.first]=p.second;
        }
        all_data.push_back({case_name,kv});
    }

    sort(all_data.begin(),all_data.end(),[](const pair<string,map<string,string>> &a,const pair<string,map<string,string>> &b) {
        return (a.first<b.first);
    });

    for(const string &m:(metrics)) {
        string outpath=DOCSMD_DIR+m+".md";
        ofstream fout(outpath,ios::out);
        fout<<"# "<<m<<endl;
        bool found=false;
        for(const pair<string,map<string,string>> &d:(all_data)) {
            if(d.second.count(m)) {
                fout<<endl;
                fout<<"### "<<d.first<<".out.txt"<<endl;
                string nv=d.second.count("n")?d.second.at("n"):"?";
                string mv=d.second.count("m")?d.second.at("m"):"?";
                string wlv=d.second.count("wl")?d.second.at("wl"):"?";
                string wrv=d.second.count("wr")?d.second.at("wr"):"?";
                fout<<"- n="<<nv<<",m="<<mv<<",w["<<wlv<<","<<wrv<<"]"<<endl;
                fout<<"- "<<m<<"="<<d.second.at(m)<<endl;
                found=true;
            }
        }
        if(!found) {
            fout<<"(no data)"<<endl;
        }
        fout.close();
        cout<<"- wrote "<<outpath<<endl;
    }
}

int main() {
    cout<<"- collector: scanning "<<REPORT_DIR<<endl;
    collect();
    cout<<"- done"<<endl;
    pause();
    return 0;
}
