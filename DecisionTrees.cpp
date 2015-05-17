#include <iostream>
#include <string>
#include <iterator>
#include <fstream>
#include "Table.h"
#include "TableIterator.h"
#include "TreeNode.h"
#include "Row.h"

using namespace std;

struct Arguments{
	bool help = false;
	bool no_titles = false;
	string train_file = "";
	string test_file = "";
	string classification_attribute = "";
	string separator = " ";
	int classification_index = -1;
};

/*
Assumes each row has the same number of columns
*/
void lineUp(string text[], size_t num_rows, size_t num_cols){
	//get the maximum character count on each column
	typedef string::size_type s_sz;
	vector<s_sz> max_lengths(num_cols);
	for (size_t i = 0; i < num_rows; i++){
		for (size_t j = 0; j < num_cols; j++){
			if (text[i * num_cols + j].length() > max_lengths[j]){
				max_lengths[j] = text[i * num_cols + j].length();
			}
		}
	}
	for (size_t i = 0; i < num_rows; i++){
		for (size_t j = 0; j < num_cols; j++){
			cout << text[i * num_cols + j];
			cout << string(max_lengths[j] - text[i * num_cols + j].length(), ' ');
			cout << "\t";
		}
		cout << "\n";
	}
}
void printHelp(string err=""){
	if (err != ""){
		cout << "Error: " << err << endl;
	}
	cout << "Usage ./DecisionTrees -f <input_file.txt>"
			"[-nt] [-ca <attribute>] [-sp <string>] [-ci <index>] [-h]\n";

	string text[] = {
		"(-tr|--train) <train_file.txt>", "(required) Training input file",
		"(-te|--test) <test_file.txt>", "(required) Testing input file",
		"(-nt|--no_titles)", "If this flag is here, it is assumed that",
		"", "there are no attribute titles in the file. Otherwise it",
		"", "is assumed that the first row is attribute titles",
		"(-ca|--classificaton_attriute) <attribute>", "The name of the attribute for classification"
	};
	lineUp(text, 6, 2);
}

Arguments parseArgs(int argc, char ** argv){
	Arguments args;
	for (int i = 0; i < argc; i++){
		string a = argv[i];
		if (a == "-h" || a == "--help"){
			args.help = true;
		}
		else if (a == "-tr" || a == "--train"){
			//check that next arg is filepath
			if (i == argc - 1){
				throw domain_error("No training file identifier passed");
			}
			args.train_file = argv[++i];
		}
		else if (a == "-te" || a == "--test"){
			//check that next arg is filepath
			if (i == argc - 1){
				throw domain_error("No testing file identifier passed");
			}
			args.test_file = argv[++i];
		}
		else if (a == "-nt" || a == "--no_titles"){
			args.no_titles = true;
		}
		else if (a == "-ca" || a == "--classification_attribute"){
			if (i == argc - 1){
				throw domain_error("No classification attribute passed");
			}
			args.classification_attribute = argv[++i];
		}
		else if (a == "-sp" || a == "--separator"){
			if (i == argc - 1){
				throw domain_error("No separator passed");
			}
			args.separator = argv[++i];
		}
	}
	return args;
}
int main(int argc, char** argv)
{
	Arguments args = parseArgs(argc, argv);
	if (args.help){
		printHelp();
		return 0;
	}
	if (args.train_file == ""){
		printHelp("No valid training file identifier passed");
		return 1;
	}
	if (args.test_file == ""){
		printHelp("No valid testing file identifier passed");
		return 1;
	}
	//read file into table
	ifstream train_file(args.train_file);
	ifstream test_file(args.test_file);
	if (!train_file.is_open()){
		printHelp("Could not open '" + args.train_file + "'");
		return 1;
	}

	cout << "Training data\n";
	Table train_table(train_file, args.classification_attribute, args.separator);
	TableIterator train_iter(&train_table);
	TreeNode decision_tree(train_iter, train_table.getFeatureAttributes());
	decision_tree.split();

	cout << "Testing data\n";
	Table test_table(test_file, args.classification_attribute, args.separator);
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
	return 0;
}
