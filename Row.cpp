#include "Row.h"
#include <iostream>
#include <string>
#include <map>

using namespace std;
void printRow(Row r){
	for (map<string, string>::iterator mi = r.data.begin(); mi != r.data.end(); mi++){
		cout << "[" << mi->first << " => " << mi->second << "] ";
	}
	cout << "\n Classification: " << r.classification << "\n";
}