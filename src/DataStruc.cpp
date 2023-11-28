#include "DataStruc.h"
#include <iostream>
#include <memory>

using namespace std;

OctreeNode::OctreeNode(bool isLeaf, char tag) : isLeaf(isLeaf), tag(tag) {}

Octree::Octree() {
    root = make_unique<OctreeNode>(false);
}

void Octree::buildOctree(const vector<vector<vector<char>>>& blockModel) {
    size = blockModel.size(); // Assuming cubic blocks
    buildOctreeHelper(root.get(), blockModel, 0, 0, 0, size);
}

void Octree::buildOctreeHelper(OctreeNode* node, const vector<vector<vector<char>>>& blockModel,
                               int startX, int startY, int startZ, int size) {
    //Base case
    if (size == 1) {
        node->isLeaf = true;
        node->tag = blockModel[startZ][startY][startX];

        //Debug output
        cout << "Leaf Node: (" << startX << ", " << startY << ", " << startZ << "), Tag: " << node->tag << '\n';
        return;
    }

    //Check if all cells in this block have the same tag (loop)
    char firstTag = blockModel[startZ][startY][startX];
    bool allSame = true;

    for (int z = startZ; z < startZ + size; ++z) {
        for (int y = startY; y < startY + size; ++y) {
            for (int x = startX; x < startX + size; ++x) {
                if (blockModel[z][y][x] != firstTag) {
                    allSame = false;
                    break;
                }
            }
            if (!allSame) break;
        }
        if (!allSame) break;
    }

    if (allSame) {
        node->isLeaf = true;
        node->tag = firstTag;

        //Debug output
        //cout << "Homogeneous Node: (" << startX << ", " << startY << ", " << startZ << ") to (" << startX + size - 1 << ", " << startY + size - 1 << ", " << startZ + size - 1 << "), Tag: " << firstTag << '\n';
    } else { //Recursive case
        node->isLeaf = false;
        int newSize = size / 2;

        //Debug output
        //cout << "Non-leaf Node: (" << startX << ", " << startY << ", " << startZ << ") to (" << startX + size - 1 << ", " << startY + size - 1 << ", " << startZ + size - 1 << "), Splitting...\n";

        for (int i = 0; i < 8; ++i) {
            int xOffset = (i & 1) * newSize;
            int yOffset = ((i >> 1) & 1) * newSize;
            int zOffset = ((i >> 2) & 1) * newSize;
            node->children[i] = make_unique<OctreeNode>(false);
            
            buildOctreeHelper(node->children[i].get(), blockModel, startX + xOffset, startY + yOffset, startZ + zOffset, newSize);
        }
    }
}