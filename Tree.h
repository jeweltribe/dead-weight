#include "A1_Node.h"
#include "A1_Symbol.h"

class Tree {
public:
	A1_Node *root;

	Tree() { root = NULL; }

	void treeInsert(A1_Node *newNode, A1_Node *treeNode) {
		if (root == NULL) {
			root = newNode;
		}
		else {
			if (treeNode) {
				if (newNode->momID == treeNode->momID || newNode->ruleID == treeNode->ruleID) { // lollipop
					newNode->mom = treeNode; // set the mom node for the newNode
					treeNode->children.push_back(newNode);

				}
				else {
					for (unsigned int i = 0; i < treeNode->children.size(); i++) {
						treeInsert(newNode, treeNode->children[i]);
					}
				}
			}
			else {
				return;
			}
		}
		
	}
};