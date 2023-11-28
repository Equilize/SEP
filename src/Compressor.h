#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "DataStruc.h"
#include <string>
#include <map>

class CompressedData {
public:
    std::string runLengthEncodedData;  // For run-length encoding
    std::map<char, std::string> huffmanCodes;  // For Huffman encoding
    std::string huffmanEncodedData;  // Encoded data using Huffman codes

    // Additional methods to manipulate or retrieve compressed data can be added here
};

class Compressor {
public:
    CompressedData compressData(const Octree& octree);
    void runLengthEncode(OctreeNode* node, std::string& encodedData);
    void buildHuffmanTree(const std::string& data, std::map<char, std::string>& huffmanCodes);
    void huffmanEncode(const std::string& data, const std::map<char, std::string>& huffmanCodes, std::string& encodedData);
};

#endif // COMPRESSOR_H