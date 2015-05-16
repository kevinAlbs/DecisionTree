#ifndef TABLE_H
#define TABLE_H
#include <iostream>
#include <map>
#include <vector>
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
	Row readRow(istream& in);
	void reset();
	vector<Row>::size_type size();
	Row getRow(vector<Row>::size_type);
	vector<string> getAttributes(){ return attributes; }
	vector<string> getFeatureAttributes();

private:
	vector<Row> data;
	vector<string> attributes;
	vs_sz classification_index;
	string classification_attribute;
	string separator;
	vector<string> readLine(istream& in);
};
#endif