#pragma once
#include <bits/stdc++.h>
using namespace std;

#include ".public.hpp"

template<typename T>
class chainList {
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
	struct _INDEX_ {} const INDEX;
private:
	_ullong_ length;
	Node* head;
	Node* tail;
	Node* _find_pre_tail() {
		Node* preptr=nullptr;
		Node* ptr=(this->head);
		if(ptr) {
			while(ptr->next) {
				preptr=ptr;
				ptr=(ptr->next);
			}
		}
		return preptr;
	}
public:
	chainList() {
		this->head=nullptr;
		this->tail=nullptr;
		this->length=0;
	}
	chainList(T &first) {
		this->head=new (Node);
		this->head->data=first;
		this->tail=(this->head);
		this->length=1;
	}
	chainList(vector<T> &vec) {
		Node* firstptr=new Node;
		Node* ptr=firstptr;
		for(T onedata:vec) {
			ptr->next=new Node(onedata);
			ptr=(ptr->next);
		}
		this->tail=ptr;
		this->head=(firstptr->next);
		delete firstptr;
		this->length=(vec.size());
	}
	T head_data() {
	    return (this->head->data);
    }
	T tail_data() {
	    return (this->tail->data);
    }
	bool empty() {
		return !((this->head) && (this->tail) && (length!=0));
	}
	_ullong_ size() {
		return (this->length);
	}
	bool is_loop() {
		if(this->empty()) {
			return false;
		}
		return ((this->tail->next)==(this->head));
	}
	chainList* enloop() {
		if(!(this->empty())) {
			if(!(this->is_loop())) {
				this->tail->next=(this->head);
			}
		}
		return this;
	}
	chainList* deloop() {
		if(!(this->empty())) {
			if(this->is_loop()) {
				this->tail->next=nullptr;
			}
		}
		return this;
	}
	Node* find(_ullong_ idx) {
		if(idx<=0) {
			return nullptr;
		}
		if(idx>(this->length)) {
			if(this->is_loop()) {
				idx%=(this->length+1);
			} else {
				return nullptr;
			}
		}
		Node* ptr=(this->head);
		_ullong_ cnt=0;
		while(cnt<idx /* && ptr->next */) {
			ptr=(ptr->next);
			cnt++;
		}
		return ptr;
	}
	_ullong_ find(Node* target) {
		if(target) {
			if(this->empty()) {
				return 0;
			}
			Node* ptr=(this->head);
			_ullong_ cnt=0,l=(this->length);
			while(cnt<l) {
				cnt++;
				if(ptr==target) {
				    return cnt;
                }
           	    ptr=(ptr->next);
			}
		}
		return 0;
	}
	Node* find_data(_ullong_ target) {
	    if(this->empty()) {
	        return nullptr;
        }
		Node* ptr=(this->head);
		_ullong_ cnt=0,l=(this->length);
		while(cnt<l) {
			cnt++;
			if((ptr->data)==target) {
			    return ptr;
            }
       	    ptr=(ptr->next);
		}
		return nullptr;
	}
	_ullong_ find_data(_ullong_ target,chainList::_INDEX_) {
		if(this->empty()) {
	        return 0;
        }
		Node* ptr=(this->head);
		_ullong_ cnt=0,l=(this->length);
		while(cnt<l) {
			cnt++;
			if((ptr->data)==target) {
			    return cnt;
            }
       	    ptr=(ptr->next);
		}
		return 0;
	}
	chainList* add(T &newdata) {
		if(this->empty()) {
			this->head=new Node(newdata);
			this->tail=(this->head);
		} else {
			bool original_loop=(this->is_loop());
			if(original_loop) {
				this->deloop();
			}
			this->tail->next=new Node(newdata);
			this->tail=(this->tail->next);
			if(original_loop) {
				this->enloop();
			}
		}
		this->length+=1;
		return this;
	}
	chainList* add(vector<T> vec) {
	    for(T onedata:vec) {
	        this.add(onedata);
        }
        return this;
    }
	chainList* pop(bool residual=false) {
		if(this->empty()) {
			return this;
		}
		bool original_loop=(this->is_loop());
		/* // actually the opreation 'delete' has influenced as this->deloop() 
		if(original_loop) {
			this->deloop();
		} */
		Node* pre_tail=(this->_find_pre_tail());
		/* delete tail only after _find_pre_tail() */
		if(!residual) {
			delete (this->tail);
		}
		if(pre_tail) {
			this->tail=pre_tail;
			this->tail->next=nullptr;
			if(original_loop) {
				this->enloop();
			}
		} else {
			/* pre_tail==false means: this->length==1 (0 has been thought)*/
			this->tail=nullptr;
			this->head=nullptr;
			/* then this->empty()==true and no need this->enloop() */
		}
		this->length-=1;
		return this;
	}
	chainList* pop(_ullong_ number,bool residual=false) {
	    while(number--) {
	        this->pop();
        }
        return this;
    }
	chainList* clear(bool residual=false) {
		/* belike this->pop() to loop */
		if(this->empty()) {
			return this;
		}
		if(!residual) {
			Node* ptr=(this->head);
			Node* nextptr=nullptr;
			while(ptr) {
				nextptr=(ptr->next);
				delete ptr;
				ptr=nextptr;
			}
		}
		this->head=nullptr;
		this->tail=nullptr;
		this->length=0;
		return this;
	}
	chainList* rebuild(vector<T> &vec,bool residual=false) {
		bool original_loop=(this->is_loop());
		this->clear(residual);
		/* belike this->add() to loop */
		Node* firstptr=new Node;
		Node* ptr=firstptr;
		for(T onedata:vec) {
			ptr->next=new Node(onedata);
			ptr=(ptr->next);
		}
		this->tail=ptr;
		this->head=(firstptr->next);
		delete firstptr;
		this->length=(vec.size());
		if(original_loop) {
			this->enloop();
		}
		return this;
	}
	vector<T> get_vector() {
	    if(this->empty()) {
	        return {};
        }
        vector<T> vec(this->length);
        Node* ptr=(this->head);
		_ullong_ cnt=0,l=(this->length);
		while(cnt<l) {
		    vec[cnt]=(ptr->data);
			cnt++;
       	    ptr=(ptr->next);
		}
		return vec;
    }
	chainList* copy() {
	    if(this->empty()) {
	        return nullptr;
        }
        chainList* newcl=new chainList;
        newcl->head=new Node;
        Node* nclptr=(newcl->head);
        Node* ptr=(this->head);
		_ullong_ cnt=0,l=(this->length);
		while(cnt<l) {
			cnt++;
			nclptr->next=new Node(ptr->data);
			nclptr=(nclptr->next);
       	    ptr=(ptr->next);
		}
		newcl->tail=nclptr;
		newcl->length=(this->length);
		return newcl;
    }
    chainList* copy(chainList* target,bool residual=false) {
	    if(this->empty()) {
	        return nullptr;
        }
        /* belike target->rebuid(this->get_vector(),residual); */
        target->clear(residual);
        chainList* newcl=(this->copy());
        target->head=(newcl->head);
        target->tail=(newcl->tail);
        target->length=(newcl->length);
    }
    chainList* reverse(bool residual=false) {
        vector<T> vec=(this->get_vector());
        reverse(vec.begin(),vec.end());
        this.rebuild(vec,residual);
        return this;
    }
    chainList* reverse(chainList* target,bool residual=false) {
        vector<T> vec=(this->get_vector());
        reverse(vec.begin(),vec.end());
        target.rebuild(vec,residual);
        return target;
    }
};

