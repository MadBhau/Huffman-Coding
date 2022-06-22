#pragma once

#include<string>
#include<map>
#include<vector>


enum NODE_TYPE{LEAF, BRANCH};

class Node{

public:
    
    int freq;
    char data;
    NODE_TYPE type;
    Node* left;
    Node* right;

  
    //constructors

    Node();
    //leaf Node
    Node(int f,char d, NODE_TYPE t = LEAF);
    //branch Node
    Node(Node* l, Node* r, NODE_TYPE t = BRANCH);

    //destructor
    ~Node();

    int getFrequency();

    void fill(std::map<char, std::pair<int,int>>& enc, int bits, int nbits);   

    bool operator < (Node& a) ;

    
    
    
    

};