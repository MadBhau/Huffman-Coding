#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<map>
#include<algorithm>

#include"huffman.h"
#include"Node.h"


Huffman::Huffman(){
    encodedMap = new std::map<char, std::pair<int,int>>;
    frequencies = NULL;
    data_file = NULL;
    table_file = NULL;
}

//Deconstructor
Huffman::~Huffman(){
    delete encodedMap;
    delete frequencies;
    delete data_file;
    delete table_file;
}


void Huffman::encode(std::string file_name){

    

    char* charData;
    int size,start,stop;
    std::string stringData;
    std:: ifstream file(file_name.c_str(), std::ios::binary);
    int s=0;
    std::vector<std::pair<int,int>>::iterator itr;


    if(frequencies) delete frequencies;

    if(!file){
        std::cout<<"Could not open file: " << file_name << "\n";
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

        

        //hash map of characters
        frequencies = getFrequencies(charData,size);

        buildTree();
        showTree();

        encodedRes = doEncoding(charData,size);
        createDataFile(encodedRes);

        //why pointer??___________
        createTableFile(*encodedMap);

        if(!writeDataFile(file_name)) return;
        if(!writeTableFile(file_name)) return;

        for(itr = encodedRes.begin();itr!= encodedRes.end();itr++){
            s+=itr->second;
        }

        float comp = 1-((float)s / (float)(size*8));

        std::cout << "\n\nAmount of bits in regular string: " << size * 8;//as size is in bytes convert it in bits
        std::cout << "\nAmount of bits in encoded string: " << s;
        std::cout << "\nCompression: " << comp * 100 << "%";

    }
}

//Decode String

void Huffman::decode(std::string data_filename, std::string table_filename){

    //complete this
    
    //return;

    //still not working
    readTableFile(table_filename);
    //writeTableFile("note");

}

std::vector<std::pair<int,int>> Huffman::doEncoding(char* data,int size){
    std::map<char, std::pair<int,int>>::iterator itr;
    std::vector<std::pair<int,int>> res;
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
    tree->fill(*encodedMap, bits, 0);

}

void Huffman::showTree(){
    std::map<char,std::pair<int,int>>::iterator itr;

    for(itr = encodedMap->begin();itr!= encodedMap->end();itr++){
        std::cout<< itr->first << ": (" << itr->second.second << ") " << itr->second.first << "\n";
    }

}

void Huffman::createDataFile(std::vector<std::pair<int,int>>& v){

    int bitpos, bit;

    if(data_file) delete data_file;
    data_file = new dataFile;
    //nbsp character
    data_file->key = 0xA0;

    int size=0, poff;
    
    for(auto it=v.begin(); it != v.end();it++){
        size += it->second;
    }

    // calculate no of bytes required(ceil to multiple of 8) 
    // extra padding required to get till multiple of 8   
    poff = size;
    while (poff > 0) poff -= 8;
    poff *= -1;
    size+=poff;
    size /=8;//bits to bytes

    //
    data_file->size = size;
    data_file->data = new char[data_file->size];
    data_file->offset = poff;

    int charpos =0;
    int charbitpos = 7;

    for(auto itr=v.begin(); itr!= v.end(); itr++){

        //filling every byte data
        for(bitpos = itr->second-1;bitpos >=0; bitpos --){

            //Extract Bits
            bit = (itr->first >> bitpos) & 1;


            if(charbitpos < 0){
                charbitpos = 7;
                charpos ++;
            }

            if(charbitpos == 7){
                data_file->data[charpos] = 0x0;
            }

            //add it 
            data_file->data[charpos] = (data_file->data[charpos]) | bit << charbitpos;
            charbitpos--;
        }
    }
}

void Huffman::createTableFile(std::map<char, std::pair<int,int>>& m){

    if(table_file) delete table_file;
    table_file = new tableFile;

    tableEntry temp;
    table_file->key = 0xA0;

    for (auto it = encodedMap->begin(); it != encodedMap->end(); it++) {
        temp.data = it->first;
        temp.bits = it->second.first;
        temp.size = it->second.second;
        table_file->data.push_back(temp);
    }

}


bool Huffman::readDataFile(std::string str){

    char charbuffer;
    char buffer[256];
    char* databuffer;
    
    if(data_file) delete data_file;
    data_file = new dataFile;

    std::ifstream file(str.c_str(),std::ios::binary);

    

    

    if(!file){
        std::cout << "Could not open datafile\n";
		file.close();
		return false;
    }else{
        // Read key and check contents
		file.read(&charbuffer, 1);
		if (charbuffer != 0xA0) {
			std::cout << "Invalid datafile0\n";
			file.close();
			return false;
		}

        try {
        // Read the datafile contents
            while (!file.eof()) {
                // Read the size
                file.read(buffer, 4);
                data_file->size = (int)buffer;
                if (file.fail()) throw "Error reading datafile1";

                //create buffer to read the data
                databuffer = new char[data_file->size];
                
                // Read the padding
                file.read(&charbuffer, 1);
                data_file->offset = charbuffer;
			    if (file.fail()) throw "Error reading datafile2";
            
                // Read the encoded data
                file.read(databuffer, data_file->size);
                data_file->data = databuffer;

    			if (file.fail()) throw "Error reading datafile3";
                
                
            }
        }
        catch (char* err) {
            file.close();
            std::cout << err << "\n";
            return false;
        }
	}
	file.close();
    return true;
}
    

bool Huffman::readTableFile(std::string str){
    char charbuffer;
	char buffer[256];
	tableEntry temp;
	
	if (table_file) delete table_file;
	table_file =    new tableFile;
	
	std::ifstream file(str.c_str(), std::ios::binary);
	
	if (!file) {
		std::cout << "Could not open tablefile\n";
		file.close();
		return false;
	}
	else {
		
		// Read key and check contents
		file.read(&charbuffer, 1);
		if (charbuffer != -96) {
			std::cout << "Invalid tablefile\n"<<charbuffer;
			file.close();
			return false;
		}
		
        try {
        // Read the table entries
            while (file.read(&charbuffer, 1)) {
                
                // Read the char in question
                
                temp.data = charbuffer;
                if (file.fail()) throw "Error reading tablefile";
                std::cout<<"here"<<'\n';
                
                // Read the size (number of bits) this encoded char uses
                file.read(&charbuffer, 1);
                temp.size = charbuffer;
			    if (file.fail()) throw "Error reading tablefile";
                std::cout<<"here1"<<'\n';

            
                // Read the bits for this encoded char
                file.read(buffer, 4);
                temp.bits = (int)buffer;
    			if (file.fail()) throw "Error reading tablefile";
                std::cout<<"here2"<<'\n';
                

                
                // Append the new entry into the data vector
                table_file->data.push_back(temp);
                std::cout<<"here3"<<'\n';

            }
        }
        catch (const char* err) {
            file.close();
            std::cout << err << "\n";
            return false;
        }
	}
	file.close();
    return true;

}


bool Huffman::writeTableFile(std::string str) {
	str += ".htbl";
    std::ofstream file(str.c_str(), std::ios::binary);
    

    if (!file) {
        std::cout << "Could not write Tablefile for encoded data\n";
        file.close();
        return false;
    }
    else {
        file.write(&table_file->key, 1);

        for (auto it = table_file->data.begin(); it != table_file->data.end(); it++) {
            file.write(&it->data, 1);
            file.write(&it->size, 1);
            file.write((char*)&it->bits, 4);
        }
    }
    file.close();
    return true;
}

bool Huffman::writeDataFile(std::string str) {
	str += ".hdta";
    std::ofstream file(str.c_str(), std::ios::binary);

    if (!file) {
        std::cout << "Could not write Datafile for encoded data\n";
        file.close();
        return false;
    }
    else {
        file.write(&data_file->key, 1);
        file.write((char*)&data_file->size, 4);
        file.write(&data_file->offset,1);
        file.write((char*)data_file->data, data_file->size);
    }
    file.close();
    return true;
}


//------------Helper Functions---------------

void Huffman::readTableFileContent(){

    for(auto it = table_file->data.begin(); it!=table_file->data.end();it++){
        std::cout<<"\n\n";
        std::cout<<it->data<<' '<<(int)it->size << ' '<<it->bits << '\n';
    }
    


};