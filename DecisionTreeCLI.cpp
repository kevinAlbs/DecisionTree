#include <iostream>
#include <string>
#include <iterator>
#include <fstream>

#include "Table.h"
#include "TableIterator.h"
#include "TreeNode.h"
#include "Row.h"
#include "DecisionTree.h"
#include "utils.h"
#include <cmath>

using namespace std;

int main(int argc, char** argv)
{
	Arguments args = parseArgs(argc, argv);
	if (args.help){
		return printHelp();
	}
	if (args.train_file == ""){
		return printHelp("No valid training file identifier passed");
	}

	cout << "Building decision tree from file " << args.train_file << "\n";

	DecisionTree tree(args.separator, args.no_titles, args.classification_attribute);
	tree.train(args.train_file);

	if (args.test_file != ""){
		cout << "Testing data from file '" << args.test_file << "'\n";

		pair<int,int> results = tree.testLabeled(args.test_file);

		double percent_correct = 100.0 * ((double)results.first) / ((double)results.second);
		cout << "Results: " << results.first << " of " << results.second << " correct\n"
			<< "Percentage correct: " << percent_correct << "\n";
	}

	if (args.classify_infile != ""){
		cout << "Classifying unlabeled data from '" << args.classify_infile << "'\n";
		tree.classifyUnlabeled(args.classify_infile, args.classify_outfile);
		cout << "Results stored in '" << args.classify_outfile << "'\n";
	}
	return 0;
}
