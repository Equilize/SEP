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

    # Read block info
    line = input().strip()
    tokens = line.split(',')
    if len(tokens) != 6:
        raise RuntimeError("Invalid format for block info. Expected 6 tokens.")
    block = BlockInfo(int(tokens[0]), int(tokens[1]), int(tokens[2]), int(tokens[3]), int(tokens[4]), int(tokens[5]))

    # Read tag table
    while True:
        line = input().strip()
        if not line:  # Check for an empty line to continue reading
            continue
        tokens = line.split(',')
        if len(tokens) != 2:
            raise RuntimeError("Invalid format for tag table. Expected 2 tokens.")
        tag, label = tokens
        tagTable[tag] = label
        if tag == 'o':  # Assuming 'o' is the last tag in your tag table
            break

    # Initialize block model
    try:
        for z in range(block.z_count):
            slice = []
            for y in range(block.y_count):
                while True:  # Keep reading lines until we get a non-empty line
                    line = input().strip()
                    if line:  # Break the loop once we get a non-empty line
                        break
                if len(line) != block.x_count:
                    raise RuntimeError(f"Block content width ({len(line)}) and x_count ({block.x_count}) does not match. The error occurred at slice (z): {z}, row (y): {y}")
                slice.append(list(line))
            blockModel.append(slice)
            
            # Skip the blank line after reading a z slice, if there is one
            if z < block.z_count - 1:  # Except after the last z slice
                line = input().strip()
    except EOFError:
        pass
    
    if len(blockModel) != block.z_count:
        raise RuntimeError(f"blockModel has {len(blockModel)} slices, expected {block.z_count}.")
    for z_slice in blockModel:
        if len(z_slice) != block.y_count:
            raise RuntimeError(f"A z-slice in blockModel has {len(z_slice)} rows, expected {block.y_count}.")
        for row in z_slice:
            if len(row) != block.x_count:
                raise RuntimeError(f"A row in blockModel has {len(row)} columns, expected {block.x_count}.")

    return block, tagTable, blockModel

def is_homogeneous(blockModel, x, y, z, x_size, y_size, z_size, tag):
    for dz in range(z_size):
        for dy in range(y_size):
            for dx in range(x_size):
                if blockModel[z + dz][y + dy][x + dx] != tag:
                    return False
    return True

def compress_block(x, y, z, x_size, y_size, z_size, block, tagTable, blockModel, visited):
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

def validate_output(block, compressedOutput, blockModel, tagTable):
    # Create an empty 3D list of the same size as blockModel
    reconstructed = [[[None for _ in range(block.x_count)] for _ in range(block.y_count)] for _ in range(block.z_count)]

    # Fill the reconstructed 3D list based on the compressedOutput
    for outputBlock in compressedOutput:
        if (outputBlock.x + outputBlock.x_size > block.x_count) or \
           (outputBlock.y + outputBlock.y_size > block.y_count) or \
           (outputBlock.z + outputBlock.z_size > block.z_count):
            raise ValueError(f"Error at line {compressedOutput.index(outputBlock) + 1}: Block exceeds model boundaries.")

        if outputBlock.x_size > block.parent_x or \
           outputBlock.y_size > block.parent_y or \
           outputBlock.z_size > block.parent_z:
            raise ValueError(f"Error at line {compressedOutput.index(outputBlock) + 1}: Block exceeds parent block size.")

        for dx in range(outputBlock.x_size):
            for dy in range(outputBlock.y_size):
                for dz in range(outputBlock.z_size):
                    if reconstructed[outputBlock.z + dz][outputBlock.y + dy][outputBlock.x + dx] is not None:
                        raise ValueError(f"Error at line {compressedOutput.index(outputBlock) + 1}: Overlapping block detected.")

                    reconstructed[outputBlock.z + dz][outputBlock.y + dy][outputBlock.x + dx] = outputBlock.label

    # Compare the reconstructed 3D list with the original blockModel
    for x in range(block.x_count):
        for y in range(block.y_count):
            for z in range(block.z_count):
                original_tag = blockModel[z][y][x]
                if tagTable[original_tag] != reconstructed[z][y][x]:
                    raise ValueError(f"Mismatch at ({x}, {y}, {z}). Expected {tagTable[original_tag]}, got {reconstructed[z][y][x]}.")

# In your main function, after the write_output call:
# validate_output(block, compressedOutput, blockModel, tagTable)

def main():
    validate = "--validate" in sys.argv
    try:
        block, tagTable, blockModel = read_input()
        compressedOutput = compress(block, tagTable, blockModel)
        write_output(compressedOutput)
        if validate:
            validate_output(block, compressedOutput, blockModel, tagTable)
    except RuntimeError as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)

        # Debug code: python your_script_name.py --validate
        # On windows: Get-Content the_intro_one_32768_4x4x4.csv | python test.py > output.txt --validate

if __name__ == "__main__":
    main()