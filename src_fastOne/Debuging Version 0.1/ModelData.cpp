#include "ModelData.h"
#include <stdexcept>

struct OctreeNode {
    char tag; // tag of the block if this node represents a block
    bool isLeaf;
    OctreeNode* children[8];

    OctreeNode(char t) : tag(t), isLeaf(true) {
        for(int i = 0; i < 8; ++i) children[i] = nullptr;
    }

    ~OctreeNode() {
        for(int i = 0; i < 8; ++i) {
            delete children[i];
        }
    }
};

OctreeNode* buildOctree(const std::vector<std::vector<std::vector<char>>> &blockContent, int xStart, int xEnd, int yStart, int yEnd, int zStart, int zEnd) {
    if(xStart > xEnd || yStart > yEnd || zStart > zEnd) return nullptr;

    char firstTag = blockContent[zStart][yStart][xStart];
    bool isUniform = true;

    for(int z = zStart; z <= zEnd && isUniform; ++z) {
        for(int y = yStart; y <= yEnd && isUniform; ++y) {
            for(int x = xStart; x <= xEnd && isUniform; ++x) {
                if(blockContent[z][y][x] != firstTag) {
                    isUniform = false;
                }
            }
        }
    }

    OctreeNode* node = new OctreeNode(firstTag);
    if(!isUniform) {
        node->isLeaf = false;
        int xMid = (xStart + xEnd) / 2;
        int yMid = (yStart + yEnd) / 2;
        int zMid = (zStart + zEnd) / 2;

        for (int i = 0; i < 8; ++i) {
            int xOffset = (i & 1) ? (xMid + 1) : xStart;
            int yOffset = (i & 2) ? (yMid + 1) : yStart;
            int zOffset = (i & 4) ? (zMid + 1) : zStart;

            node->children[i] = buildOctree(blockContent, xOffset, xOffset + xMid - xStart, yOffset, yOffset + yMid - yStart, zOffset, zOffset + zMid - zStart);
        }
    }

    return node;
}

void compressOctree(OctreeNode* node, const BlockInfo &block, const std::map<char, std::string> &tagTable, int xStart, int xEnd, int yStart, int yEnd, int zStart, int zEnd, std::vector<OutputBlock> &output) {
    if(node->isLeaf) {
        OutputBlock blockOutput = {xStart, yStart, zStart, xEnd - xStart + 1, yEnd - yStart + 1, zEnd - zStart + 1, tagTable.at(node->tag)};
        output.push_back(blockOutput);
    } else {
        int xMid = (xStart + xEnd) / 2;
        int yMid = (yStart + yEnd) / 2;
        int zMid = (zStart + zEnd) / 2;

        for (int i = 0; i < 8; ++i) {
            int xOffset = (i & 1) ? (xMid + 1) : xStart;
            int yOffset = (i & 2) ? (yMid + 1) : yStart;
            int zOffset = (i & 4) ? (zMid + 1) : zStart;

            compressOctree(node->children[i], block, tagTable, xOffset, xOffset + xMid - xStart, yOffset, yOffset + yMid - yStart, zOffset, zOffset + zMid - zStart, output);
        }
    }
}

std::vector<OutputBlock> compress(const BlockInfo &block, const std::map<char, std::string> &tagTable, const std::vector<std::vector<std::vector<char>>> &blockContent) {
    OctreeNode* root = buildOctree(blockContent, 0, block.x_count-1, 0, block.y_count-1, 0, block.z_count-1);

    std::vector<OutputBlock> compressedOutput;
    compressOctree(root, block, tagTable, 0, block.x_count-1, 0, block.y_count-1, 0, block.z_count-1, compressedOutput);

    delete root;
    return compressedOutput;
}
