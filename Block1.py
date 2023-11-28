#Design Archetecture
#keep modularity for extension purposes

#*************************************************
#input function
#*************************************************
#read input stream (Line by line)


#1. First line --> Size & Parent size



#2. Next n lines --> Read to tag table (Tag, label) until <blank line>



#3. Read individual blocks
# 1 slice --> Reach <blank line> --> Check next slice

#*************************************************
#Compression function
#*************************************************

#get x & y of bottom left block
#Reach max_x of same color
#reach max_y of same color
#encode_rectangle for largest rectangle
#record to ?
#x= max_x & start recursive


#*************************************************
#Output function
#*************************************************