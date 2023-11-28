import sys

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

def read_input():
    tagTable = {}
    blockModel = []

    #Read block info
    line = input().strip()
    tokens = line.split(',')
    if len(tokens) != 6:
        raise RuntimeError("Invalid format for block info.")
    block = BlockInfo(int(tokens[0]), int(tokens[1]), int(tokens[2]), int(tokens[3]), int(tokens[4]), int(tokens[5]))

    #Read tag table
    while True:
        line = input().strip()
        if not line:
            break
        tokens = line.split(',')
        if len(tokens) != 2:
            raise RuntimeError("Invalid format for tag table.")
        tag, label = tokens
        tagTable[tag] = label

    #Initialize block model
    blockModel = []
    for z in range(block.z_count):
        slice = []
        for y in range(block.y_count):
            line = input().strip()
            if len(line) != block.x_count:
                raise RuntimeError(f"Block content width ({len(line)}) and x_count ({block.x_count}) does not match. The error occurred at slice (z): {z}, row (y): {y}")
            slice.append(list(line))
        blockModel.append(slice)
        input()

    return block, tagTable, blockModel

def compress(block, tagTable, blockModel):
    compressedOutput = []
    visited = [[[False for _ in range(block.x_count)] for _ in range(block.y_count)] for _ in range(block.z_count)]

    for z in range(block.z_count):
        for y in range(block.y_count):
            for x in range(block.x_count):
                if visited[z][y][x]:
                    continue

                tag = blockModel[z][y][x]
                label = tagTable.get(tag, None)
                if label is None:
                    raise RuntimeError("Unknown tag encountered.")

                #Initialize a new output block
                outputBlock = OutputBlock(x, y, z, 1, 1, 1, label)

                #Extend the block along each axis
                while True:
                    extended = False

                    #Extend along x-axis
                    if x + outputBlock.x_size < block.x_count and outputBlock.x_size < block.parent_x:
                        can_extend_x = all(
                            blockModel[z + dz][y + dy][x + outputBlock.x_size] == tag
                            for dz in range(outputBlock.z_size)
                            for dy in range(outputBlock.y_size)
                        )
                        if can_extend_x:
                            outputBlock.x_size += 1
                            extended = True

                    # Extend along y-axis
                    if y + outputBlock.y_size < block.y_count and outputBlock.y_size < block.parent_y:
                        can_extend_y = all(
                            blockModel[z + dz][y + outputBlock.y_size][x + dx] == tag
                            for dz in range(outputBlock.z_size)
                            for dx in range(outputBlock.x_size)
                        )
                        if can_extend_y:
                            outputBlock.y_size += 1
                            extended = True

                    # Extend along z-axis
                    if z + outputBlock.z_size < block.z_count and outputBlock.z_size < block.parent_z:
                        can_extend_z = all(
                            blockModel[z + outputBlock.z_size][y + dy][x + dx] == tag
                            for dy in range(outputBlock.y_size)
                            for dx in range(outputBlock.x_size)
                        )
                        if can_extend_z:
                            outputBlock.z_size += 1
                            extended = True

                    if not extended:
                        break

                #Mark smaller blocks as visited
                for dz in range(outputBlock.z_size):
                    for dy in range(outputBlock.y_size):
                        for dx in range(outputBlock.x_size):
                            visited[z + dz][y + dy][x + dx] = True

                compressedOutput.append(outputBlock)

    return compressedOutput


def write_output(compressedOutput):
    for outputBlock in compressedOutput:
        print(f"{outputBlock.x},{outputBlock.y},{outputBlock.z},{outputBlock.x_size},{outputBlock.y_size},{outputBlock.z_size},{outputBlock.label}")

def main():
    try:
        block, tagTable, blockModel = read_input()
        compressedOutput = compress(block, tagTable, blockModel)
        write_output(compressedOutput)
    except RuntimeError as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    main()
