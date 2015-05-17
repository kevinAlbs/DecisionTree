#include "Table.h"
#include <time.h>
using namespace std;

/*
Returns a dynamically allocated list representing the delimitted parts of a line
If there is no more data, only comments/newlines, it returns an empty list
*/
list<string> * Table::readLine(istream& in){
	list<string> *line_parts = new list<string>();
	int line_num = 1;
	if (in){
		string line = "";
		string::iterator token_start, token_end;
		//skip lines starting with comment # or newlines
		char c = in.get();
		while(c == '#' || c == '\n'){
			while (c != '\n' && c != EOF){
				c = in.get();
			}
			//if we reached end, there's nothing to return
			if(!in){
				return line_parts;
			}
			line_num++;
			c = in.get();
		}
		if(!in){
			return line_parts;
		}
		while (c != '\n' && c != EOF){
			line += c;
			c = in.get();
		}
		//split the line
		token_start = line.begin();
		token_end = token_start;
		while (token_end < line.end()){
			token_end = search(token_start, line.end(), separator.begin(), separator.end());
			if (token_end == token_start){
				token_start += (separator.end() - separator.begin());
			}
			else{
				string part(token_start, token_end);
				line_parts->push_back(part);
			}
			if (token_end == line.end()){
				break;
			}
			token_start = token_end + (separator.end() - separator.begin());
		}
	}
	return line_parts;
}

/*
Reads a data table with a classification attribute.
Returns true on successful read, false on failure.
*/
bool Table::readLabeled(istream& in, string separator, bool no_titles, string classification_attribute){
	return this->read(in, separator, no_titles, classification_attribute, true);
}

bool Table::readUnlabeled(istream& in, string separator, bool no_titles){
	return this->read(in, separator, no_titles, classification_attribute, false);
}

bool Table::read(istream& in, string separator, bool no_titles, string classification_attribute, bool labeled){
	this->separator = separator;
	bool first = true;
	int line_num = 1;
	list<string>* line_parts = readLine(in);
	while (line_parts->size() > 0){
		//copy(line_parts->begin(), line_parts->end(), ostream_iterator<string>(cout, " "));
		//cout << "\n";
		if (first){
			if (no_titles){
				//create placeholder attributes
				for (int i = 1; i <= line_parts->size(); i++){
					this->attributes.push_back("attribute_" + i);
				}
			}
			else{
				//read attribute names
				copy(line_parts->begin(), line_parts->end(), back_inserter(this->attributes));
			}

			if (labeled) {
				if (classification_attribute == "" || no_titles){
					//last attribute is classification attribute
					this->classification_attribute = this->attributes[this->attributes.size() - 1];
				}
				else {
					this->classification_attribute = classification_attribute;
				}
				this->classification_index = find(this->attributes.begin(), this->attributes.end(), this->classification_attribute) - this->attributes.begin();
				if (this->classification_index >= this->attributes.size()){
					//ensure it is among the attributes
					cerr << "Attribute" + this->classification_attribute + " is not among the list of provided attributes";
					return false;
				}
			}
		}

		if (!first || no_titles) {
			if (line_parts->size() != this->attributes.size()){
				cerr << "Line " << line_num << " has a different number of attributes\n";
				return false;
			}
			Row* r = this->makeRow(line_parts);
			data.push_back(*r);
			delete r;
		}

		first = false;
		line_num++;
		delete line_parts;
		line_parts = readLine(in);
	}
	return true;
}

/*
Returns a dynamically allocated Row.
*/
Row* Table::makeRow(list<string>* line_parts){
	Row* r = new Row();
	map<string, string> m;
	size_t index = 0;
	for (list<string>::const_iterator iter = line_parts->begin(); iter != line_parts->end(); iter++){
		if (index == this->classification_index){
			r->classification = *iter;
		}
		else {
			m[this->attributes[index]] = *iter;
		}
		index++;
	}
	r->data = m;
	return r;
}

Row Table::getRow(vector<Row>::size_type i){
	return data[i];
}

vector<Row>::size_type Table::size(){
	return data.size();
}

vector<string> Table::getFeatureAttributes(){
	vector<string> feature_attr = attributes;
	feature_attr.erase(find(feature_attr.begin(), feature_attr.end(), this->classification_attribute));
	return feature_attr;
}
