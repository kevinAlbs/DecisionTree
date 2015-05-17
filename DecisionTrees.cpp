#include <iostream>
#include <string>
#include <iterator>
#include <fstream>
#include "Table.h"
#include "TableIterator.h"
#include "TreeNode.h"
#include "Row.h"
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

	/*
	Build decision tree from training file
	*/
	ifstream train_file(args.train_file);
	if (!train_file.is_open()){
		return printHelp("Could not open '" + args.train_file + "'");
	}

	cout << "Building decision tree from file " << args.train_file << "\n";
	Table train_table;
	if (!train_table.readLabeled(train_file, args.separator, args.no_titles, args.classification_attribute)){
		cerr << "Could not parse '" << args.train_file << "\n";
		train_file.close();
		return 1;
	}
	train_file.close();

	TableIterator train_iter(&train_table);
	TreeNode decision_tree(train_iter, train_table.getFeatureAttributes());
	decision_tree.split();

	if (args.test_file != ""){
		ifstream test_file(args.test_file);
		if (!test_file.is_open()){
			printHelp("Could not open '" + args.test_file + "'");
			return 1;
		}
		cout << "Testing data from file '" << args.test_file << "'\n";
		Table test_table;
		if (!test_table.readLabeled(test_file, args.separator, args.no_titles, args.classification_attribute)){
			cerr << "Could not parse '" << args.test_file << "'\n";
			test_file.close();
			return 1;
		}
		test_file.close();

		TableIterator test_iter(&test_table);
		unsigned int num_correct = 0;
		while (test_iter.hasNext()){
			Row r = test_iter.next();
			if (decision_tree.classify(r) == r.classification){
				//correct
				num_correct++;
			}
		}

		double percent_correct = 100.0 * ((double)num_correct) / ((double)test_table.size());
		cout << "Results: " << num_correct << " of " << test_table.size() << " correct\n"
			<< "Percentage correct: " << percent_correct << "\n";
	}

	if (args.classify_infile != ""){
		cout << "Classifying unlabeled data from '" << args.classify_infile << "'\n";
		ifstream classify_infile(args.classify_infile);
		if (!classify_infile.is_open()){
			printHelp("Could not open '" + args.classify_infile + "'");
			return 1;
		}
		Table classify_table;
		if (!classify_table.readUnlabeled(classify_infile, args.separator, args.no_titles)){
			cerr << "Could not parse '" << args.test_file << "'\n";
			classify_infile.close();
			return 1;
		}
		classify_infile.close();

		TableIterator classify_iter(&classify_table);

		//write to outfile
		ofstream classify_outfile(args.classify_outfile);
		if (!classify_outfile.is_open()){
			printHelp("Could not open '" + args.classify_outfile + "'");
			return 1;
		}
		while (classify_iter.hasNext()){
			Row r = classify_iter.next();
			string classification = decision_tree.classify(r);
			for (map<string, string>::const_iterator val_iter = r.data.begin(); val_iter != r.data.end(); val_iter++){
				classify_outfile << val_iter->second << args.separator;
			}
			classify_outfile << classification << "\n";
		}
		cout << "Results stored in '" << args.classify_outfile << "'\n";
	}

	return 0;
}
