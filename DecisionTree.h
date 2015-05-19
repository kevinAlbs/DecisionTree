#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include "Table.h"
#include "TreeNode.h"

using namespace std;
class DecisionTree{
public:
  DecisionTree(string separator, bool no_titles, string classification_attribute);
  ~DecisionTree();
  int train(string train_filename);
  pair<int, int> testLabeled(string test_filename);
  int classifyUnlabeled(string unlabeled_filename, string output_filename);
private:
  Table *train_table = NULL;
  TreeNode *root = NULL;
  string separator;
  bool no_titles;
  string classification_attribute;
};
#endif
