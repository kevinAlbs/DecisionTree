#include "TableIterator.h"

using namespace std;

TableIterator::TableIterator(Table *t){
	for (vr_sz i = 0; i < t->size(); i++){
		indices.push_back(i);
	}
	current = 0;
	this->t = t;
}

TableIterator::TableIterator(Table *t, vector<vr_sz> indices){
	this->t = t;
	this->indices = indices;
	current = 0;
}

Row TableIterator::next(){
	if (!hasNext()){
		throw domain_error("TableIterator out of bounds");
	}
	Row r = t->getRow(indices[current]);
	current++;
	return r;
}

bool TableIterator::hasNext(){
	return current < indices.size();
}
map<string, TableIterator> TableIterator::split(string attr){
	map<string, vector<vr_sz>> val_to_indices;
	for (vvr_sz i = 0; i < indices.size(); i++){
		vr_sz table_index = indices[i];
		Row r = t->getRow(table_index);
		string value = r.data[attr];
		val_to_indices[value].push_back(table_index);
	}
	map<string, TableIterator> ret;
	//go through each resulting vector of indices, and create a new TableIterator
	for (map<string, vector<vr_sz> >::const_iterator iter = val_to_indices.begin(); iter != val_to_indices.end(); iter++){
		TableIterator ti(t, iter->second);
		ret[iter->first] = ti;
	}
	return ret;
}

/*
Computes Shannon Entropy for a given TableIterator (i.e. subset of table)
*/
double TableIterator::entropy(){
	map<string, int> class_to_count;
	double table_size = indices.size();
	for (vvr_sz i = 0; i < indices.size(); i++){
		vr_sz table_index = indices[i];
		Row r = t->getRow(table_index);
		string classification = r.classification;
		class_to_count[classification] += 1;
	}
	double sum = 0;
	for (map<string, int>::const_iterator iter = class_to_count.begin(); iter != class_to_count.end(); iter++){
		double prob = iter->second / table_size;
		double log_prob = log2(prob);
		sum += prob * log_prob;
	}
	return -1 * sum;
}
/*
Returns a vector of pairs of classifications and their probabilities
*/
vector<pair<string, double> > TableIterator::getProbabilities(){
	map<string, int> counts;
	vector<pair<string, double> > probs;
	for (vvr_sz i = 0; i < indices.size(); i++){
		vr_sz table_index = indices[i];
		Row r = t->getRow(table_index);
		counts[r.classification]++;
	}
	for (map<string, int>::const_iterator count_iter = counts.begin(); count_iter != counts.end(); count_iter++){
		probs.push_back(pair<string, double>(count_iter->first, (double)count_iter->second / this->indices.size() ));
	}
	return probs;
}

void TableIterator::reset(){
	current = 0;
}