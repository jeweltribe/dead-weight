#pragma once
#include <vector>
#include "A1_Symbol.h"

class A1_Rule {
public:
	int ruleId;
	A1_Symbol lhs;
	vector<A1_Symbol> rhs;
	A1_Rule() {}
	A1_Rule(int _ruleId, A1_Symbol _lhs, vector<A1_Symbol> _rhs) { ruleId = _ruleId; lhs = _lhs; rhs = _rhs; }
};
