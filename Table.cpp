#include "Table.h"
#include <time.h>
using namespace std;

/*
Returns a dynamically allocated list representing the delimitted parts of a line
*/
list<string> * Table::readLine(istream& in){
	list<string> *line_parts = new list<string>();
	if (in){
		string line = "";
		string::iterator token_start, token_end;
		//read line
		char c = in.get();
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
*/
Table::Table(istream& in, string classification_attribute, string separator){
	this->classification_attribute = classification_attribute;
	this->separator = separator;

	bool first = true;
	while (in){		
		if (first){
			list<string>* line_parts = readLine(in);
			//Create the list of attributes
			copy(line_parts->begin(), line_parts->end(), back_inserter(this->attributes));
			vector<string>::iterator classification_iter = find(this->attributes.begin(), this->attributes.end(), this->classification_attribute);
			if (classification_iter == attributes.end()){
				throw domain_error("Attribute" + this->classification_attribute + " is not among the list of provided attributes");
			}
			this->classification_index = classification_iter - this->attributes.begin();
			first = false;
			delete line_parts;
		}
		else {
			Row* r = this->readRow(in);
			data.push_back(*r);
			delete r;
		}
	}
}

/*
Returns a dynamically allocated Row.
*/
Row* Table::readRow(istream& in){
	list<string>* line_parts = readLine(in);
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
	delete(line_parts);
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