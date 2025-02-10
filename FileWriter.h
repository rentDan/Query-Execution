#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <vector>
#include <sstream>
#include <fstream>

using namespace std;
#include "Record.h"

/*This FileWriter class will be used by the join operator.
When written to, it will initialize a file based on the record structure.
That is, the first line being a comma delimited list of field names,
the second line being a comma delimited list of field types (String Double, Integer),
and the third line being a junk line of dashes.

A write method will be used to write pages(vectors) of records to the file
line by line - comma delimited.
the first three meta data lines might also have to be done with the first
write call.
*/
class FileWriter {
	public:
		ofstream dbFile;

		void open(const string& fileName) {
			//open file in append mode
			dbFile.open(fileName);

			if (!dbFile) {
				cout << "Failed to open file: " << fileName << endl;
				//throw runtime_error("Failed to open file: " + fileName);
			}
		}

		void write(const vector<Record>& pageToWrite, const bool& firstWriteCall) {
			//if it is the first pass through,
			//use the first record to write the metadata + junk line
			if (firstWriteCall) {
				//write names
				for (int i = 0; i < pageToWrite[0].getAttributeNames().size(); i++) {
					dbFile << pageToWrite[0].getAttributeNames()[i];
					if (i != pageToWrite[0].getAttributeNames().size() - 1) {
						dbFile << ",";
					}
				}
				dbFile << endl;

				//write types
				for (int i = 0; i < pageToWrite[0].getAttributeTypes().size(); i++) {
					dbFile << pageToWrite[0].getAttributeTypes()[i];
					if (i != pageToWrite[0].getAttributeTypes().size() - 1) {
						dbFile << ",";
					}
				}
				dbFile << endl;

				//write junk line
				for (int i = 0; i < pageToWrite[0].getAttributeNames().size(); i++) {
					dbFile << "-------";
				}
				dbFile << endl;
			}

			//write records
			//regular loop to use i
			for (const Record& r : pageToWrite) {

				for (int i = 0; i < r.attributes.size(); i++) {
					dbFile << r.attributes[i].value;
					if (i != r.attributes.size() - 1) {
						dbFile << ",";
					}
				}
				dbFile << endl;
			}
		}

		void close() {
			if (dbFile.is_open()) {
				dbFile.close();
			}	
		}
};


#endif