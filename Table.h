#ifndef TABLE_H
#define TABLE_H
#include <iostream>
#include <map>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include "Row.h"

using namespace std;

/*
Represents the entire table of data. Once created, it should never be modified
*/

class Table{
	typedef vector<string>::size_type vs_sz;
	typedef vector<vector<string> >::size_type vvs_sz;
public:
	Table(istream& in, string classification_attribute, string separator);
	void reset();
	vector<Row>::size_type size();
	Row getRow(vector<Row>::size_type);
	vector<string> getAttributes(){ return attributes; }
	vector<string> getFeatureAttributes();

private:
	Row* readRow(istream& in);
	vector<Row> data;
	vector<string> attributes;
	size_t classification_index;
	string classification_attribute;
	string separator;
	list<string>* readLine(istream& in);
};
#endif