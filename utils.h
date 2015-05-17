#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>

using namespace std;

struct Arguments{
	bool help = false;
	bool no_titles = false;
	string train_file = "";
	string test_file = "";
	string classify_infile = "";
	string classify_outfile = "";
	string classification_attribute = "";
	string separator = " ";
	int classification_index = -1;
};

void lineUp(string text[], size_t num_rows, size_t num_cols);
int printHelp(string err="");
Arguments parseArgs(int argc, char ** argv);

#endif
