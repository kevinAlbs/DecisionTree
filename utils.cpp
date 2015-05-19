#include <iostream>
#include <string>
#include <iterator>
#include <vector>
#include "utils.h"

using namespace std;

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

int printHelp(string err){
	if (err != ""){
		cout << "Error: " << err << endl;
	}
	cout << "Usage ./DecisionTrees -tr <file> -te <file> -cl <file>"
			"[-nt] [-ca <attribute>] [-sp <string>] [-ci <index>] [-h]\n";

	string text[] = {
		"(-tr|--train) <train_file>", "(required) Labeled training input file",
		"(-te|--test) <test_file>", "(required) Labeled testing input file",
		"(-cl|--classify) <infile> <outfile>", "Unlabeled input file to be classified",
		"", "Resulting classified data is saved in <output_file>",
		"-sp", "Delimitter of attribute values, default is space",
		"(-nt|--no_titles)", "If this flag is here, it is assumed that",
		"", "there are no attribute titles in the file. Otherwise it",
		"", "is assumed that the first row is attribute titles",
		"(-ca|--classificaton_attriute) <attribute>", "The name of the attribute for classification.",
		"", "If omitted, it is assumed to be the last attribute"
	};
	lineUp(text, 10, 2);
	if(err != ""){
		return 1;
	} else {
		return 0;
	}
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
		else if (a == "-cl" || a == "--classify"){
			//check that next arg is filepath
			if (i == argc - 1 || i == argc - 2){
				throw domain_error("Incorrect specification for classify file, need input and output file");
			}
			args.classify_infile = argv[++i];
			args.classify_outfile = argv[++i];
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
