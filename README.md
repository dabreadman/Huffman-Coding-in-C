# Huffman-Coding-in-C
Implementing Huffman Coding in C 
Huffman Coding is a lossless compression, meaning no data is loss during the compression.
It achieved it by changing the encoding of each characters after analysing the frequecies of each characters.
This allow more frequent characters to be encoded using shorter encoding, which reduces the file size.


----------Training----------
1. Reads the file.
2. Update character frequencies for each character.
3. Create a binary tree with least frequencies characters at the deepest.
4. Create a table containing encoding for each characters after  tree traversal.

----------Encoding----------
1. Reads the input file.
2. Check the table for the encoding and write it into the output file.
   This is due to the encoding not having the same sizes.
   We are buffering the code, and writing it down byte by byte using bitset.
   That would allow us to write down codes that has length not multiple of byte(s) without wasted space.

---------Decoding-----------
1. Reads the input file.
2. Reads bit by bit.

3.0 If there is a match after comparing the the table, write the corresponding character into the output file.
3.1 If there is no match, buffer the bit and read another bit and add it after the buffered bit(s). 
    Repeat 3.0

Thanks for Lexes for helping me.  
Lexes: https://github.com/lexesjan 
Credit to David for providing make/test files.
David: https://www.scss.tcd.ie/David.Gregg/ 
