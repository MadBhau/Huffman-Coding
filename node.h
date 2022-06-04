#pragma once

#include<map>
#include <vector>


enum NODE_TYPE{LEAF, BRANCH};

class Node{

private:
    NODE_TYPE type;
    int freq;
    char data;
    Node* left;
    Node* right;

public:
    //constructors
    //leaf Node
    Node(int f,char d, NODE_TYPE t = LEAF);
    //branch Node
    Node(Node* l, Node* r, NODE_TYPE t = BRANCH);

    //destructor
    ~Node();

    int getFrequency();

    void fill(std::map<char, std::pair<int,int>>& enc, int bits, int nbits);

};