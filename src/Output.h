#ifndef OUTPUTDATA_H
#define OUTPUTDATA_H

#include <vector>
#include <string>
#include "DataStruc.h"

using namespace std;

struct OutputBlock {
    int x, y, z;  // Coordinates of the block's bottom-left corner
    int dx, dy, dz;  // Dimensions of the block
    std::string tag;  // Tag representing the type of block
};

class OutputData {
public:
    std::vector<OutputBlock> blocks;
    void generateOutput(const Octree& octree, int parent_x, int parent_y, int parent_z);
    void generateOutputHelper(OctreeNode* node, int x, int y, int z, int size, int parent_x, int parent_y, int parent_z);
    void printOutput(const std::map<char, std::string>& tagTable);
};

#endif // OUTPUTDATA_H
