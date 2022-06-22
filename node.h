#pragma once


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

};