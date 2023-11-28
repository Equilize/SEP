#ifndef BLOCK_H
#define BLOCK_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <istream>
#include <vector>
#include <map>


struct BlockInfo {
    int x_count, y_count, z_count;  // Block Size
    int parent_x, parent_y, parent_z;  // Parent size
};

class Block {
public:
    void parseBlockInfo(std::istream& stream, BlockInfo& block);
    void parseTagTable(std::istream& stream, std::map<char, std::string>& tagTable);
    void parseBlockContent(std::istream& stream, const BlockInfo& block, std::vector<std::vector<std::vector<char>>>& blockModel);
};


#endif // BLOCK_H
