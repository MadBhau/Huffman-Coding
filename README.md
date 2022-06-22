
# Huffman Coding

## Overview

Huffman coding is a lossless data compression algorithm. The idea is to assign codes to input characters and the length of the assigned code is based on the frequency of character occurance. The most frequent character gets the smallest code and the least frequent character gets the largest code.

## Implementation

### Compression

1. Open input file, find the frequencies of the characters.
2. Construct Huffman Tree, find and store Huffman Codes.
3. Store Huffman Codes and encoded data into a desired file with `.henc` extension.

### Decompression

1. Open the compressed file (`.henc` extension), recreate Huffman Tree based on Huffman Codes.
2. Decode the file based on the tree and store the results into the desired file.

## Usage
Checkout [encoding.cpp](https://github.com/MadBhau/Huffman-Coding/blob/master/encoding.cpp) and [decoding.cpp](https://github.com/MadBhau/Huffman-Coding/blob/master/decoding.cpp)


1. Create an object of `Huffman` class. \ 
`Huffman h;`

2. To compress, call `encode` function which takes two arguments:\
`h.encode("file to be compressed", "name of compressed file")`

3. To decompress, call `decode` function which takes two arguments:\
`h.decode("file to be decompressed", "name of decompressed file")`

4. Run the code.



## References

 - [ECE264: Huffman Coding](https://engineering.purdue.edu/ece264/17au/hw/HW13?alt=huffman)
 - [Huffman Encoding by maxtors](https://github.com/maxtors/Huffman)
 - [Huffman Coding by cynricfu](https://github.com/cynricfu/huffman-coding)

