#ifndef PROJECT_H
#define PROJECT_H

#include <vector>
using namespace std;
#include "Record.h"
#include "Operator.h"
#include "FileReader.h"

class Project : public Operator {
    
  public:

    vector<string> columnNamesToKeep;
    Operator* input;
    FileReader fr;

    // Project constructors
    Project(string fileName, vector<string> fieldNames) {
        columnNamesToKeep = fieldNames;
        fr.setFileName(fileName);
        input = &fr;
    }

    Project(Operator *inputOp, vector<string> fieldNames) {
        columnNamesToKeep = fieldNames;
        input = inputOp;
    }

    void open() {
        input->open();
    }

    vector<Record> next() {
      vector<Record> outPage;

      //get inPage from below via in->next()
      vector<Record> inPage = input->next();

      //for each record in inPage create an outRecord
      //containing just attributes we want to keep
      for(const Record& r: inPage) {
        Record outRecord;
        for(const string& columnName: columnNamesToKeep) {
            for(const Attribute& a: r.attributes) {
                if(a.name == columnName) {
                    outRecord.addAttribute(a);
                }
            }
        }
       
        //push outRecord into outPage
        outPage.push_back(outRecord);
      }

      // Return an empty vector if no more pages exist
      return outPage;
    }

    void close() {
        input->close();
    }
};

#endif
