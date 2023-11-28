// ModelData.h

#ifndef MODEL_DATA_H
#define MODEL_DATA_H

#include <vector>
#include <map>
#include <string>

// Define the structure for a block of 3D data
struct BlockInfo {
    int x_count, y_count, z_count;
    int parent_x, parent_y, parent_z;
};

// Define the structure for compressed output
struct OutputBlock {
    int x, y, z;
    int x_size, y_size, z_size;
    std::string label;
};

std::vector<OutputBlock> compress(
    const BlockInfo &block, 
    const std::map<char, std::string> &tagTable, 
    const std::vector<std::vector<std::vector<char>>> &blockContent
);

#endif // MODEL_DATA_H
