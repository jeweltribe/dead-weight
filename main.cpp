// Varun Rai 
// Felipe Perez

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <iomanip>
#include "A1_Parser.h"
#include "A1_Node.h"
#include "Tree.h"

/*#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif*/

// Function declarations
void addChar();                // add nextChar to char array
void getChar();				   // read next character from file
int next_token();              // perform analysis, return token id
void skipBlanks();			   // ignore spaces
int getKwdID();					// get ID numbers for lexeme
int getSignsID();
int getTypeID();				// for integer and float type token
int searchEnumR(int key);
int searchEnumC(int key);
string getStrAbbr(int key);

								// global declarations
int lineNumber = 1;
int position = 0;
int charClass;				  // keep track of whether char is letter, digit, or punct
char nextChar;                // char read from text file
char lexeme[100];             // store nextChar in array
int lexLen;                   // keep track of char array
int nextToken;                // id of token for lexeme
int doubleQuotes;			  // increment when double quotes is read for string reading
FILE *f = fopen("Test.txt", "r");

// character class
#define letter 100			 // can contain all other classes, but nextChar must begin with a letter or double quotes (strings)
#define digit 101		     // will only accept the dot character other than numbers
#define punct 102            
#define unknown 103			 // if the character isn't defined in the lexicon

// single and multi line comments
#define comment 1

// literals and variables
#define identifier 2
#define integer 3
#define float 4
#define STRING 5

#define SL 50 
#define Ss 51 
#define Y4 52
#define SO 53
#define EL 54 
#define E2 55
#define E 56
#define E_ 57
#define T 58
#define T_ 59
#define F 60 
#define Fa 61 
#define Oa 62 
#define Om 63 

// reference to matrix
enum Rows { Pgm, Slist, Stmt, _Y4, S_Out, Elist, Elist2, Ex, _Ex, Te, _Te, Fe, Fatom, Opadd, Opmul };
enum Columns { kp, br1, br2, sem, iden, eq, kwip, kwdp, com, par1, par2, inte, floa, strn, plu, minu, ast, sla, car, dol };

// unpaired delimiters
#define comma 6
#define semi 7

// keywords
#define kwdprog 10
#define kwdfcn 12
#define kwdfloat 15
#define kwdint 16
#define kwdstring 17
#define kwdif 18
#define kwdelseif 19
#define kwdelse 20
#define kwdwhile 21
#define kwdinput 22
#define kwdprint 23
#define kwdnew 24
#define kwdreturn 25

// paired delimeters
#define brace1 33
#define brace2 34
#define parens1 37
#define parens2 38

// other punctuation
#define aster 41
#define caret 42
#define colon 43
#define equal 45
#define minus 46
#define plus 47
#define slash 48

// miscellaneous
#define eps 30
#define error 99
#define eof 0

// creation of the symbol table
struct SymTab {
	string ID;
	vector<pair<int, int>> occurrences;
	
};

vector<SymTab> symbolTable;

// test ll_parser machine
int main() {
	A1_Parser LL_Parser;
	LL_Parser.buildRules("Rules.txt");
	LL_Parser.ll_parser();
	fclose(f);
	return 0;
}

void addChar() { // append nextChar to char array
	if (lexLen <= 98) {
		lexeme[lexLen++] = nextChar;
		lexeme[lexLen] = 0;
	}
	else {
		printf("Error - lexeme is too long \n");
	}
}

void getChar() { // read char from text file
	if ((nextChar = getc(f)) != EOF) {
		if (isalpha(nextChar)) {
			charClass = letter;
		}
		else if (isdigit(nextChar)) {
			charClass = digit;
		}
		else if (ispunct(nextChar)) {
			if (nextChar == '"') {  // punctuation for strings goes to letter character class
				doubleQuotes++;
				charClass = letter;
			}
			else {
				charClass = punct;
			}
		}
		else {
			charClass = unknown;
		}
		position++;
	}
	else {
		charClass = eof;
	}
}

int next_token() {
	doubleQuotes = 0;
	lexLen = 0;
	skipBlanks();

	switch (charClass) {
	case letter:
		addChar();
		getChar();
		// letter character class accepts digits, punctuation, and spaces for strings and identifiers
		while ((charClass == letter || charClass == digit || nextChar == '_' || ((nextChar == ' ' || charClass == punct) && doubleQuotes == 1))) {
			addChar();
			getChar();

		}
		nextToken = getKwdID();  // retrieve token id for lexeme
		break;
	case digit:
		addChar();
		getChar();
		// digit character class can only accept periods, plus and minus are dealt separately (the parser can handle whether to pair)
		while (charClass == digit || nextChar == '.') {
			addChar();
			getChar();
		}
		nextToken = getTypeID();
		break;
	case punct:
		addChar();
		getChar();
		if (nextChar == '/') {			// ignore comments that start with //
			addChar();
			while (nextChar != '\n') {
				getChar();
			}
		}
		else if (nextChar == '*') {// ignore comments that start with /* and end with */
			addChar();
			while (nextChar != '/') {
				getChar();
			}
		}
		else {
			// check for multi-char operators
			while ((charClass == punct) && nextChar == '-' || nextChar == '=' || nextChar == '!' || nextChar == '<' || nextChar == '>') {
				addChar();
				getChar();
			}
		}
		nextToken = getSignsID();
		break;
	case eof:
		nextToken = eof;
		lexeme[0] = 'E';
		lexeme[1] = 'O';
		lexeme[2] = 'F';
		lexeme[3] = 0;
		break;
	}
	std::string str(lexeme);
	if (nextToken == STRING) { str.erase(remove(str.begin(), str.end(), '\"'), str.end()); } // remove double quotes for string lexeme
	if (nextToken != comment) { 
		if (nextChar == '\n') { lineNumber++; position = 0; } // keep track of newline and position
	}
	else { // get the next character to avoid trailing '/' for */ comments
		getChar();
	}
	 
	return nextToken;
}

// skip whitespace and get to the next character
void skipBlanks() {
	while (isspace(nextChar)) {
		getChar();
	}
}

int getKwdID() {
	std::string str(lexeme);
	if (str == "prog") {
		return kwdprog;
	}
	else if (str == "fcn") {
		return kwdfcn;
	}
	else if (str == "float") {
		return kwdfloat;
	}
	else if (str == "int") {
		return kwdint;
	}
	else if (str == "string") {
		return kwdstring;
	}
	else if (str == "if") {
		return kwdif;
	}
	else if (str == "elseif") {
		return kwdelseif;
	}
	else if (str == "else") {
		return kwdelse;
	}
	else if (str == "while") {
		return kwdwhile;
	}
	else if (str == "input") {
		return kwdinput;
	}
	else if (str == "print") {
		return kwdprint;
	}
	else if (str == "new") {
		return kwdnew;
	}
	else if (str == "return") {
		return kwdreturn;
	}
	else {
		std::size_t found = str.find("\"");
		if (found != std::string::npos) {
			return STRING;
		}
		else {
			return identifier;
		}
	}
}

int getSignsID() {
	std::string str(lexeme);
	if (str == ",") {
		return comma;
	}
	else if (str == ";") {
		return semi;
	}
	else if (str == "{") {
		return brace1;
	}
	else if (str == "}") {
		return brace2;
	}
	else if (str == "(") {
		return parens1;
	}
	else if (str == ")") {
		return parens2;
	}
	else if (str == "*") {
		return aster;
	}
	else if (str == "^") {
		return caret;
	}
	else if (str == ":") {
		return colon;
	}
	else if (str == "=") {
		return equal;
	}
	else if (str == "-") {
		return minus;
	}
	else if (str == "+") {
		return plus;
	}
	else if (str == "/") {
		return slash;
	}
	else if (str == "//" || str == "/*") {
		return comment;
	}
	else {
		return error;
	}
}

int getTypeID() {
	std::string str(lexeme);
	std::size_t found = str.find(".");
	if (found != std::string::npos) {
		return float;
	}
	else {
		return integer;
	}
}

// implementing the ll(1) parse machine
void A1_Parser::ll_parser() {
	Tree *t = new Tree();
	A1_Symbol dollar = A1_Symbol("$", 0, true);
	A1_Symbol start = A1_Symbol("Pg", 10, false);
	s.push(dollar);
	s.push(start);
	getChar();
	int tokId = next_token();

	vector<A1_Node*> temp;
	A1_Node *startNode = new A1_Node(start,"prog", 1, NULL, temp);
	// insert root node pgm into tree
	t->treeInsert(startNode, t->root);

	while (!s.empty()) {
		if (s.top().isTerminal) { // is terminal
			int checkTop = s.top().tokenId;
			if (s.top().tokenId == tokId) { // TOP == FRONT
				s.pop();
				tokId = next_token(); // advance the input
				displayStack(s);
				cout << "Front : " << lexeme << endl;
			}
			else { // error goes here
				cout << "token not defined ";
				cout << "Caused by " << lexeme << endl;
				break;
			}
		} 
		else { // is non terminal
			A1_Symbol tempSymbol = s.top(); // saving top of stack before popping 
			s.pop();
			displayStack(s); // display contents of stack
			int row = searchEnumR(tempSymbol.tokenId);
			int column = searchEnumC(tokId);
			int ruleIden = matrix[row][column];
			if (ruleIden == 0) {
				cout << "rule doesn't exist\n";
				cout << "Caused by " << lexeme << endl;
				break;
			}
			else if (ruleIden != eps) {
				A1_Symbol newSymbol;
				A1_Node *newNode;
				// push the reverse of the rhs of the symbol onto the 
				A1_Rule r = retrieveRule(ruleIden);
				for (size_t i = r.rhs.size(); i-- > 0;) {
					if (r.rhs[i].isTerminal) {
						newSymbol = A1_Symbol(r.rhs[i].Name, r.rhs[i].tokenId, r.rhs[i].isTerminal);
						newNode = new A1_Node(newSymbol, r.lhs.Name, r.ruleId, NULL, temp);
					}
					else {
						newSymbol = A1_Symbol(r.rhs[i].Name, r.rhs[i].tokenId, r.rhs[i].isTerminal);
						newNode = new A1_Node(newSymbol, r.rhs[i].Name, r.ruleId, NULL, temp);
					}
					
					t->treeInsert(newNode, t->root);
					s.push(r.rhs[i]);
				}
				//displayStack(s);
				//cout << "Front : " << lexeme << endl;
			}
		}
	}
	cout << "Front : \n";
}

int searchEnumR(int key) {
	if (key == kwdprog) {
		return Pgm;
	}
	else if (key == SL) {
		return Slist;
	}
	else if (key == Ss) {
		return Stmt;
	}
	else if (key == Y4) {
		return _Y4;
	}
	else if (key == SO) {
		return S_Out;
	}
	else if (key == EL) {
		return Elist;
	}
	else if (key == E2) {
		return Elist2;
	}
	else if (key == E) {
		return Ex;
	}
	else if (key == E_) {
		return _Ex;
	}
	else if (key == T) {
		return Te;
	}
	else if (key == T_) {
		return _Te;
	}
	else if (key == F) {
		return Fe;
	}
	else if (key == Fa) {
		return Fatom;
	}
	else if (key == Oa) {
		return Opadd;
	}
	else {
		return Opmul;
	}
} 


int searchEnumC(int key) {
	if (key == kwdprog) {
		return kp;
	}
	else if (key == brace1) {
		return br1;
	}
	else if (key == brace2) {
		return br2;
	}
	else if (key == semi) {
		return sem;
	}
	else if (key == identifier) {
		return iden;
	}
	else if (key == equal) {
		return eq;
	}
	else if (key == kwdinput) {
		return kwip;
	}
	else if (key == kwdprint) {
		return kwdp;
	}
	else if (key == comma) {
		return com;
	}
	else if (key == parens1) {
		return par1;
	}
	else if (key == parens2) {
		return par2;
	}
	else if (key == integer) {
		return inte;
	}
	else if (key == float) {
		return floa;
	}
	else if (key == STRING) {
		return strn;
	}
	else if (key == plus) {
		return plu;
	}
	else if (key == minus) {
		return minu;
	}
	else if (key == aster) {
		return ast;
	}
	else if (key == slash) {
		return sla;
	}
	else if (key == caret) {
		return car;
	}
	else {
		return dol;
	}
}

