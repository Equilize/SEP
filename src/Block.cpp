#include "Block.h"
#include <sstream>
#include <istream>
#include <vector>
#include <map>
#include <limits>
#include <ios>
#include <stdexcept>
using namespace std;

void Block::parseBlockInfo(istream& stream, BlockInfo& block) {
    // Parse block size and parent size from the stream
    string line;
    getline(stream, line);
    istringstream iss(line);

    char delim; //Store commas
    if (!(iss >> block.x_count >> delim >> block.y_count >> delim >> block.z_count 
              >> delim >> block.parent_x >> delim >> block.parent_y >> delim >> block.parent_z)) {
        throw runtime_error("Failed to parse block information.");
    }
    //Debug output
    cout << "Parsed Block Info: (" << block.x_count << ", " << block.y_count << ", " << block.z_count << "), ("
              << block.parent_x << ", " << block.parent_y << ", " << block.parent_z << ")\n";
}

void Block::parseTagTable(istream& stream, map<char, string>& tagTable) {
    // Parse the tag table from the stream until an empty line is encountered
    string line;
    

    while (getline(stream, line) && !line.empty()) {
        istringstream iss(line);
        char tag;
        string label;

        if(!(iss >> tag)) {
            throw runtime_error("Failed to parse tag.");
        }

        getline(iss, label);

        if(label.empty()) {
            throw runtime_error("No label entered.");
        }

        tagTable[tag] = label;

        //Debug output
        cout << "Parsed Tag: " << tag << ", Label: " << label << '\n';
    }
}

void Block::parseBlockContent(istream& stream, const BlockInfo& block, vector<vector<vector<char>>>& blockModel) {
    // Parse the 3D block content from the stream
    string line;

    blockModel.resize(block.z_count, vector<vector<char>>(block.y_count, vector<char>(block.x_count)));
    for (int z = 0; z < block.z_count; ++z) {

        // Debug output for the slice
        cout << "Parsing slice at z=" << z << '\n'; 

        for (int y = block.y_count - 1; y >= 0; --y) {  // Reverse loop to start at what will be the "top"
            getline(stream, line);

            if (line.size() != static_cast<size_t>(block.x_count)) {
                throw runtime_error("The number of elements in one row does not match x_count.");
            }
            for (int x = block.x_count - 1; x >= 0; --x) {  // Reverse loop to start at what will be the "left"
                blockModel[z][y][x] = line[block.x_count - 1 - x];  // Adjust index for reverse loop
                
                // Debug output for each cell
                // cout << "Parsed Cell at (" << x << ", " << y << ", " << z << "): " << line[x] << '\n';
            }
        }
        if (z != block.z_count - 1) {  // No need to read blank line after the last slice
        getline(stream, line);
        }

        // Debug output for the entire slice
        cout << "Finished parsing slice at z=" << z << '\n';
        for (int y = 0; y < block.y_count; ++y) {
            for (int x = 0; x < block.x_count; ++x) {
                cout << blockModel[z][y][x];
            }
            cout << '\n';
        }
    }
}
