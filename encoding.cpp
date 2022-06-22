#include <iostream>
#include <fstream>
#include "huffman.h"


int main() {

    Huffman h;

    h.encode("note.txt","note-compressed");
    
    std::cout << "\n"; system("pause");
    return 0;

}