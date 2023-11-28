#include "Compressor.h"
#include <queue>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <bitset>

using namespace std;

void Compressor::runLengthEncode(OctreeNode* node, string& encodedData) {
    if (node->isLeaf) {
        encodedData += node->tag;
    } else {
        for (int i = 0; i < 8; ++i) {
            runLengthEncode(node->children[i].get(), encodedData);
        }
    }
}

//Helper function for building huffman
struct HuffmanNode {
    char ch;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;
};

struct Compare {
    bool operator()(HuffmanNode* l, HuffmanNode* r) {
        return l->freq > r->freq;
    }
};

void generateCodes(HuffmanNode* root, const string& str, map<char, string>& huffmanCodes) {
    if (!root) return;

    if (root->ch != '\0') {
        huffmanCodes[root->ch] = str;
    }

    generateCodes(root->left, str + "0", huffmanCodes);
    generateCodes(root->right, str + "1", huffmanCodes);
}

void Compressor::buildHuffmanTree(const string& data, map<char, string>& huffmanCodes) {
    unordered_map<char, int> freqMap;
    for (char ch : data) {
        ++freqMap[ch];
    }

    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> pq;
    for (auto& entry : freqMap) {
        HuffmanNode* newNode = new HuffmanNode{entry.first, entry.second, nullptr, nullptr};
        pq.push(newNode);
    }

    while (pq.size() > 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();

        HuffmanNode* merged = new HuffmanNode{'\0', left->freq + right->freq, left, right};
        pq.push(merged);
    }

    HuffmanNode* root = pq.top();
    generateCodes(root, "", huffmanCodes);
}

void Compressor::huffmanEncode(const string& data, const map<char, string>& huffmanCodes, string& encodedData) {
    for (char ch : data) {
        encodedData += huffmanCodes.at(ch);
    }
}

CompressedData Compressor::compressData(const Octree& octree) {
    CompressedData compressedData;
    // Run-length encode the octree
    runLengthEncode(octree.root.get(), compressedData.runLengthEncodedData);
    
    // Build the Huffman Tree and encode the data
    buildHuffmanTree(compressedData.runLengthEncodedData, compressedData.huffmanCodes);
    huffmanEncode(compressedData.runLengthEncodedData, compressedData.huffmanCodes, compressedData.huffmanEncodedData);
    
    return compressedData;
}