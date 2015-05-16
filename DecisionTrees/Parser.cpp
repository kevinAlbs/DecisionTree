#include "stdafx.h"
#include <iostream>
#include <string>
#include <ios>
#include <vector>

using namespace std;

template<class T> void split(string s, char c, T o){
	string::iterator word_begin = s.begin();
	string::iterator word_end = word_begin;
	while (word_begin != s.end()){
		//remove inital characters
		while (*word_begin == c){
			word_begin++;
		}
		if (word_begin == s.end()){
			break;
		}
		word_end = find(word_begin, s.end(), c);
		string output;
		copy(word_begin, word_end, back_inserter(output));
		*o++ = string(word_begin, word_end);
		word_begin = word_end;
	}
}

void parse(istream& in, vector<string>& titles){
	//first line is titles
	string line;
	if (getline(in, line)){
		split(line, ' ', back_inserter(titles));
	}
	else {
		throw domain_error("Title line does not exist");
	}
}