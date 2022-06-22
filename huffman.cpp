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
}

//Deconstructor
Huffman::~Huffman(){
    delete encodedMap;
    delete frequencies;
}

//----------------Huffman Encode----------------//

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

    else if(isFileEmpty(file)){
        std::cout<<"File "<< infile_name <<" is empty..."<<'\n';
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
        file.read(charData, size);
        file.close();

        
        frequencies = getFrequencies(charData,size);

        buildTree();

        findCodes();

        showTree();

        writeEncodedFile(infile_name,outfile_name);
        

        //find bits in actual file
        for(auto itr = encodedMap->begin();itr!=encodedMap->end();itr++){
            auto it = frequencies->find(itr->first);
            s+= it->second * itr->second.second;
        }

        //find compression percentage
        float comp = 1-((float)s / (float)(size*8));

        //as 'size' is in bytes convert it in bits
        std::cout << "\n\nAmount of bits in regular file: " << size * 8;
        std::cout << "\nAmount of bits in encoded file: " << s;
        std::cout << "\nCompression: " << comp * 100 << "%";

    }
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

//creates Huffman Tree
void Huffman::buildTree(){

    int bits =0;
    std::vector<Node*> nodes;

    //create nodes and push them in the vector
    for(auto itr = frequencies->begin();itr != frequencies->end();itr++){
        nodes.push_back(new Node(itr->second,itr->first));
    }

	sort(nodes);
	
    //Create the huffman tree 
    while (nodes.size() != 1) {
    	// Create new branch node
    	nodes.push_back(new Node(nodes.at(0), nodes.at(1)));

        // remove used nodes
        nodes.erase(nodes.begin(), nodes.begin()+2);
        
        // sort all nodes
    	sort(nodes);
    }

    //the last node remaining in the vector will be the root of the tree
    tree = nodes.front();
    
}

// Merge Sort(ascending order by frequency)
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
        if (lefthalf[i]->freq <= righthalf[j]->freq) {
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

//simply calls traverse that stores codes in encodedMap
void Huffman::findCodes(){
    traverse(tree,"",*encodedMap);
}

//traverse and find code for each leaf node
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

//Prints Huffman Codes
void Huffman::showTree(){

    for(auto itr = encodedMap->begin();itr!= encodedMap->end();itr++){
        std::cout<< "Character (" <<itr->first << ") Code (" << itr->second.first << ")\n";
    }

}

//Write the character and code first 
//Then encode the actual data
void Huffman::writeEncodedFile(std::string infile_name,std::string outfile_name) {
    
    //file extension for encoded file
	outfile_name += ".henc";

    std::ifstream inFile(infile_name, std::ios::in |std::ios::binary);
    std::ofstream outFile(outfile_name, std::ios::binary);

    if(!inFile || !outFile){
        std::cout<<"Could not open file" << '\n';
        return;
    }

    //in stores the whole string to be written in the encoded file
    std::string in ="", temp;

    //--------Save the codes first--------

    //save number of unique characters in the file
    in += (char)(encodedMap->size());

    for(auto itr = encodedMap->begin();itr!=encodedMap->end();itr++){

        //save the character
        in+= itr->first;

        //all codes will be saved in this format [000001 + actual code] 128 bits long
        //where 1 will determine start of huffman code
        //hence zeros needed 128 - 1 - no. of bits of code
        temp.assign(127 - itr->second.second,'0');
        
        temp+='1';

        //now append the code
        temp.append(itr->second.first);
        
        in += (char)binary_to_decimal(temp.substr(0, 8));

        //cut into 8-bit binary codes that can convert into char
        for (int i = 0; i < 15; i++)
		{
			temp = temp.substr(8);
			in += (char)binary_to_decimal(temp.substr(0, 8));
		}

    }
    
    temp.clear();

    //--------start encoding of actual file--------
    char data;
    inFile.get(data);
    //do encoding 
    while (!inFile.eof())
	{
        //get the huffman code
		temp += encodedMap->find(data)->second.first;

		while (temp.size() > 8)
		{   
            //cut into 8-bit binary codes that can convert into saving char
			in += (char)binary_to_decimal(temp.substr(0, 8));
			temp = temp.substr(8);
		}
		inFile.get(data);
	}
    
    
    //create required zero padding
    //to the the no of bits = 8 
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



//----------------Huffman Decode----------------//

void Huffman::decode(std::string infile_name, std::string outfile_name){

    reCreateTree(infile_name);
    writeDecodedFile(infile_name,outfile_name);
}


//Reads the encoded file and the codes
//and builds a tree using huffman codes
void Huffman::reCreateTree(std::string infile_name){
    
    std:: ifstream inFile(infile_name, std::ios::in|std::ios::binary);

    if(!inFile) {
        std::cout<<"Error Opening Input File "<< infile_name <<'\n';
        return;
    }
    if(isFileEmpty(inFile)){
        std::cout<<"File"<< infile_name << "is empty..."<<'\n';
        return;
    }
    

    unsigned char size;
    inFile.read(reinterpret_cast<char*>(&size),1);
    

    tree = NULL;

    for (int i = 0; i < size; i++)
	{
		char data;
        //for binary code
		unsigned char h_code_c[16];

        //read the character and the code																
		inFile.read(&data, 1);
		inFile.read(reinterpret_cast<char*>(h_code_c), 16);

		std::string h_code_s = "";
		for (int i = 0; i < 16; i++)
		{   //obtain the oringinal 128-bit binary string
			h_code_s += decimal_to_binary(h_code_c[i]);
		}

        //delete the added '000..­1' to get the real huffman code
		int j = 0;
		while (h_code_s[j] == '0')
		{
			j++;
		}
		h_code_s = h_code_s.substr(j + 1);
        
        //add a node in the tree
        buildTree(h_code_s,data);
        
	}
    
	inFile.close();

    std::cout<<"Huffman tree is Created!"<<'\n';

}

//Add nodes to the tree
void Huffman::buildTree(std::string& code, char data){

    //root node
    if(!tree) tree = new Node();
    
    Node* current = tree;
    for(int i = 0;i< code.size();i++){
        //if 0 go left
        if(code[i]=='0'){
            if(current->left==NULL){
                current->left = new Node();
            }
            current=current->left;
        }
        //if 1 go right
        else if(code[i]=='1'){
            if(current->right==NULL){
                current->right = new Node();
            }
            current=current->right;
        }

    }
    //put the data in the leaf node
    current->data = data;
    current->type = LEAF;

}

//Write decoded file
//by traversing the tree and finding the leaf data
void Huffman::writeDecodedFile(std::string infile_name,std::string outfile_name){
    std::ifstream inFile(infile_name, std::ios::in | std::ios::binary);
	std::ofstream outFile(outfile_name, std::ios::out);

    if(!inFile) {
        std::cout<<"Error Opening Input File "<< infile_name <<'\n';
        return;
    }
    if(isFileEmpty(inFile)){
        std::cout<<"File"<< infile_name << "is empty..."<<'\n';
        return;
    }


	unsigned char size;
    																		
	inFile.read(reinterpret_cast<char*>(&size), 1);

    //jump to last file to get the size of zero padding
	inFile.seekg(-1, std::ios::end);															
	char zeroCount;
	inFile.read(&zeroCount, 1);

    //jump to the position where the actual text starts
    //i.e. skip the size(1) char+code(1+16) part
	inFile.seekg(1 + 17 * size, std::ios::beg);													

	std::vector<unsigned char> text;
	unsigned char textseg;

	inFile.read(reinterpret_cast<char*>(&textseg), 1);
    //get the text byte by byte using unsigned char
	while (!inFile.eof())
	{
		text.push_back(textseg);
		inFile.read(reinterpret_cast<char*>(&textseg), 1);
	}

    //root node of tree
	Node* current = tree;
	std::string code;

    //last text item will be padding size 
    //hence iterating till text.size() - 1
	for (int i = 0; i < text.size() - 1; i++)
	{   
        //translate the huffman code
		code = decimal_to_binary(text[i]);

        //last 8 bits - cut the extra zeros added
		if (i == text.size() - 2){
            code = code.substr(0, 8 - zeroCount);
        }
			
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
                
                //don't put ASCII 13 (carriage return)
                //as it creates extra line
                if((int)current->data!=13){
                    outFile.put(current->data);
                }
				current = tree;
			}
		}
	}
	inFile.close();
	outFile.close();

    std::cout<<"File is decoded!"<<'\n';
    
}



//--------Helper Functions--------

bool Huffman::isFileEmpty(std::ifstream& pFile)
{
    return pFile.peek() == std::ifstream::traits_type::eof();
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

