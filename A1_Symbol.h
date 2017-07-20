#pragma once
#include <string>
using namespace std;

class A1_Symbol {
	friend class A1_Node;
public:
	string Name;
	int tokenId;
	bool isTerminal;
	A1_Symbol() : Name(""), tokenId(9001), isTerminal(false) { }
	A1_Symbol(string _Name, int _tokenId, bool _isTerminal) : Name(_Name), tokenId(_tokenId), isTerminal(_isTerminal) { } 
};