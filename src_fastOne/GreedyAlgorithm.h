#ifndef GREEDY_ALGORITHM_H
#define GREEDY_ALGORITHM_H

#include <vector>
#include <map>
#include <string>

struct BlockInfo {
    int x_count, y_count, z_count;
    int parent_x, parent_y, parent_z;
};

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

#endif // GREEDY_ALGORITHM_H
