#pragma once
#include <stack>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

#include "A1_Rule.h"

#define eps 30

class A1_Parser {
public:
	stack<A1_Symbol> s; 
	// kwdprog	brace1	brace2	semi	id	equal	kwdinput	kwdprint	comma	paren1	paren2	int	float	string	plus	minus	aster	slash	caret	$
	int matrix[17][20] = {
		{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,eps,0,2,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,3,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,5,0,6,0,0,5,0,5,5,5,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,8,0,0,0,0,8,eps,8,8,8,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,9,0,eps,0,0,0,0,0,0,0,0,0 },
		{ 0,0,0,0,10,0,0,0,0,10,0,10,10,10,0,0,0,0,0,0 },
		{ 0,0,0,eps,0,0,0,0,eps,0,eps,0,0,0,11,11,0,0,0,0 },
		{ 0,0,0,0,12,0,0,0,0,12,0,12,12,12,0,0,0,0,0,0 },
		{ 0,0,0,eps,0,0,0,0,eps,0,eps,0,0,0,eps,eps,13,13,13,0 },
		{ 0,0,0,0,14,0,0,0,0,15,0,14,14,14,0,0,0,0,0,0 },
		{ 0,0,0,0,16,0,0,0,0,0,0,17,18,19,0,0,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,21,0,0,0,0 },
		{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,22,23,24,0 } };
	vector<A1_Rule> Rule;
	void buildRules(string fName);
	void ll_parser();
	A1_Rule retrieveRule(int ID);
	void displayStack(stack<A1_Symbol> s);

};

// build the rules from a text file
void A1_Parser::buildRules(string fName) { 
	A1_Symbol LHS, rightSide;
	vector<A1_Symbol> RHS;
	string Name, isTerminal, line;
	bool isTerm;
	int tokenId, RuleId = 1, size;
	ifstream f(fName);

	while (!f.eof()) {
		getline(f, line);
		stringstream linestream(line);
		linestream >> size;
		if (size == 1) {
			linestream >> Name >> tokenId >> isTerminal; // read the left hand side
			isTerm = (isTerminal == "true") ? 1 : 0;
			LHS = A1_Symbol(Name, tokenId, isTerm);
		}
		else {
			while (linestream >> Name >> tokenId >> isTerminal) { // read the right hand side
				isTerm = (isTerminal == "true") ? 1 : 0;
				rightSide = A1_Symbol(Name, tokenId, isTerm);
				RHS.push_back(rightSide);
			}
			A1_Rule rules(RuleId, LHS, RHS);
			Rule.push_back(rules);
			RuleId++;
			RHS.clear();
		}
	}
	f.close();
}

A1_Rule A1_Parser::retrieveRule(int ID) {
	A1_Rule found;
	for (unsigned int i = 0; i < Rule.size(); i++) {
		if (ID == Rule[i].ruleId) {
			found = A1_Rule(Rule[i].ruleId, Rule[i].lhs, Rule[i].rhs);
		}
	}
	return found;
}

void A1_Parser::displayStack(stack<A1_Symbol> s) {
	cout << "Stack : ";
	while (!s.empty()) {
		cout << s.top().Name << " ";
		s.pop();
	}
	cout << endl;
}
