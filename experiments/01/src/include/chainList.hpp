#pragma once
#include <bits/stdc++.h>
using namespace std;

#include ".public.hpp"

template<typename T>
class chainList {
private:
	struct Node {
		T data;
		Node* next;
	};
	ll length;
	bool cycle;
	chainList::Node* first;
public:
	chainList() {
		
	}
};



