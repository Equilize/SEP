//Version 1.0.2
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <stdexcept>
#include <fstream>

using namespace std;

struct BlockInfo {
    int x_count, y_count, z_count; //Block Size
    int parent_x, parent_y, parent_z; //Parent size
};

struct OutputBlock {
    int x, y, z;
    int x_size, y_size, z_size;
    string label;
};

//Reading input as stdin
void read_input(BlockInfo &block, map<char, string> &tagTable, vector<vector<vector<char>>> &blockModel) {
    auto splitString = [](const string &s, char delimiter) {
        vector<string> tokens;
        string token;
        istringstream tokenStream(s);
        while (getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    };
    
    //First line --> Size & parent size
    string line;
    getline(cin, line);
    auto tokens = splitString(line, ',');
    if (tokens.size() != 6) {
        throw runtime_error("Invalid format for block info.");
    }
    block.x_count = stoi(tokens[0]);
    block.y_count = stoi(tokens[1]);
    block.z_count = stoi(tokens[2]);
    block.parent_x = stoi(tokens[3]);
    block.parent_y = stoi(tokens[4]);
    block.parent_z = stoi(tokens[5]);

    //Debug output for block info
    // cout << "Block Info: " << block.x_count << ", " << block.y_count << ", " << block.z_count << ", " 
    //           << block.parent_x << ", " << block.parent_y << ", " << block.parent_z << std::endl;

    
    //Next n lines --> read to tag table (tag, label) until <blank line>
    while(getline(cin, line) && !line.empty()) {
        tokens = splitString(line, ',');
        if(tokens.size() != 2) {
            throw runtime_error("Invalid format for tag table.");
        }
        char tag = tokens[0][0];
        string label = tokens[1];
        tagTable[tag] = label; //Storing tag and corresponding label
    }

    //Debug output for tag table
    // cout << "Tag table:" << endl;
    // for(const auto &entry : tagTable) {
    //     cout << "Tag: " << entry.first << ", Label: " << entry.second << endl;
    // }

    //3D Model --> Read individual blocks
    //1 slice --> Reach <blank line> --> Check next slice

    //Initialize the block model
    blockModel.resize(block.z_count, vector<vector<char>>(block.y_count, vector<char>(block.x_count)));

    //Reading block model content
    for(int z = 0; z < block.z_count; ++z) {
        //vector<string> sliceRows;
        for(int y = 0; y < block.y_count; ++y) {
            getline(cin, line);
            // cout << "Debug: line.length() = " << line.length() << ", block.x_count = " << block.x_count << endl;
            // cout << "Debug: line = '" << line << "'" << endl;

            // Trim the line (remove whitespace and special characters at both ends)
            line.erase(0, line.find_first_not_of(" \t\r\n"));
            line.erase(line.find_last_not_of(" \t\r\n") + 1);
            
            //Length debug
            // cout << line << endl;
            // cout << "Length of line is: " << line.length() << endl;
            // cout << "x_count of current line of block is: " << static_cast<size_t>(block.x_count) << endl;

            if (line.length() != static_cast<size_t>(block.x_count)) {
                stringstream ss;
                ss << "Block content width and x_count does not match. ";
                ss << "The error occurred at slice (z): " << z << ", row (y): " << y;
                throw runtime_error(ss.str());
            }
            //sliceRows.push_back(line);
            blockModel[z][y] = vector<char>(line.begin(), line.end());
        }
        if (z != block.z_count - 1) {  // No need to read blank line after the last slice
        getline(cin, line);
        }

        //Store rows in reverse order
        // for(int y = 0; y < block.y_count; ++y) {
        //     blockModel[z][y] = vector<char>(sliceRows[block.y_count - y - 1].begin(), sliceRows[block.y_count - y -1].end());
        // }
    }
    
    //Debug output for block model content
    // for (int z = 0; z < block.z_count; ++z) {
    //     cout << "Slice " << z << ": " << endl;
    //     for (int y = 0; y < block.y_count; ++y) {
    //         for(int x = 0; x < block.x_count; ++x) {
    //             cout << blockModel[z][y][x];
    //         }
    //         cout << endl;
    //     }
    // }

}
// Check if a given block is homogeneous
bool is_homogeneous(const vector<vector<vector<char>>> &blockModel, int x, int y, int z, int x_size, int y_size, int z_size, char tag) {
    for(int dz = 0; dz < z_size; dz++) {
        for(int dy = 0; dy < y_size; dy++) {
            for(int dx = 0; dx < x_size; dx++) {
                if (blockModel[z + dz][y + dy][x + dx] != tag) {
                    return false;
                }
            }
        }
    }
    return true;
}

// Recursive function to compress blocks
vector<OutputBlock> compress_block(int x, int y, int z, int x_size, int y_size, int z_size, const BlockInfo &block, const map<char, string> &tagTable, const vector<vector<vector<char>>> &blockModel) {
    vector<OutputBlock> blocks;
    char tag = blockModel[z][y][x];
    auto it = tagTable.find(tag);
    if (it == tagTable.end()) {
        throw runtime_error("Unknown tag encountered.");
    }

    if (is_homogeneous(blockModel, x, y, z, x_size, y_size, z_size, tag)) {
        blocks.push_back({x, y, z, x_size, y_size, z_size, it->second});
    } else {
        int half_x = max(x_size / 2, 1);
        int half_y = max(y_size / 2, 1);
        int half_z = max(z_size / 2, 1);

        for (int dx = 0; dx < x_size; dx += half_x) {
            for (int dy = 0; dy < y_size; dy += half_y) {
                for (int dz = 0; dz < z_size; dz += half_z) {
                    auto innerBlocks = compress_block(x+dx, y+dy, z+dz, min(half_x, x_size-dx), min(half_y, y_size-dy), min(half_z, z_size-dz), block, tagTable, blockModel);
                    blocks.insert(blocks.end(), innerBlocks.begin(), innerBlocks.end());
                }
            }
        }
    }

    return blocks;
}

vector<OutputBlock> compress(const BlockInfo &block, const map<char, string> &tagTable, const vector<vector<vector<char>>> &blockContent) {
    vector<OutputBlock> compressedOutput;
    for (int z = 0; z < block.z_count; z += block.parent_z) {
        for (int y = 0; y < block.y_count; y += block.parent_y) {
            for (int x = 0; x < block.x_count; x += block.parent_x) {
                auto blocks = compress_block(x, y, z, block.parent_x, block.parent_y, block.parent_z, block, tagTable, blockContent);
                compressedOutput.insert(compressedOutput.end(), blocks.begin(), blocks.end());
            }
        }
    }
    return compressedOutput;
}

void write_output(const vector<OutputBlock>& compressedOutput) {
    //cout << "Compressed Output:" << endl;
    for (const auto& outputBlock : compressedOutput) {
        cout << outputBlock.x << "," << outputBlock.y << "," << outputBlock.z << ","
             << outputBlock.x_size << "," << outputBlock.y_size << "," << outputBlock.z_size << ","
             << outputBlock.label << endl;
             if (outputBlock.y > 20) {
                break;
             }
    }
}


int main()
{
    BlockInfo block;
    map<char, string> tagTable;
    vector<vector<vector<char>>> blockModel;
    
    try {
        read_input(block, tagTable, blockModel);

        vector<OutputBlock> compressedOutput = compress(block, tagTable, blockModel);

        write_output(compressedOutput);
    }
    catch(const runtime_error &e) {
        cerr << "Error: " <<e.what() << endl;
        return 1;
    }
    return 0;
}




