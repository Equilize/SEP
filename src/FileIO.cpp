#include "FileIO.h"
#include "Block.h"
using namespace std;

void FileIO::readFromStream(BlockInfo& block, map<char, string>& tagTable, vector<vector<vector<char>>>& blockModel) {
    Block blockParser;
    blockParser.parseBlockInfo(cin, block);
    blockParser.parseTagTable(cin, tagTable);
    blockParser.parseBlockContent(cin, block, blockModel);
}

void FileIO::readFromCSV(const string& filename, BlockInfo& block, map<char, string>& tagTable, vector<vector<vector<char>>>& blockModel) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Failed to open CSV file.");
    }

    Block blockParser;
    blockParser.parseBlockInfo(file, block);
    blockParser.parseTagTable(file, tagTable);
    blockParser.parseBlockContent(file, block, blockModel);
    file.close();
}
