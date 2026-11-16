// FileHandler.cpp
// CA2: binary file functions using fstream

#include "FileHandler.h"
#include <fstream>

// Read every record at program start so the data survives a restart
int loadAllDonors(string fileName, Donor donors[], int maxCount) {
    ifstream file(fileName, ios::binary);
    if (!file)
        return 0;   // no file yet (first run)

    DonorRecord r;
    int count = 0;
    while (count < maxCount && file.read((char *)&r, sizeof(r))) {
        donors[count].fromRecord(r);
        count++;
    }
    file.close();
    return count;
}

// Add one donor at the end of the file
bool saveDonor(string fileName, const Donor &d) {
    ofstream file(fileName, ios::binary | ios::app);
    if (!file)
        return false;

    DonorRecord r = d.toRecord();
    file.write((char *)&r, sizeof(r));
    file.close();
    return true;
}

// Number of records = file size / size of one record
int countRecords(string fileName) {
    ifstream file(fileName, ios::binary);
    if (!file)
        return 0;
    file.seekg(0, ios::end);              // go to the end of the file
    long bytes = (long)file.tellg();      // position at the end = file size
    file.close();
    return (int)(bytes / sizeof(DonorRecord));
}

// Search by ID: jump to each record with seekg and compare the ID
int findPositionInFile(string fileName, string id) {
    ifstream file(fileName, ios::binary);
    if (!file)
        return -1;

    int total = countRecords(fileName);
    DonorRecord r;
    for (int i = 0; i < total; i++) {
        file.seekg(i * sizeof(DonorRecord), ios::beg);
        file.read((char *)&r, sizeof(r));
        if (id == r.id) {
            file.close();
            return i;
        }
    }
    file.close();
    return -1;
}

// Read only the record at this position
bool readDonorAt(string fileName, int position, Donor &d) {
    ifstream file(fileName, ios::binary);
    if (!file)
        return false;

    DonorRecord r;
    file.seekg(position * sizeof(DonorRecord), ios::beg);
    if (!file.read((char *)&r, sizeof(r))) {
        file.close();
        return false;
    }
    d.fromRecord(r);
    file.close();
    return true;
}

// Overwrite only the record at this position, the rest of the file is not touched
bool updateDonorAt(string fileName, int position, const Donor &d) {
    fstream file(fileName, ios::binary | ios::in | ios::out);
    if (!file)
        return false;

    DonorRecord r = d.toRecord();
    file.seekp(position * sizeof(DonorRecord), ios::beg);
    file.write((char *)&r, sizeof(r));
    bool ok = !file.fail();
    file.close();
    return ok;
}
