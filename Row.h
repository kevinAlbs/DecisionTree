#ifndef ROW_H
#define ROW_H
#include <map>
using namespace std;
struct Row{
	map<string, string> data;
	string classification;
};

void printRow(Row r);

#endif