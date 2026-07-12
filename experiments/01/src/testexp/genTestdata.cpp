#pragma once
#include <bits/stdc++.h>
#include <filesystem>
using namespace std;

#include "../assist.hpp"
#include "../indexGraph.hpp"

#define TESTDATA_DIR "./input/"
#define INEXT ".in.txt"

ofstream fout;
unsigned long long bytescnt=0;

void generate(string name,int n,int m,int wl,int wr) {
    string filename=(name+INEXT);
    if((wl>wr) || (m>(n*(n-1)))) {
        cout<<"- INVALID: n="<<n<<" m="<<m<<" w["<<wl<<","<<wr<<"]"<<endl;
        return;
    }
    filesystem::path dir=TESTDATA_DIR;
    if(!filesystem::exists(dir)) {
        filesystem::create_directories(dir);
    }
    fout.open(TESTDATA_DIR+filename,ios::out);
    fout<<n<<' '<<m<<endl;
    if(wl==wr) {
        map<pair<int,int>,bool> v={};
        for(int i=0;i<m;) {
            int from=rdint.random(1,n,i+n+m);
            int to=from;
            while(to==from) {
                to=rdint.random(1,n,i+to+wl+wr);
            }
            if(v.count({from,to})) {
                continue;
            }
            i++;
            fout<<from<<' '<<to<<' '<<wl<<endl; 
        }
    } else {
        map<pair<int,int>,bool> v={};
        for(int i=0;i<m;) {
            int from=rdint.random(1,n,i+n+m);
            int to=from;
            while(to==from) {
                to=rdint.random(1,n,i+to+wl+wr);
            }
            if(v.count({from,to})) {
                continue;
            }
            i++;
            int w=rdint.random(wl,wr,i+from+to+wl+wl);
            fout<<from<<' '<<to<<' '<<w<<endl;
        }
    }
    fout<<endl<<wl<<endl<<wr<<endl;
    fout.close();
    bytescnt+=(filesystem::file_size(TESTDATA_DIR+filename));
    printf("%-5d    %-5d  %4d ~ %-4d",n,m,wl,wr);
    cout<<TESTDATA_DIR+filename<<endl;
}

int main() {
    cout<<"- generating: "<<endl<<endl
        <<"nodes    edges    weights    filepath"<<endl;

    /* batch 01-05: m=n, w[1,1] */
    generate("01",100,100,1,1);
    generate("02",200,200,1,1);
    generate("03",500,500,1,1);
    generate("04",1000,1000,1,1);
    generate("05",2000,2000,1,1);

    /* batch 11-15: m=2n, w[1,1] */
    generate("11",100,200,1,1);
    generate("12",200,400,1,1);
    generate("13",500,1000,1,1);
    generate("14",1000,2000,1,1);
    generate("15",2000,4000,1,1);

    /* batch 31-35: m=3n, w[1,1] */
    generate("31",100,300,1,1);
    generate("32",200,600,1,1);
    generate("33",500,1500,1,1);
    generate("34",1000,3000,1,1);
    generate("35",2000,6000,1,1);

    /* batch 41-45: m=4n, w[1,1] */
    generate("41",100,400,1,1);
    generate("42",200,800,1,1);
    generate("43",500,2000,1,1);
    generate("44",1000,4000,1,1);
    generate("45",2000,8000,1,1);

    /* batch 51-55: m=5n, w[1,1] */
    generate("51",100,500,1,1);
    generate("52",200,1000,1,1);
    generate("53",500,2500,1,1);
    generate("54",1000,5000,1,1);
    generate("55",2000,10000,1,1);

    /* batch 61-65: m=n, w[1,10] */
    generate("61",100,100,1,10);
    generate("62",200,200,1,10);
    generate("63",500,500,1,10);
    generate("64",1000,1000,1,10);
    generate("65",2000,2000,1,10);

    /* batch 71-75: m=2n, w[1,20] */
    generate("71",100,200,1,20);
    generate("72",200,400,1,20);
    generate("73",500,1000,1,20);
    generate("74",1000,2000,1,20);
    generate("75",2000,4000,1,20);

    /* batch 81-85: m=3n, w[1,50] */
    generate("81",100,300,1,50);
    generate("82",200,600,1,50);
    generate("83",500,1500,1,50);
    generate("84",1000,3000,1,50);
    generate("85",2000,6000,1,50);

    /* batch 91-95: m=4n, w[1,100] */
    generate("91",100,400,1,100);
    generate("92",200,800,1,100);
    generate("93",500,2000,1,100);
    generate("94",1000,4000,1,100);
    generate("95",2000,8000,1,100);

    /* batch 101-105: m=5n, w[-100,100] */
    generate("a1",100,500,-100,100);
    generate("a2",200,1000,-100,100);
    generate("a3",500,2500,-100,100);
    generate("a4",1000,5000,-100,100);
    generate("a5",2000,10000,-100,100);

    cout<<"\n- total bytes: "<<bytescnt<<"bytes = "<<bytescnt/1000<<"KB"<<endl;
    pause();

    return 0;
}
