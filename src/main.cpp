#include "FileIO.h"
#include "Block.h"
#include "DataStruc.h"
#include "Compressor.h"
#include "GPU.h"
#include "Output.h"
#include <iostream>
#include <stdexcept>

using namespace std;

int main() {
    //Initialization
    FileIO fileIO;
    Block blockParser;
    
    Compressor compressor;
    //GPU gpuAccelerator;
    
    OutputData outputData;

    //Define the block info, tag table, and 3D block model
    BlockInfo block;
    map<char, string> tagTable;
    vector<vector<vector<char>>> blockModel;
    vector<OutputBlock> blocks;

    //Specify input mode
    string inputMode = "stream"; //"stream" Or "csv" --> For upload or debug offline

    try {
        //Read input based on the specified mode
        if (inputMode == "stream") {
            fileIO.readFromStream(block, tagTable, blockModel);
        } else if (inputMode == "csv") {
            string filepath = "****file path"; 
            fileIO.readFromCSV(filepath, block, tagTable, blockModel);
        } else {
            throw runtime_error("Invalid input mode specified.");
        }

        //Process the block data into an appropriate data structure
        Octree octree;
        octree.buildOctree(blockModel);

        //Compress the data
        CompressedData compressedData = compressor.compressData(octree);

        //(Optional) Use GPU acceleration for parts of the algorithm
        //gpuAccelerator.optimizeCompression(compressedData);

        // Generate the output blocks with parent block size constraint
        outputData.generateOutput(octree, block.parent_x, block.parent_y, block.parent_z);
        
        // Generate and write the output
        outputData.printOutput(tagTable);  // print to console for now

    } catch (const runtime_error& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
