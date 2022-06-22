

#include <iostream>
#include "node.h"

Node::Node(){
    freq =0;
    data =' ';
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


Node::~Node(){
    delete left;
    delete right;
};

int Node::getFrequency(){
    return freq;
};

void Node::fill(std::map<char, std::pair<int,int>>& enc, int bits, int nbits){

    if(type == LEAF){
        
        enc[data] = std::pair<int,int>(bits,nbits);
    }

    else if(type== BRANCH){
        nbits += 1;
        bits <<= 1;
        left->fill(enc, bits, nbits);
        bits += 1;
        right->fill(enc, bits, nbits);
        bits >>=1;
        nbits--;
    }

};





bool Node::operator < (Node& a) {
    return getFrequency()> a.getFrequency();
}


