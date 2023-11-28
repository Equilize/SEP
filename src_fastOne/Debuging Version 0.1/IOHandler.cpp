//这是在windows环境下面向linux环境编写的代码,请尝试在linux环境下运行

//目前输入进程卡在
/* ./myProgram < input.txt                              
Debug: Read first line: '2,2,2,0,0,0'Debug: Block Info - x_count: 2, y_count: 2, z_count: 2, parent_x: 0, parent_y: 0, parent_z: 0
Debug: Read line: '', Length: 0
Debug: Encountered empty line. Ending tag table reading.
Debug: Tag table content: 
Debug: Read block line: 'A,T', Length: 3
Debug: Block line length mismatch: 3 instead of 2
Error code: 3
Error: Mismatch in block content.
*/

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

    auto trim = [](std::string& str) {
        str.erase(0, str.find_first_not_of(" \t\r\n"));
        str.erase(str.find_last_not_of(" \t\r\n") + 1);
    };

    std::string line;
    std::getline(std::cin, line);
    trim(line);

    if (line.empty()) {
        std::cout << "Debug: Encountered an empty line while reading first line. Exiting." << std::endl;
        result.errorCode = ErrorCode::INVALID_BLOCK_INFO;
        return result;
    }

    std::cout << "Debug: Read first line: '" << line << "'";

    auto tokens = splitString(line, ',');
    if (tokens.size() != 6) {
        std::cout << "Debug: First line has invalid token count" << std::endl;
        result.errorCode = ErrorCode::INVALID_BLOCK_INFO;
        return result;
    }

    try {
        result.block.x_count = std::stoi(tokens[0]);
        result.block.y_count = std::stoi(tokens[1]);
        result.block.z_count = std::stoi(tokens[2]);
        result.block.parent_x = std::stoi(tokens[3]);
        result.block.parent_y = std::stoi(tokens[4]);
        result.block.parent_z = std::stoi(tokens[5]);
    } catch (const std::exception& e) {
        std::cerr << "Error: Failed to parse block info. Exception: " << e.what() << std::endl;
        result.errorCode = ErrorCode::INVALID_BLOCK_INFO;
        return result;
    }

    std::cout << "Debug: Block Info - x_count: " << result.block.x_count << ", y_count: " << result.block.y_count << ", z_count: " << result.block.z_count
              << ", parent_x: " << result.block.parent_x << ", parent_y: " << result.block.parent_y << ", parent_z: " << result.block.parent_z << std::endl;

    // Adding new check for std::cin state
    if (std::cin.fail()) {
        std::cout << "Debug: Input stream is in a fail state." << std::endl;
        result.errorCode = ErrorCode::IO_ERROR;  // Replace IO_ERROR with an appropriate error code
        return result;
    }
    // End of new check

    while (true) {
        std::getline(std::cin, line);
        trim(line);

        std::cout << "Debug: Read line: '" << line << "', Length: " << line.length() << std::endl;


        if (line.empty()) {
            std::cout << "Debug: Encountered empty line. Ending tag table reading." << std::endl;
            break;
        }
        if (line.substr(0, 2) == "//") continue;
        tokens = splitString(line, ',');
        if (tokens.size() != 2) {
            std::cout << "Debug: Tag table line has invalid token count";
            result.errorCode = ErrorCode::INVALID_TAG_TABLE;
            return result;
        }
        char tag = tokens[0][0];
        std::string label = tokens[1];
        result.tagTable[tag] = label;
    }

    // Skip extra blank lines, if any
    while (std::cin.peek() == '\n') {
        std::cout << "Debug: Skipping extra blank line.";
        std::cin.get();
    }

    std::cout << "Debug: Tag table content: ";
    for (auto &entry : result.tagTable) {
        std::cout << "(" << entry.first << ", " << entry.second << ") ";
    }
    std::cout << std::endl;

    result.blockModel.resize(result.block.z_count, std::vector<std::vector<char>>(result.block.y_count, std::vector<char>(result.block.x_count)));

    for (int z = 0; z < result.block.z_count; ++z) {
        for (int y = 0; y < result.block.y_count; ++y) {
            if (!std::getline(std::cin, line)) {
                std::cout << "Debug: Premature end of input while reading blocks" << std::endl;
                result.errorCode = ErrorCode::MISMATCH_BLOCK_CONTENT;
                return result;
            }

            trim(line);

            std::cout << "Debug: Read block line: '" << line << "', Length: " << line.length() << std::endl;

            if (line.length() != static_cast<size_t>(result.block.x_count)) {
                std::cout << "Debug: Block line length mismatch: " << line.length() << " instead of " << result.block.x_count << std::endl;
                result.errorCode = ErrorCode::MISMATCH_BLOCK_CONTENT;
                return result;
            }

            result.blockModel[z][y] = std::vector<char>(line.begin(), line.end());
        }
    }

    std::cout << "Debug: Successfully read block model" << std::endl;

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
