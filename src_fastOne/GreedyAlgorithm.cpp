#include "GreedyAlgorithm.h"
#include <stdexcept>

std::vector<OutputBlock> compress(const BlockInfo &block, const std::map<char, std::string> &tagTable, const std::vector<std::vector<std::vector<char>>> &blockContent) {
    std::vector<OutputBlock> compressedOutput;
    for (int z = 0; z < block.z_count; z += 2) {
        for (int y = 0; y < block.y_count; y += 2) {
            for (int x = 0; x < block.x_count; x += 2) {
                char firstTag = blockContent[z][y][x];
                bool isUniform = true;
                for (int dz = 0; dz < 2 && isUniform; dz++) {
                    for (int dy = 0; dy < 2 && isUniform; dy++) {
                        for (int dx = 0; dx < 2 && isUniform; dx++) {
                            if (blockContent[z + dz][y + dy][x + dx] != firstTag) {
                                isUniform = false;
                            }
                        }
                    }
                }

                OutputBlock blockOutput = {x, y, z, 2, 2, 2, tagTable.at(firstTag)};
                if (!isUniform) {
                    blockOutput.label = "NON_UNIFORM";
                }
                compressedOutput.push_back(blockOutput);
            }
        }
    }
    return compressedOutput;
}


