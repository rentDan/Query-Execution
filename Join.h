#ifndef JOIN_H
#define JOIN_H

#include "Operator.h"
#include "FileReader.h"
#include "FileWriter.h"

class Join : public Operator {
  public:
	  Operator* inputL;
	  Operator* inputR;
	  FileReader frL;
	  FileReader frR;
	  FileWriter fw;
      vector<Record> leftPage;
	  vector<Record> rightPage;
	  int leftIndex;
	  int rightIndex;
	  string attrL;
	  string attrR;
	  bool initInputs = true;
	  bool materialization = false;
	  static inline int tempFileCount;
	  string tempFileName;
	  int leftAttrIndex;
	  int rightAttrIndex;



    Join(Operator *op1, Operator *op2, string pAttr1, string pAttr2) {
		inputL = op1;
		inputR = op2;
		attrL = pAttr1;
		attrR = pAttr2;
		tempFileName = "temp" + to_string(++tempFileCount) + ".data";
		frR.setFileName(tempFileName);
		materialization = true;
    }

    Join(Operator *op, string fileName, string pAttr1, string pAttr2) {
		inputL = op;
		frR.setFileName(fileName);
		inputR = &frR;
		attrL = pAttr1;
		attrR = pAttr2;
    }

    Join(string fileName1, string fileName2, string pAttr1, string pAttr2) {
		frL.setFileName(fileName1);
		inputL = &frL;
		frR.setFileName(fileName2);
		inputR = &frR;
		attrL = pAttr1;
		attrR = pAttr2;
    }

    void open() {
		inputL->open();
		inputR->open();
    }

    vector<Record> next() {
		vector<Record> outPage;

		//if this is the first pass through
		if (initInputs) {
			leftPage = inputL->next();
			rightPage = inputR->next();
			//write the page to the file
			if (materialization) {
				fw.open(tempFileName);
				fw.write(rightPage, true);
			}

			//initialize the indexes
			leftAttrIndex = leftPage[0].getAttrIndex(attrL);
			rightAttrIndex = rightPage[0].getAttrIndex(attrR);

			leftIndex = 0;
			rightIndex = -1;
			initInputs = false;
		}

		//while outpage is not full and there are still records to process
		while (outPage.size() < pageSize) {
	
			if (advanceToNextPairOfRecords()) {
				return outPage;
			}

			Record leftRecord = leftPage[leftIndex];
			Record rightRecord = rightPage[rightIndex];

			//if the attributes match
			if (leftRecord.attributes[leftAttrIndex].value == rightRecord.attributes[rightAttrIndex].value) {
				//combine the records and add it to outpage
				outPage.push_back(leftRecord + rightRecord);
			}
		}
		return outPage;
    }

	//following class code
	 bool advanceToNextPairOfRecords() {
		rightIndex++;

		//once you hit the end of right page
		//increase left index to go again
		if (rightIndex == rightPage.size()) {
			//reset right index and increment left index
			rightIndex = 0;
			leftIndex++;
		}

		//if you hit the end of left page
		if (leftIndex == leftPage.size()) {
			//reset left index 
			leftIndex = 0;
			rightPage = inputR->next();

			if (!rightPage.empty() && materialization) {
				//if materialization, write the page to the file
				fw.write(rightPage, false);
			}

			//when you reach the end of right pages move to next left page
			if (rightPage.empty()) {
				leftPage = inputL->next();

				//if you reach the end of left pages
				if (leftPage.empty()) {
					return true;
				}

				//close the right side and reopen at the top again
				inputR->close();

				//if materialization is true,
				//then change op to the file reader
				if (materialization) {
					materialization = false;
					fw.close();
					inputR = &frR;
				}
				
				inputR->open();
				rightPage = inputR->next();
			}
		}
		return false;
	}

	void close() {
		inputL->close();
		inputR->close();
	}
};


#endif