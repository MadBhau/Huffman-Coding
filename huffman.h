#pragma once

#include <string>
#include <vector>
#include <map>
#include "Node.h"

class Huffman{

private:
    //the root node of huffman tree
    Node* tree;
    
    //map of character with pair of <huffman code, no. of bits of huffman code>
    std::map<char, std::pair<std::string,int>>* encodedMap;

    //map of character and its frequency in the file
    std::map<char,int>* frequencies;

    //creates frequency map for all the characters in the data
    std::map<char,int>* getFrequencies(char *data, int size);


    //Huffman Encode Functions
    void buildTree();
    void sort(std::vector<Node*>& v);

    void findCodes();
    void traverse(Node* node, std::string code,std::map<char, std::pair<std::string,int>>& enc);

    void showTree();

    void writeEncodedFile(std::string infile_name,std::string outfile_name);


    //Huffman Decode Functions
    void reCreateTree(std::string infilename);
    void buildTree(std::string& code, char data);
    void writeDecodedFile(std::string infile_name,std::string outfile_name);
    
    
    //Helper Functions
    std::string decimal_to_binary(int in);
    int binary_to_decimal(std::string const& in);
    bool isFileEmpty(std::ifstream& infile_name);


public:
    Huffman();
    ~Huffman();
    
    void encode(std::string infile_name,std::string outfile_name);
    void decode(std::string infile_name, std::string outfile_name);

};



