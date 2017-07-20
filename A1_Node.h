#pragma once
#include <vector>
#include "A1_Symbol.h"

class A1_Node {
public:
	A1_Symbol symbol;
	string momID;
	int ruleID;
	A1_Node *mom;
	vector<A1_Node*> children;

	A1_Node() {
		
	}
	A1_Node(A1_Symbol _symbol, string _momID, int _ruleID, A1_Node *_mom, vector<A1_Node*> _children) {
		symbol = _symbol;
		momID = _momID;
		ruleID = _ruleID;
		mom = _mom;
		children = _children;
	}
};
