#ifndef TABLEITERATOR_H
#define TABLEITERATOR_H
#include <vector>
#include <map>
#include "Table.h"
#include "Row.h"
using namespace std;

/*
TODO: use a vector of indices to maintain references to the original table
Split this vector when splitting
*/
class TableIterator {
	typedef vector<Row>::size_type vr_sz;
	typedef vector<vector<Row> >::size_type vvr_sz;
public:
	TableIterator(){}//so map doesn't complain
	TableIterator(Table *t);
	TableIterator(Table *t, vector<vr_sz> indices);
	Row next();
	bool hasNext();
	void reset();
	map<string, TableIterator> split(string attr);
	vr_sz size(){ return indices.size(); }
	double entropy();
	vector<pair<string, double> >getProbabilities();
private:
	vr_sz current;
	vector<vr_sz> indices;
	Table *t;
};

#endif
