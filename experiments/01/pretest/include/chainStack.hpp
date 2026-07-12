#pragma once
#include <bits/stdc++.h>
using namespace std;

#include ".public.hpp"

template<typename T>
class chainStack {
public:
    struct Node {
		T data;
		Node* pre;
		Node() {
			this->pre==nullptr;
		}
		Node(T &onedata) {
			this->data=onedata;
			this->pre==nullptr;
		}
	};
private:
    Node* top;
    _ullong_ length;
public:
    chainStack() {
        this->top=nullptr;
        this->length=0; 
    }
    chainStack(vector<T> &vec) {
        this->top=nullptr;
        this->length=0;
        this->push(vec); 
	}
	bool empty() {
	    return !(this->top && (this->length)>0);
    }
    _ullong_ size() {
        return (this->length);
    }
    T top_data() {
        return (this->top->data);
    }
	chainStack* push(T onedata) {
	    if(this->top) {
	        Node* onend=new Node(onedata);
    	    onend->pre=(this->top);
    	    this->top=onend;
    	    this->length+=1;
        } else {
            this->top=new Node(onedata);
            this->length=1;
        }
    	return this;
    }
	chainStack* push(vector<T> &vec) {
	    for(T onedata:vec) {
	        this->push(onedata);
        }
        return this;
    }
    chainStack* pop() {
        if(this->top) {
            this->top=(this->top->pre);
            length-=1;
        }
        return this;
    }
};
