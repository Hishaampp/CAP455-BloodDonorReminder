// FileHandler.h
// CA2: saving and reading donors from the binary file data/donors.dat
// Every donor takes the same number of bytes (sizeof(DonorRecord)),
// so donor number n starts at byte n * sizeof(DonorRecord).

#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include "Donor.h"
using namespace std;

const string DATA_FILE = "data/donors.dat";

int loadAllDonors(string fileName, Donor donors[], int maxCount);   // returns how many were read
bool saveDonor(string fileName, const Donor &d);                    // add at the end of the file
int findPositionInFile(string fileName, string id);                 // seekg search, -1 if not found
bool readDonorAt(string fileName, int position, Donor &d);          // seekg to one record
bool updateDonorAt(string fileName, int position, const Donor &d);  // seekp to overwrite one record
int countRecords(string fileName);

#endif
