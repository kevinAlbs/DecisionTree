#include <iostream>
#include <fstream>

#include "Table.h"
#include "TableIterator.h"
#include "DecisionTree.h"

using namespace std;

DecisionTree::DecisionTree(string separator, bool no_titles, string classification_attribute){
  this->separator = separator;
  this->no_titles = no_titles;
  this->classification_attribute = classification_attribute;
}

DecisionTree::~DecisionTree(){
  if(this->root != NULL){
    delete this->root;
  }
  if(this->train_table != NULL){
    delete this->train_table;
  }
}

/*
Build decision tree from training file
Returns the number of entries read, or -1 on failure
*/
int DecisionTree::train(string train_filename){
  if(this->root != NULL){
    cerr << "Cannot train a decision tree twice\n";
    return -1;
  }
	ifstream train_file(train_filename);
	if (!train_file.is_open()){
	   cerr << "Could not open '" + train_filename + "'\n";
     return -1;
	}

	train_table = new Table();
	if (!train_table->readLabeled(train_file, separator, no_titles, classification_attribute)){
		cerr << "Could not parse '" << train_filename << "\n";
		train_file.close();
		return -1;
	}
	train_file.close();

	TableIterator train_iter(train_table);
	this->root = new TreeNode(train_iter, train_table->getFeatureAttributes());
	this->root->split();
  return train_table->size();
}

pair<int, int> DecisionTree::testLabeled(string test_filename){
  if(this->root == NULL){
    cerr << "Cannot test data, decision tree has not yet been built\n";
    return pair<int,int>(-1, -1);
  }
  ifstream test_file(test_filename);
  if (!test_file.is_open()){
    cerr << "Could not open '" + test_filename + "'\n";
    return pair<int,int>(-1, -1);
  }
  Table test_table;
  if (!test_table.readLabeled(test_file, separator, no_titles, classification_attribute)){
    cerr << "Could not parse '" << test_filename << "'\n";
    test_file.close();
    return pair<int,int>(-1, -1);
  }
  test_file.close();

  TableIterator test_iter(&test_table);
  int num_correct = 0;
  while (test_iter.hasNext()){
    Row r = test_iter.next();
    if (this->root->classify(r) == r.classification){
      //correct
      num_correct++;
    }
  }
  return pair<int,int>(num_correct, (int)test_table.size());
}

int DecisionTree::classifyUnlabeled(string unlabeled_filename, string output_filename){
  ifstream classify_infile(unlabeled_filename);
  if (!classify_infile.is_open()){
    cerr << "Could not open '" << unlabeled_filename << "'\n";
    return -1;
  }
  Table classify_table;
  if (!classify_table.readUnlabeled(classify_infile, separator, no_titles)){
    cerr << "Could not parse '" << unlabeled_filename << "'\n";
    classify_infile.close();
    return -1;
  }
  classify_infile.close();

  TableIterator classify_iter(&classify_table);

  //write to outfile
  ofstream classify_outfile(output_filename);
  if (!classify_outfile.is_open()){
    cerr << "Could not open '" + output_filename + "'\n";
    return -1;
  }
  while (classify_iter.hasNext()){
    Row r = classify_iter.next();
    string classification = this->root->classify(r);
    for (map<string, string>::const_iterator val_iter = r.data.begin(); val_iter != r.data.end(); val_iter++){
      classify_outfile << val_iter->second << separator;
    }
    classify_outfile << classification << "\n";
  }
  classify_outfile.close();

  return classify_table.size();
}
