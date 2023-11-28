#ifndef DATASTRUC_H
#define DATASTRUC_H

#include <vector>
#include <memory>
#include <string>
#include <map>

using namespace std;

struct OctreeNode {
    bool isLeaf;
    char tag; //Only valid if isLeaf is true
    unique_ptr<OctreeNode> children[8]; //8 for 3D
    OctreeNode(bool isLeaf, char tag = '\0');
};

class Octree {
public:
    unique_ptr<OctreeNode> root;
    int size;
    
    Octree();
    void buildOctree(const vector<vector<vector<char>>>& blockModel);
    void buildOctreeHelper(OctreeNode* node, const vector<vector<vector<char>>>& blockModel,
                           int startX, int startY, int startZ, int size);
};

#endif // DATASTRUC_H
