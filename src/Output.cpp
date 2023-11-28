#include "Output.h"
#include <iostream>
#include <algorithm>

void OutputData::generateOutput(const Octree& octree, int parent_x, int parent_y, int parent_z) {
    generateOutputHelper(octree.root.get(), 0, 0, 0, octree.size, parent_x, parent_y, parent_z);
}

void OutputData::generateOutputHelper(OctreeNode* node, int x, int y, int z, int size, int parent_x, int parent_y, int parent_z) {
    if (node->isLeaf) {
        OutputBlock block {x, y, z, size, size, size, std::string(1, node->tag)};
        blocks.push_back(block);
    } else if (size <= std::min({parent_x, parent_y, parent_z})) {
        bool allSame = true;
        char firstTag = node->children[0]->tag;
        
        for (int i = 0; i < 8; ++i) {
            if (node->children[i]->tag != firstTag) {
                allSame = false;
                break;
            }
        }

        if (allSame) {
            OutputBlock block {x, y, z, size, size, size, std::string(1, firstTag)};
            blocks.push_back(block);
            return;
        }
    }
    
    if (!node->isLeaf) {
        int newSize = size / 2;
        for (int i = 0; i < 8; ++i) {
            int xOffset = (i & 1) * newSize;
            int yOffset = ((i >> 1) & 1) * newSize;
            int zOffset = ((i >> 2) & 1) * newSize;
            generateOutputHelper(node->children[i].get(), x + xOffset, y + yOffset, z + zOffset, newSize, parent_x, parent_y, parent_z);
        }
    }
}

void OutputData::printOutput(const std::map<char, std::string>& tagTable) {
    for (const auto& block : blocks) {
        std::cout << block.x << "," << block.y << "," << block.z << ","
                  << block.dx << "," << block.dy << "," << block.dz << ","
                  << tagTable.at(block.tag[0]) << std::endl;
    }
}