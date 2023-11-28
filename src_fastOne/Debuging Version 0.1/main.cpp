#include "IOHandler.h"
#include "ModelData.h"
#include <iostream>

int main() {
    // Debug: Check std::cin state before calling read_input
    if (std::cin.fail()) {
        std::cout << "Debug: std::cin is in a fail state before calling read_input()." << std::endl;
    }

    // Read the input
    InputResult inputResult = read_input();

    // Debug: Check std::cin state after calling read_input
    if (std::cin.fail()) {
        std::cout << "Debug: std::cin is in a fail state after calling read_input()." << std::endl;
    }

    if (inputResult.errorCode != ErrorCode::SUCCESS) {
        std::cerr << "Error code: " << static_cast<int>(inputResult.errorCode) << std::endl;  // Display error code

        switch (inputResult.errorCode) {
            case ErrorCode::INVALID_BLOCK_INFO:
                std::cerr << "Error: Invalid block information." << std::endl;
                break;
            case ErrorCode::INVALID_TAG_TABLE:
                std::cerr << "Error: Invalid tag table." << std::endl;
                break;
            case ErrorCode::MISMATCH_BLOCK_CONTENT:
                std::cerr << "Error: Mismatch in block content." << std::endl;
                break;
            case ErrorCode::MISMATCH_TAG_LABEL:
                std::cerr << "Error: Mismatch between tags and labels." << std::endl;
                break;
            default:
                std::cerr << "Error: Unknown error." << std::endl;
        }
        return 1;
    }

    // Compress the model data
    std::vector<OutputBlock> compressedOutput = compress(inputResult.block, inputResult.tagTable, inputResult.blockModel);

    // Write the compressed output
    write_output(compressedOutput);

    return 0;
}
