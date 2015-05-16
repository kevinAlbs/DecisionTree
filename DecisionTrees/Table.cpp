#include "Table.h"

using namespace std;

vector<string> Table::readLine(istream& in){
	vector<string> line_parts;
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
				line_parts.push_back(part);
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
Reads a data table.
*/
Table::Table(istream& in, string classification_attribute, string separator){
	this->classification_attribute = classification_attribute;
	this->separator = separator;

	bool first = true;
	while (in){		
		if (first){
			vector<string> line_parts = readLine(in);
			//Create the list of attributes
			this->attributes = line_parts;
			vector<string>::iterator classification_iter = find(this->attributes.begin(), this->attributes.end(), this->classification_attribute);
			if (classification_iter == attributes.end()){
				throw domain_error("Attribute" + this->classification_attribute + " is not among the list of provided attributes");
			}
			this->classification_index = classification_iter - this->attributes.begin();
			first = false;
		}
		else {
			Row r = this->readRow(in);
			data.push_back(r);
		}
	}
}

/*
Reads a single row. This can only be called after the attributes have been read.
The classification attribute is optional.
*/
Row Table::readRow(istream& in){
	vector<string> line_parts = readLine(in);
	Row r;
	map<string, string> m;
	for (vs_sz i = 0; i < line_parts.size(); i++){
		if (i == this->classification_index){
			r.classification = line_parts[i];
		}
		else {
			m[this->attributes[i]] = line_parts[i];
		}
	}
	r.data = m;
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