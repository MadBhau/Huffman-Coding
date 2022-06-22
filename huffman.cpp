#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<map>
#include<algorithm>

#include"huffman.h"
#include"Node.h"


Huffman::Huffman(){
    encodedMap = new std::map<char, std::pair<std::string,int>>;
    frequencies = NULL;
    data_file = NULL;
    
}

//Deconstructor
Huffman::~Huffman(){
    delete encodedMap;
    delete frequencies;
    delete data_file;
    
}


void Huffman::encode(std::string infile_name,std::string outfile_name){

    char* charData;
    int size,start,stop;
    std::string stringData;
    std:: ifstream file(infile_name.c_str(), std::ios::binary);
    int s=0;
    


    if(frequencies) delete frequencies;

    if(!file){
        std::cout<<"Could not open file: " << infile_name << "\n";
        return;
    }

    else{
        

        //gets the file size
        file.seekg(0,std::ios::beg);
        start = file.tellg();
        file.seekg(0,std::ios::end);
        stop = file.tellg();
        size = stop - start;

        //allocate memory to charData
        charData = new char[size];

        //take pointer again to start
        file.seekg(0,std::ios::beg);
        //read the file
        file.read(charData, size);

        file.close();

        //hash map of characters
        frequencies = getFrequencies(charData,size);

        // for(auto itr = frequencies->begin();itr!= frequencies->end();itr++){
        //     std::cout<< (int)itr->first << "freq-(" << itr->second << ") "<<'\n'; 
        // }

        buildTree();
        showTree();

        //inorder(tree);

        encodedRes = doEncoding(charData,size);
        


        writeEncodedFile(infile_name,outfile_name);
        

        for(auto itr = encodedRes.begin();itr!= encodedRes.end();itr++){
            s+=itr->second;
        }

        float comp = 1-((float)s / (float)(size*8));

        std::cout << "\n\nAmount of bits in regular string: " << size * 8;//as size is in bytes convert it in bits
        std::cout << "\nAmount of bits in encoded string: " << s;
        std::cout << "\nCompression: " << comp * 100 << "%";

    }
}

//Decode String

void Huffman::decode(std::string infile_name, std::string outfile_name){

    reCreateTree(infile_name);
    std::cout<<"tree Created"<<'\n';
    writeDecodedFile(infile_name,outfile_name);

    std::cout<<"file is decoded"<<'\n';
    

}

std::vector<std::pair<std::string,int>> Huffman::doEncoding(char* data,int size){
    std::map<char, std::pair<std::string,int>>::iterator itr;
    std::vector<std::pair<std::string,int>> res;
    for(int i=0;i<size;i++){
        itr = encodedMap->find(data[i]);
        res.push_back(itr->second);
    }

    return res;

}

//creates frequency map for all the characters in the data
std::map<char,int>* Huffman::getFrequencies(char* data, int size){
    std::map<char,int>* f =new std::map<char,int>;
    

    for(int i=0;i<size;i++){

        //if not in the map then insert
        if(f->find(data[i]) == f->end()){
            f->insert(std::pair<char,int>(data[i],1));
            
        }
        else{
            (*f)[data[i]]+=1;
        }
        
    }

    return f;

}

// Merge Sort

void Huffman::sort(std::vector<Node*>& v){
    
    if (v.size()<=1) return;
    
    int mid = v.size()/2;
    std::vector<Node*> lefthalf(v.begin(),v.begin()+mid);
    std::vector<Node*> righthalf(v.begin()+mid,v.end());
    
    sort(lefthalf);
    sort(righthalf);

    int i = 0;
    int j = 0;
    int k = 0;
    while (i < lefthalf.size() && j < righthalf.size()) {
        if (lefthalf[i]->getFrequency() <= righthalf[j]->getFrequency()) {
            v[k]=lefthalf[i];
            ++i;
        } else {
            v[k] = righthalf[j];
            ++j;
        }
        ++k;
    }

    while (i<lefthalf.size()) {
        v[k] = lefthalf[i];
        ++i;
        ++k;
    }

    while (j<righthalf.size()) {
        v[k]=righthalf[j];
        ++j;
        ++k;
    }

}



void Huffman::buildTree(){

    int bits =0;
    std::vector<Node*> nodes;
    std::map<char,int> temp = *frequencies;
    std::map<char,int>::iterator itr,lowest;


    //push nodes in nodes vecotr
    while (!temp.empty()) {
        itr    = temp.begin();
        lowest  = itr;

        // Loop through all the elements 
        // and find node with least frequency
        while (itr != temp.end()) {
            if (itr->second < lowest->second) lowest = itr;
            ++itr;
        }
        
        // Create new leafnode and add to vector
        nodes.push_back(new Node(lowest->second,lowest->first));
        temp.erase(lowest);
        
    }
	
	
    while (nodes.size() != 1) {
        // the last node will be the root of the tree
    	// Create new branch node
        // remove used nodes
        // sort all nodes
    	nodes.push_back(new Node(nodes.at(0), nodes.at(1)));
        nodes.erase(nodes.begin(), nodes.begin()+2);
    	sort(nodes);
    }

    // Set tree pointer
    // Fill the tree
    tree = nodes.front();

    //tree->fill(*encodedMap, bits, 0);
    findCodes();
}

void Huffman::showTree(){
    std::map<char,std::pair<std::string,int>>::iterator itr;

    for(itr = encodedMap->begin();itr!= encodedMap->end();itr++){
        std::cout<< (int)itr->first << ": (" << itr->second.second << ") " << itr->second.first << "\n";
    }

}

//builds a tree from huffman codes(taking frequencies as 0)
void Huffman::buildTree(std::string& code, char data){

    //root node
    if(!tree) tree = new Node();
    
    Node* current = tree;
    for(int i = 0;i< code.size();i++){
        if(code[i]=='0'){
            if(current->left==NULL){
                current->left = new Node();
            }
            current=current->left;
        }
        else if(code[i]=='1'){
            if(current->right==NULL){
                current->right = new Node();
            }
            current=current->right;
        }

    }
    current->data = data;
    current->type = LEAF;

}

void Huffman::reCreateTree(std::string infilename){
    
    std:: ifstream inFile(infilename, std::ios::in|std::ios::binary);

    if(!inFile) {
        std::cout<<"Error Opening Input File"<<'\n';
    }

    

    unsigned char size;
    inFile.read(reinterpret_cast<char*>(&size),1);
    

    tree = NULL;

    for (int i = 0; i < size; i++)
	{
		char a_code;
		unsigned char h_code_c[16];//for binary code

        //read the character and the code																
		inFile.read(&a_code, 1);
		inFile.read(reinterpret_cast<char*>(h_code_c), 16);

		std::string h_code_s = "";
		for (int i = 0; i < 16; i++)
		{   //obtain the oringinal 128-bit binary string
			h_code_s += decimal_to_binary(h_code_c[i]);
		}
        // std::cout<<"-------------"<<'\n';
        // std::cout<<h_code_s<<'\n';

		int j = 0;
		while (h_code_s[j] == '0')
		{//delete the added '000..­1' to get the real huffman code
			j++;
		}
		h_code_s = h_code_s.substr(j + 1);
		
        // std::cout<<h_code_s<< "--" << (int)a_code<<'\n';
        // std::cout<<"-------------"<<'\n';
        

        buildTree(h_code_s,a_code);
        
	}
    
	inFile.close();

}


void Huffman::writeDecodedFile(std::string infilename,std::string outfilename){
    std::ifstream inFile(infilename, std::ios::in | std::ios::binary);
	std::ofstream outFile(outfilename, std::ios::out);
	unsigned char size;
    																		
	inFile.read(reinterpret_cast<char*>(&size), 1);
    //jump to last file to get the size of zero padding
	inFile.seekg(-1, std::ios::end);															
	char zeroCount;
	inFile.read(&zeroCount, 1);

    

    //jump to the position where text starts
    //i.e. skip the size(1) char+code(1+16) part
	inFile.seekg(1 + 17 * size, std::ios::beg);													

	std::vector<unsigned char> text;
	unsigned char textseg;

	inFile.read(reinterpret_cast<char*>(&textseg), 1);
	while (!inFile.eof())
	{//get the text byte by byte using unsigned char
		text.push_back(textseg);
		inFile.read(reinterpret_cast<char*>(&textseg), 1);
	}

    //root node of tree
	Node* current = tree;
	std::string code;

    //last text item will be padding size 
    //hence iterating till text.size() - 1
	for (int i = 0; i < text.size() - 1; i++)
	{   //translate the huffman code
		code = decimal_to_binary(text[i]);

               

        //last 8 bits - cut the extra zeros added
		if (i == text.size() - 2){
            code = code.substr(0, 8 - zeroCount);
        }
			
        
        //std::cout<<"-----------------"<<'\n';
        //std::cout<< code << '\n'; 

		for (int j = 0; j < code.size(); j++)
		{
			if (code[j] == '0'){
                current = current->left;
            }	
			else{
                current = current->right;
            }
			if (current->left == NULL && current->right == NULL)
			{
                //std::cout<< (int)current->data << '\n';
                //take care of (carriage return)
                if((int)current->data!=13){
                    outFile.put(current->data);
                }
				
				current = tree;
			}
		}
	}
	inFile.close();
	outFile.close();

    
}
    
std::string Huffman::decimal_to_binary(int in)
{
	std::string temp = "";
	std::string result = "";
	while (in)
	{
		temp += ('0' + in % 2);
		in /= 2;
	}
	result.append(8 - temp.size(), '0');

	for (int i = temp.size() - 1; i >= 0; i--)												
	{
		result += temp[i];
	}
	return result;
}


int Huffman::binary_to_decimal(std::string const& in)
{
	int result = 0;
	for (int i = 0; i < in.size(); i++)
		result = result * 2 + in[i] - '0';
	return result;
}


std::string toBinary(int n)
{
    std::string r="";
    if(n==0) return "0";
    while(n!=0) {
        r=(n%2==0 ?"0":"1")+r; 
        n/=2;
    }
    return r;
}


void Huffman::writeEncodedFile(std::string infilename,std::string outfilename) {

	outfilename += ".henc";
    std::ifstream inFile(infilename, std::ios::in |std::ios::binary);
    std::ofstream outFile(outfilename, std::ios::binary);

    if(!inFile || !outFile){
        std::cout<<"Could not open file" << '\n';
        return;
    }


    std::string in ="", temp;



    //save number of unique characters in the file
    in += (char)(encodedMap->size());

    for(auto itr = encodedMap->begin();itr!=encodedMap->end();itr++){

        //save the character
        in+= itr->first;

        //all codes will be saved in this format [000001 + actual code] 128 bits long
        //where 1 will determine start of huffman code
        //hence zeros needed 128 - 1 - nbits
        //but as 
        temp.assign(127 - itr->second.second,'0');
        
        temp+='1';
        //now append the code
        // std::cout<<toBinary(itr->second.first)<<'\n';
        temp.append(itr->second.first);
        // std::cout<<temp<<'\n';
        

        in += (char)binary_to_decimal(temp.substr(0, 8));

        //cut into 8-bit binary codes that can convert into char
        for (int i = 0; i < 15; i++)
		{
			temp = temp.substr(8);
			in += (char)binary_to_decimal(temp.substr(0, 8));
		}

    }
    

    temp.clear();

    char data;
    inFile.get(data);

    

    //do encoding 
    while (!inFile.eof())
	{
        //get the huffman code
		temp += encodedMap->find(data)->second.first;

        // std::cout<<"-----------------"<<'\n';
        // std::cout<< encodedMap->find(data)->second.first << '\n';
        // std::cout<< toBinary(encodedMap->find(data)->second.first) << '\n';

		while (temp.size() > 8)
		{   //cut into 8-bit binary codes that can convert into saving char
			in += (char)binary_to_decimal(temp.substr(0, 8));
			temp = temp.substr(8);
		}
		inFile.get(data);
	}
    
    
    //create required padding
    int count = 8 - temp.size();
    if(temp.size()< 8){
        temp.append(count, '0');
    }

    in+=(char)binary_to_decimal(temp);

    //last byte represent the padding zeros added
    in+=(char)count;

    //write this string to the file
    outFile.write(in.c_str(), in.size());
    outFile.close();
    inFile.close(); 
    
}


void Huffman::traverse(Node* node, std::string code,std::map<char, std::pair<std::string,int>>& enc)
{
	if (node->left == NULL && node->right == NULL)
	{
		enc[node->data] = std::pair<std::string,int>(code,code.size());
	}
	else
	{
		traverse(node->left, code + '0',enc);
		traverse(node->right, code + '1',enc);
	}
}

void Huffman::inorder(Node* root){
    if(!root) return;
    inorder(root->left);
    std::cout<<"_" <<root->data <<"_" <<root->freq;
    inorder(root->right);
}


void Huffman::findCodes(){
    traverse(tree,"",*encodedMap);
}