#ifndef FILEREADER_H
#define FILEREADER_H

#include <vector>
#include <sstream>
#include <fstream>
using namespace std;
#include "Record.h"
#include "Operator.h"


// This function takes in a string with items delimited by commas
// and returns a vector of those items.
// Example - input: "str1,str2,str3,str4"
//          output: {"str1", "str2", "str3", "str4"}
vector<string> split(string line) {
  vector<string> v;
  string s;
  istringstream is(line);
  while (getline(is, s, ','))
    v.push_back(s);
  return v;
}

class FileReader : public Operator {
  public:
    vector<string> names;
    vector<string> types;
    ifstream dbFile;
    string fileName;
    string valuesToParse;

    void open() {
        //call .open(fileName) on your ifstream object
        dbFile.open(fileName);
        if(!dbFile) {
            throw runtime_error("Failed to open file: " + fileName);
        }
        
        //read/parse names
        string stringToParse;
        getline(dbFile, stringToParse);
        names = split(stringToParse);

        //read/parse types
        getline(dbFile, stringToParse);
        types = split(stringToParse);

        //read junk line
        getline(dbFile, stringToParse);
    }

    vector<Record> next() {
      vector<Record> outPage;
      vector<string> parsedValues;

      //try to read records pagesize(4) times
      for(int i=0; i < pageSize && getline(dbFile, valuesToParse); i++) {
    
        parsedValues = split(valuesToParse);

        Record record(names, types, parsedValues);
        //put them in outPage
        outPage.push_back(record);
       
        //If clear just changes size to 0, then this should be better than
        //initializing new vectors each iteration
        parsedValues.clear();
      }

      // Return an empty vector if no more pages exist
      return outPage;
    }

    void setFileName(const string& fileNameGiven) {
        fileName = fileNameGiven;
    }

    void close() {
      // Implement this
        //call close on ifstream object
        dbFile.close();
    }
};

#endif
