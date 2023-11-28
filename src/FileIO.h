#ifndef FILEIO_H
#define FILEIO_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include "Block.h"  

using namespace std;

class FileIO {
public:
    void readFromStream(BlockInfo& block, map<char, string>& tagTable, vector<vector<vector<char>>>& blockModel);
    void readFromCSV(const string& filename, BlockInfo& block, map<char, string>& tagTable, vector<vector<vector<char>>>& blockModel);
};

#endif // FILEIO_H
