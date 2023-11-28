#include "IOHandler.h"
#include <sstream>
#include <iostream>
#include <stdexcept>


InputResult read_input() {
    InputResult result;
    auto splitString = [](const std::string &s, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    };

    // First line --> Size & parent size
    std::string line;
    std::getline(std::cin, line);
    auto tokens = splitString(line, ',');
    if (tokens.size() != 6) {
        result.errorCode = ErrorCode::INVALID_BLOCK_INFO;
        return result;
    }

    result.block.x_count = std::stoi(tokens[0]);
    result.block.y_count = std::stoi(tokens[1]);
    result.block.z_count = std::stoi(tokens[2]);
    result.block.parent_x = std::stoi(tokens[3]);
    result.block.parent_y = std::stoi(tokens[4]);
    result.block.parent_z = std::stoi(tokens[5]);

    // Next n lines --> read to tag table (tag, label) until <blank line>
    while(std::getline(std::cin, line) && !line.empty()) {
        tokens = splitString(line, ',');
        if(tokens.size() != 2) {
            result.errorCode = ErrorCode::INVALID_TAG_TABLE;
            return result;
        }
        char tag = tokens[0][0];
        std::string label = tokens[1];
        result.tagTable[tag] = label;
    }

    // Validate if tags from tagTable are unique
    // if (result.tagTable.size() != tokens.size() / 2) {
    //     result.errorCode = ErrorCode::MISMATCH_TAG_LABEL;
    //     return result;
    // }

    // Initialize the block model
    result.blockModel.resize(result.block.z_count, std::vector<std::vector<char>>(result.block.y_count, std::vector<char>(result.block.x_count)));

    // Reading block model content
    for(int z = 0; z < result.block.z_count; ++z) {
        for(int y = 0; y < result.block.y_count; ++y) {
            std::getline(std::cin, line);
            //Debug
            // std::cout << "\nLine.length()= " << line.length() << "\nblock.x_count= " << result.block.x_count << "\n"; 
            if (line.length() != static_cast<size_t>(result.block.x_count)) {
                result.errorCode = ErrorCode::MISMATCH_BLOCK_CONTENT;
                return result;
            }
            result.blockModel[z][y] = std::vector<char>(line.begin(), line.end());
        }
    }

    result.errorCode = ErrorCode::SUCCESS;
    return result;
}

void write_output(const std::vector<OutputBlock>& compressedOutput) {
    std::cout << "Compressed Output:" << std::endl;
    for (const auto& outputBlock : compressedOutput) {
        std::cout << outputBlock.x << "," << outputBlock.y << "," << outputBlock.z << ","
                  << outputBlock.x_size << "," << outputBlock.y_size << "," << outputBlock.z_size << ","
                  << outputBlock.label << std::endl;
    }
}
