#include "TreeNode.h"
#include <limits>
using namespace std;

TreeNode::TreeNode(TableIterator data, vector<string> available_attributes){
	this->available_attributes = available_attributes;
	this->ti = data;
}

void TreeNode::split(){
	//if there is only one available attribute, make this into a leaf
	if (this->available_attributes.size() == 0){
		cout << "\tNo attributes left, making leaf\n";
		make_leaf();
		return;
	}
	vector<string> aa = this->available_attributes;
	vector<pair<string, double> > sums;
	/*
	Compute information gain on available attributes, split on max
	INFO_GAIN = CURRENT_ENTROPY - SPLIT_ENTROPY
	For optimization, compute the minimum SPLIT_ENTROPY only
	*/
	for (vector<string>::iterator vi = aa.begin(); vi != aa.end(); vi++){
		map<string, TableIterator> split_tables = this->ti.split(*vi);
		double sum = 0;
		// for each table, compute ratio * entropy
		for (map<string, TableIterator>::const_iterator sub_iter = split_tables.begin(); sub_iter != split_tables.end(); sub_iter++){
			TableIterator sub_ti = sub_iter->second;
			double prob = (double)sub_ti.size() / (double)this->ti.size();
			double ent = sub_ti.entropy();
			sum += prob * ent;
		}
		sums.push_back(pair<string, double>(*vi, sum));
	}
	double current_entropy = this->ti.entropy();
	double min_sum = -1;
	string min_attr = "";
	for (vector<pair<string, double> >::const_iterator pi = sums.begin(); pi != sums.end(); pi++){
		if (min_sum == -1 || pi->second < min_sum){
			min_attr = pi->first;
			min_sum = pi->second;
		}
	}
	if (min_sum == current_entropy){
		//no info gain, do not split
		cout << "\tNo info gain, this is a leaf\n";
		this->make_leaf();
		return;
	}

	cout << "\tSplitting on attribute " << min_attr 
		 << " with new entropy " << min_sum 
		 << " info_gain = " << current_entropy << " - " << min_sum << " = " << (current_entropy - min_sum) 
		 << "\n";
	
	this->chosen_attribute = min_attr;
	//create a child for each value in the TableIterator
	vector<string> new_attributes = available_attributes; //copies over
	new_attributes.erase(find(new_attributes.begin(), new_attributes.end(), this->chosen_attribute));
	cout << "\tRemoved " << chosen_attribute << "\n";
	cout << "\t";
	copy(new_attributes.begin(), new_attributes.end(), ostream_iterator<string>(cout, " "));
	cout << "\n";
	map<string, TableIterator> split_tables = this->ti.split(min_attr);
	map<string, TreeNode> child_nodes;
	for (map<string, TableIterator>::const_iterator split_iter = split_tables.begin(); split_iter != split_tables.end(); split_iter++){
		string attr_value = split_iter->first;
		child_nodes[attr_value] = TreeNode(split_iter->second, new_attributes);
		cout << "Splitting attribute " << min_attr << " of value " << attr_value << "\n";
		child_nodes[attr_value].split();
		cout << "Is leaf? " << child_nodes[attr_value].is_leaf << "\n";
	}

	this->children = child_nodes;

}
string TreeNode::classify(Row& row){
	cout << "Classification for node with attribute " << chosen_attribute << "\n"
		<< "Is it a leaf? " << is_leaf << "\n"
		<< "# children " << this->children.size() << "\n";

	//if this is a leaf node, return the classification
	if (is_leaf){
		//return classification with highest probability
		cout << "Confidence " << most_likely_classification.second << "\n";
		return most_likely_classification.first;
	}
	else {
		string attr_val = row.data[chosen_attribute];
		cout << "Attribute value " << attr_val << "\n";
		if (this->children.find(attr_val) == this->children.end()){
			cerr << "Cannot classify, contains unseen value " << attr_val
				<< " for attribute " << chosen_attribute << "\n";
			return "";
		}
		else {
			return this->children[attr_val].classify(row);
		}
	}
}

/*
Computes conditional entropy for splitting on an attribute
*/
double TreeNode::info_gain(string attr){
	printf("Computing entropy for attribute %s\n", attr);
	map<string, TableIterator> val_to_table = ti.split(attr);
	double current_entropy = ti.entropy();
	double entropy_sum = 0;
	//for each value, compute probability * entropy of that fixed value
	for (map<string, TableIterator>::const_iterator iter = val_to_table.begin(); iter != val_to_table.end(); iter++){
		string val = iter->first;
		TableIterator sub_ti = iter->second;
		double ratio = sub_ti.size() / ti.size();
		double ent = sub_ti.entropy();
		entropy_sum += ratio * ent;
	}
	cout << current_entropy - entropy_sum << "\n";
	return current_entropy - entropy_sum;
}

void TreeNode::make_leaf(){
	//compute probabilities of classifications in contained TableIterator
	leaf_probabilities = this->ti.getProbabilities();
	double max_prob = 0;
	string max_val = "";
	for (vector<pair<string, double> >::const_iterator iter = leaf_probabilities.begin(); iter != leaf_probabilities.end(); iter++){
		string attr = iter->first;
		double prob = iter->second;
		cout << "Attr: " << attr << " " << prob << "\n";
		if (prob > max_prob){
			max_prob = prob;
			max_val = attr;
		}
	}
	most_likely_classification = pair<string, double>(max_val, max_prob);
	is_leaf = true;
}