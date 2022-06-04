

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

struct tableEntry{
    char data;
    char size;
    int bits;
};

struct tableFile{
    char key;
    std::vector<tableEntry> data;
};



class Huffman{

private:
    Node* tree;
    dataFile* datafile;
    tableFile tableFile;
    std::string decoded;
    std::vector<std::pair<int,int>> encoded;
    std::map<char, std::pair<int,int>>* endcodedMap;
    std::map<char,int>* frequencies;

    
    //Functions
    std::map<char,int> getFrequencies(char *data, int size);

    std::vector<std::pair<int,int>> doEncoding(char* data, int size);

    void createDataFile(std::vector<std::pair<int,int>>& v);
    void createTableFile(std::map<char, std::pair<int,int>>& m);

    void readDataFile(std::string str);
    void readTableFile(std::string str);

    void writeDataFile(std::string str);
    void writeDataFile(std::string str);


    void sort(std::vector<Node*>& v);
    void buildTree();
    void showTree();
    void showIntVector(std::vector<int>& v);


public:
    Huffman();
    ~Huffman();
    void encode(std::string fileName);
    void decode(std::string dataFile, std::string tableFile);

};



