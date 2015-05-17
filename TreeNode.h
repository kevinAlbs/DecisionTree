#ifndef TREENODE_H
#define TREENODE_H

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include "TableIterator.h"
using namespace std;

/*
Make sure to pass TableIterator as reference when possible
*/
class TreeNode{
	typedef vector<string>::size_type svec_sz;
public:
	TreeNode(){} //so map won't complain
	TreeNode(TableIterator data, vector<string> available_attributes);
	void split();
	string classify(Row& row);
private:
	double info_gain(string attr);
	void make_leaf();
	bool is_leaf = false;
	map<string, TreeNode> children;
	TableIterator ti;
	vector<string> available_attributes;
	string chosen_attribute;
	map<string, string> classification;//set only if leaf <value of attribute -> classification>
	vector<pair<string, double>> leaf_probabilities;
	pair<string, double> most_likely_classification;
};

#endif
