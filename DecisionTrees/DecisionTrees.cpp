#include "stdafx.h"
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
	string file = "";
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
		"(-f|--file) <input_file.txt>", "(required) Input file",
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
		else if (a == "-f" || a == "--file"){
			//check that next arg is filepath
			if (i == argc - 1){
				throw domain_error("No file identifier passed");
			}
			args.file = argv[++i];
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
	if (args.file == ""){
		printHelp("No valid input file identifier passed");
	}
	//read file into table
	ifstream in_file(args.file);
	if (!in_file.is_open()){
		printHelp("Could not open '" + args.file + "'");
	}

	Table t(in_file, args.classification_attribute, args.separator);
	TableIterator ti(&t);
	TreeNode root(ti, t.getFeatureAttributes());
	root.split();

	Row r;
	map<string, string> data;
	data["buying"] = "vhigh";
	data["maint"] = "vhigh";
	data["doors"] = "2";
	data["persons"] = "4";
	data["lug_boot"] = "small";
	data["safety"] = "low";
	r.data = data;
	cout << "Classifying row as " << root.classify(r) << "\n";
	return 0;
	

}