

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
    dataFile* data_file;
    tableFile* table_file;
    std::string decodedRes;
    std::vector<std::pair<int,int>> encodedRes;
    std::map<char, std::pair<int,int>>* encodedMap;
    std::map<char,int>* frequencies;

    
    //Functions
    //why function pointer??_______________
    std::map<char,int>* getFrequencies(char *data, int size);

    std::vector<std::pair<int,int>> doEncoding(char* data, int size);

    void createDataFile(std::vector<std::pair<int,int>>& v);
    void createTableFile(std::map<char, std::pair<int,int>>& m);

    bool readDataFile(std::string str);
    bool readTableFile(std::string str);
    bool writeDataFile(std::string str);
    bool writeTableFile(std::string str);


    void sort(std::vector<Node*>& v);
    void buildTree();
    void showTree();
    //void showIntVector(std::vector<int>& v);


public:
    Huffman();
    ~Huffman();
    void readTableFileContent();
    void encode(std::string file_name);
    void decode(std::string data_filename, std::string table_filename);

};



