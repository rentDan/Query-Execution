#ifndef RECORD_H
#define RECORD_H

#include <iostream>
#include <string>
using namespace std;

class Attribute {
    public:
        string name;
        string type;
        string value;

        Attribute(string rname, string rtype, string rvalue) {
            name = rname;
            type = rtype;
            value = rvalue;            
        }

};

class Record {
  // Write code to store/handle an arbitrary number of
  //   - Attribute Names
  //   - Attribute Types
  //   - Attribute Values
    public:
        vector<Attribute> attributes;

        Record() {
        }

        Record(vector<string> names, vector<string> types, vector<string> values) {
			for (int i = 0; i < names.size(); i++) {
				Attribute a(names[i], types[i], values[i]);
				attributes.push_back(a);
			}
        }

        void addAttribute(const Attribute& a) {
            attributes.push_back(a);
        }

		Attribute getAttribute(const string& name) {
            int i;
            for (i = 0; i < attributes.size(); i++) {
                if (name == attributes[i].name)
                    break;
            }
			
            return attributes[i];
		}

        bool selectPredicate(const string& attr, const string& oper, const string& val, const int& attrIndex) {
			Attribute a = attributes[attrIndex];
			//string aVal = a.value;
			//
            // cout << "aVal: " << aVal << endl;
            //convert val based on a.type (String, Integer, Double)
            if (a.type == "Integer") {
                int aVal = stoi(a.value);
                double pVal = stoi(val);
                switch (oper[0]) {
                case '=':
                    return aVal == pVal;
                case '<':
                    return aVal < pVal;
                case '>':
                    return aVal > pVal;
                case '!':
                    return aVal != pVal;
                default:
                    throw invalid_argument("Invalid operator");
                }
			} else if (a.type == "Double") {
				double aVal = stod(a.value);
                double pVal = stod(val);
                switch (oper[0]) {
                case '=':
                    return aVal == pVal;
                case '<':
                    return aVal < pVal;
                case '>':
                    return aVal > pVal;
                case '!':
                    return aVal != pVal;
                default:
                    throw invalid_argument("Invalid operator");
                }
			} else {
                string aVal = a.value;
                switch (oper[0]) {
                case '=':
                    return aVal == val;
                case '<':
                    return aVal < val;
                case '>':
                    return aVal > val;
                case '!':
                    return aVal != val;
                default:
                    throw invalid_argument("Invalid operator");
                }
            }

			//cout << "aVal: " << aVal << endl;

            //compare aVal with val based on oper
			/*
            switch (oper[0]) {
			case '=':
				return aVal == val;
			case '<':
				return aVal < val;
			case '>':
				return aVal > val;
			case '!':
				return aVal != val;
            default:
				throw invalid_argument("Invalid operator");
			}
            */
        }

        vector<string> getAttributeNames() const {
            vector<string> names;
            for (const Attribute& a : attributes) {
                names.push_back(a.name);
            }
			return names;
        }

        vector<string> getAttributeTypes() const {
            vector<string> types;
            for (const Attribute& a : attributes) {
                types.push_back(a.type);
            }
			return types;
        }

		int getAttrIndex(const string& name) const {
			for (int i = 0; i < attributes.size(); i++) {
				if (name == attributes[i].name) {
					return i;
				}
			}
			return -1;
		}

        friend ostream& operator<< (ostream& os, const Record& r) {
            // Implement this to write a string to os displaying all attribute values
            // Print a space-delimited list of attribute vals with a space after
            // each of the values. Example: "val1 val2 val3 "
            for(const Attribute& a : r.attributes) {
                os << a.value << " ";
            }
            
            // Finally, return os
            return os;
        }

        friend Record operator+ (const Record& rL, const Record& rR) {
            Record r;
            for (const Attribute& a : rL.attributes) {
				r.attributes.push_back(a);
            }
			for (const Attribute& a : rR.attributes) {
				r.attributes.push_back(a);
			}
			return r;
        }
};

#endif
