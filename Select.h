#ifndef SELECT_H
#define SELECT_H

#include <vector>
using namespace std;
#include "Record.h"
#include "Operator.h"
#include "FileReader.h"

class Select : public Operator {
  public:
	  Operator* input;
      FileReader fr;
      vector<Record> currPage;
      int currIndex;
	  string attr;
	  string oper;
	  string val;
	  int attrIndex;
      bool firstTime = true;

    // Select constructors
    Select(string fileName, string p_attr, string p_op, string p_val) {
		fr.setFileName(fileName);
		input = &fr;
		attr = p_attr;
		oper = p_op;
		val = p_val;
    }
    Select(Operator *op, string p_attr, string p_op, string p_val) {
		input = op;
		attr = p_attr;
		oper = p_op;
		val = p_val;
    }

    void open() {
		input->open();
		currIndex = 0;
    }

    vector<Record> next() {
      vector<Record> outPage;
      // Implement this

      //while outPage not full
      while (outPage.size() < pageSize) {

		  //if currIndex is 0 or currIndex is at end of currPage
          //get next page
          if (currIndex == 0 || currIndex == currPage.size()) {
              currPage = input->next();
              currIndex = 0;
          }
          
		  //if empty page - break
          if (currPage.empty()) {
              break;
          }

          Record r = currPage[currIndex];

          //if first time get attribute index
		  if (firstTime) {
              attrIndex = r.getAttrIndex(attr);
			  firstTime = false;
		  }

          //if predicate is true add to outPage
          if (r.selectPredicate(attr, oper, val, attrIndex)) {
            outPage.push_back(r);
          }
            currIndex++;
      }
	  return outPage;
    }

    void close() {
		input->close();
    }
};

#endif
