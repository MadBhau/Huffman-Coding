#include <iostream>
#include <fstream>
#include "huffman.h"


int main() {

    std::ifstream file;
    Huffman h;

    //h.encode("note");
    h.decode("note.hdta","note.htbl");
    h.readTableFileContent();
    // std::cout<<"Here"<<'\n';
    //
    std::cout << "\n"; system("pause");
    return 0;

}