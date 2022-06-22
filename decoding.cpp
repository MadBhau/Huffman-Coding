#include <iostream>
#include <fstream>
#include "huffman.h"


int main() {

    Huffman h;

    h.decode("note-compressed.henc","note-decompressed.txt");
    
    std::cout << "\n"; system("pause");
    return 0;

}