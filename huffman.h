#pragma once

#include <string>
#include <vector>
#include <map>
#include "Node.h"

struct dataFile{
    char key;
    int size;
    char offset;
    char* data;
};




class Huffman{

private:
    Node* tree;
    dataFile* data_file;
    
    std::string decodedRes;
    std::vector<std::pair<std::string,int>> encodedRes;
    std::map<char, std::pair<std::string,int>>* encodedMap;
    std::map<char,int>* frequencies;

    
    //Functions
    //why function pointer??_______________
    std::map<char,int>* getFrequencies(char *data, int size);

    std::vector<std::pair<std::string,int>> doEncoding(char* data, int size);

    void createDataFile(std::vector<std::pair<int,int>>& v);

    void writeDecodedFile(std::string infilename,std::string outfilename);
    
    void writeEncodedFile(std::string infilename,std::string outfilename);
    
    void reCreateTree(std::string infilename);

    void sort(std::vector<Node*>& v);
    void buildTree();
    void buildTree(std::string& code, char data);
    void showTree();
    std::string decimal_to_binary(int in);
    int binary_to_decimal(std::string const& in);

    void traverse(Node* node, std::string code,std::map<char, std::pair<std::string,int>>& enc);

    void findCodes();

    void inorder(Node* root);

    //void showIntVector(std::vector<int>& v);


public:
    Huffman();
    ~Huffman();
    

    void encode(std::string infile_name,std::string outfile_name);
    void decode(std::string infile_name, std::string outfile_name);

};



