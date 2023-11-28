#ifndef GPU_H
#define GPU_H

class GPU {
public:
    //Constructor
    GPU();

    //Apply GPU acceleration on the compression
    void accelerateCompression();

    //Apply GPU acceleration on the decompression
    void accelerateDecompression();

private:
    //Additional helper methods and attributes for GPU handling maybe?
};

#endif // GPU_H
