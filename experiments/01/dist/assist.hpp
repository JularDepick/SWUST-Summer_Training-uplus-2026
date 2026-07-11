#pragma once
#include <bits/stdc++.h>
using namespace std;

class TIMER {
private:
    chrono::steady_clock::time_point beginning;
    chrono::steady_clock::time_point ending;
    bool busy;
public:
    TIMER() {
        busy=0;
    }
    TIMER& start() {
        if(!busy) {
            beginning=chrono::steady_clock::now();
            busy=1;
        }
        return *this;
    }
    TIMER& stop() {
        /* auto reset */
        if(busy) {
            ending=chrono::steady_clock::now();
            busy=0;
        }
        return *this;
    }
    TIMER& reset() {
        if(busy) {
            busy=0;
        }
        return *this;
    }
    long long dtms() {
        if(busy) {
            return -1;
        }
        return (chrono::duration_cast<chrono::milliseconds>(ending-beginning).count());
    }
    double dt() {
        if(busy) {
            return -1;
        }
        return (chrono::duration<double,milli>(ending-beginning).count());
    }
    bool working() {
        return busy;
    }
};

class {
public:
    int random(int a,int b,int extraSeed=0) const {
        if(a>b) {
            swap(a,b);
        }
        if(a==b) {
            return a;
        }
        unsigned int seed1=0;
        try {
            std::random_device rd;
            seed1=rd();
        } catch(const std::exception&) {
            long long now=(std::chrono::steady_clock::now().time_since_epoch().count());
            seed1=(static_cast<unsigned int>(now)) ^ 0x9e3779b9;
        }
        long long seed2_raw=(std::chrono::steady_clock::now().time_since_epoch().count());
        unsigned int seed2=(static_cast<unsigned int>(seed2_raw));
        unsigned int seed3=extraSeed;
        std::seed_seq seq{seed1,seed2,seed3};
        std::mt19937 gen(seq);
        std::uniform_int_distribution<int> dist(a,b);
        return (dist(gen));
    }
} const rdint;

void pause() {
    cout<<"\n>> Press any key to continue, such as Enter."<<endl;
    cin.get();
}

