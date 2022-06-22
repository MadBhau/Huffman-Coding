#include <iostream>
#include "node.h"


//Constructors

Node::Node(){
    type = BRANCH;
    left = NULL;
    right = NULL;
}


Node::Node(int f,char d, NODE_TYPE t){
    freq = f;
    data = d;
    type = t;
    left = NULL;
    right = NULL;

};
   
Node::Node(Node* l, Node* r, NODE_TYPE t){
    type = t;
    data = 0;
    freq = l->freq + r->freq;
    left = l;
    right = r;
    
};

//destructor
Node::~Node(){
    delete left;
    delete right;
};




