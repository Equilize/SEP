#ifndef IOHANDLER_H
#define IOHANDLER_H

#include "ModelData.h"
#include <map>
#include <vector>
#include <string>

enum class ErrorCode {
    SUCCESS,
    INVALID_BLOCK_INFO,
    INVALID_TAG_TABLE,
    MISMATCH_BLOCK_CONTENT,
    MISMATCH_TAG_LABEL,
    IO_ERROR
};

struct InputResult {
    ErrorCode errorCode;
    BlockInfo block;
    std::map<char, std::string> tagTable;
    std::vector<std::vector<std::vector<char>>> blockModel;
};

InputResult read_input();
void write_output(const std::vector<OutputBlock>& compressedOutput);

#endif
