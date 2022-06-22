#include <iostream>
#include <fstream>
#include "huffman.h"


int main() {

    std::ifstream file;
    Huffman h;

    
    
    
    //h.encode("note","note-compressed");
    
    h.decode("note-compressed.henc","note-decompressed.txt");
    
    // std::cout<<"Here"<<'\n';
    //
    std::cout << "\n"; system("pause");
    return 0;

}