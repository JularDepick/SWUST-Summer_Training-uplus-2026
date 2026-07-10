#pragma once
#include <bits/stdc++.h>
using namespace std;

#include ".public.hpp"

template<typename T>
class chainStack {
public:
    struct Node {
		T data;
		Node* next;
		Node() {
			this->next==nullptr;
		}
		Node(T &onedata) {
			this->data=onedata;
			this->next==nullptr;
		}
	};
private:
public:
};
