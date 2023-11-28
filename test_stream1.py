import sys
from collections import deque


class BlockInfo:
    def __init__(self, x_count, y_count, z_count, parent_x, parent_y, parent_z):
        self.x_count = x_count
        self.y_count = y_count
        self.z_count = z_count
        self.parent_x = parent_x
        self.parent_y = parent_y
        self.parent_z = parent_z

class OutputBlock:
    def __init__(self, x, y, z, x_size, y_size, z_size, label):
        self.x = x
        self.y = y
        self.z = z
        self.x_size = x_size
        self.y_size = y_size
        self.z_size = z_size
        self.label = label

def read_initial():
    tagTable = {}
    blockModel = []

    # Read block info
    line = input().strip()
    tokens = line.split(',')
    if len(tokens) != 6:
        raise RuntimeError("Invalid format for block info. Expected 6 tokens.")
    block = BlockInfo(int(tokens[0]), int(tokens[1]), int(tokens[2]), int(tokens[3]), int(tokens[4]), int(tokens[5]))

    # Read tag table
    while True:
        line = input().strip()
        if not line:  # Check for an empty line to break out of the loop
            break
        tokens = line.split(',')
        if len(tokens) != 2:
            raise RuntimeError("Invalid format for tag table. Expected 2 tokens.")
        tag, label = tokens
        tagTable[tag] = label
    return block, tagTable
   

def read_blockModel(block):
    blockModel = []
    for z in range(block.z_count):
        z_slice = []
        for y in range(block.y_count):
            line = input().strip()
            z_slice.append(list(line))
        blockModel.append(z_slice)
        if z < block.z_count - 1:  # Skip the empty line
            input()

    return blockModel

def is_last_chunk():
    # Logic to check if it's the last chunk (two blank lines)
    line = input().strip()
    return not line

def is_homogeneous(blockModel, x, y, z, x_size, y_size, z_size, tag):
    for dz in range(z_size):
        for dy in range(y_size):
            for dx in range(x_size):
                if blockModel[z + dz][y + dy][x + dx] != tag:
                    return False
    return True

def compress_block(x, y, z, x_size, y_size, z_size, block, tagTable, blockModel, visited):
    if z >= len(blockModel) or y >= len(blockModel[z]) or x >= len(blockModel[z][y]):
        raise IndexError(f"Trying to access out-of-bounds index: z={z}, y={y}, x={x}")
    tag = blockModel[z][y][x]

    if is_homogeneous(blockModel, x, y, z, x_size, y_size, z_size, tag):
        for dx in range(x_size):
            for dy in range(y_size):
                for dz in range(z_size):
                    visited[z + dz][y + dy][x + dx] = True
        return [OutputBlock(x, y, z, x_size, y_size, z_size, tagTable[tag])]
    else:
        if x_size > 1 or y_size > 1 or z_size > 1:
            half_x = max(x_size // 2, 1)
            half_y = max(y_size // 2, 1)
            half_z = max(z_size // 2, 1)
            blocks = []
            for dx in [0, half_x]:
                for dy in [0, half_y]:
                    for dz in [0, half_z]:
                        blocks += compress_block(x+dx, y+dy, z+dz, min(half_x, x_size-dx), min(half_y, y_size-dy), min(half_z, z_size-dz), block, tagTable, blockModel, visited)
            return blocks
        else:
            visited[z][y][x] = True
            return [OutputBlock(x, y, z, 1, 1, 1, tagTable[tag])]

def compress(block, tagTable, blockModel):
    visited = [[[False for _ in range(block.x_count)] for _ in range(block.y_count)] for _ in range(block.z_count)]
    compressedOutput = []

    for z in range(0, block.z_count, block.parent_z):
        for y in range(0, block.y_count, block.parent_y):
            for x in range(0, block.x_count, block.parent_x):
                if not visited[z][y][x]:
                    compressedOutput += compress_block(x, y, z, block.parent_x, block.parent_y, block.parent_z, block, tagTable, blockModel, visited)
    return compressedOutput

def write_output(compressedOutput):
    for outputBlock in compressedOutput:
        print(f"{outputBlock.x},{outputBlock.y},{outputBlock.z},{outputBlock.x_size},{outputBlock.y_size},{outputBlock.z_size},{outputBlock.label}")



def main():
    try:
        # Read the initial block information and tag table
        block, tagTable = read_initial()

        # Initialize a buffer for block models
        buffer = deque()

        BUFFER_SIZE = 2  # Example buffer size, set as appropriate

        while True:
    # Fill up the buffer up to BUFFER_SIZE or until last chunk
            while len(buffer) < BUFFER_SIZE:
                print(len(buffer))
                blockModel = read_blockModel(block)
                buffer.append(blockModel)
                if is_last_chunk():
                    break

            # Process buffered block models
            while buffer:
                blockModel = buffer.popleft()
                compressedOutput = compress(block, tagTable, blockModel)
                write_output(compressedOutput)

            if is_last_chunk():
                break



        

    except RuntimeError as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    main()