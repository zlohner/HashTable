#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "HashTable.h"

using namespace std;

enum Error { USAGE, INVALID_FILE, INVALID_COMMAND };

string add(HashTable<string>& hashTable, string value) {
    stringstream ss;
    hashTable.insert(value);
    ss << "add " << value;
    return ss.str();
}

string remove(HashTable<string>& hashTable, string value) {
    stringstream ss;
    hashTable.remove(value);
    ss << "remove " << value;
    return ss.str();
}

string find(HashTable<string>& hashTable, string value) {
    stringstream ss;
    int found = hashTable.find(value);
    ss << "find " << value << " ";
	if (found != NOT_FOUND) ss << "true";
	else ss << "false";
    return ss.str();
}

string clear(HashTable<string>& hashTable) {
    hashTable.clear();
    return "clear";
}

string print(HashTable<string>& hashTable) {
    return "print" + hashTable.toString();
}

string executeCommand(HashTable<string>& hashTable, string line) {
    stringstream command(line);
    stringstream ss;
    
    string type;
    command >> type;
    if (type == "add") {
        string value;
        command >> value;
        ss << add(hashTable, value) << endl;
    }
    else if (type == "remove") {
        string value;
        command >> value;
        ss << remove(hashTable, value) << endl;
    }
    else if (type == "find") {
        string value;
        command >> value;
        ss << find(hashTable, value) << endl;
    }
    else if (type == "clear") {
        ss << clear(hashTable) << endl;
    }
    else if (type == "print") {
        ss << print(hashTable);
    }
    else throw INVALID_COMMAND;
    
    return ss.str();
}

// void test() {
// 	HashTable<int>* hashTable = new HashTable<int>();
//
// 	hashTable->insert(8);
// 	hashTable->insert(4);
// 	hashTable->insert(2);
// 	hashTable->insert(1);
// 	hashTable->insert(3);
// 	hashTable->insert(6);
// 	hashTable->insert(5);
// 	hashTable->insert(7);
// 	hashTable->insert(12);
// 	hashTable->insert(10);
// 	hashTable->insert(9);
// 	hashTable->insert(11);
// 	hashTable->insert(14);
// 	hashTable->insert(13);
// 	hashTable->insert(15);
//
// 	cout << hashTable->toStringInorder() << endl;
// 	cout << hashTable->toStringPreorder() << endl;
// 	cout << hashTable->toString() << endl;
//
// 	hashTable->remove(8);
// 	hashTable->remove(5);
// 	hashTable->remove(7);
// 	hashTable->remove(6);
// 	hashTable->remove(9);
// 	hashTable->remove(10);
// 	hashTable->remove(11);
// 	hashTable->remove(12);
// 	hashTable->remove(13);
// 	hashTable->remove(4);
// 	hashTable->remove(14);
// 	hashTable->remove(2);
// 	hashTable->remove(3);
// 	hashTable->remove(15);
// 	hashTable->remove(1);
//
// 	cout << hashTable->toStringInorder() << endl;
// 	cout << hashTable->toStringPreorder() << endl;
// 	cout << hashTable->toString() << endl;
//
// 	delete hashTable;
// }

void checkArgs(int argc, char* argv[]) {
	if (argc != 3) throw USAGE;
}

int main(int argc, char* argv[]) {

    try {
        checkArgs(argc, argv);
        
        HashTable<string> hashTable;
        
        ifstream inFile(argv[1]);
        if (!inFile.is_open()) throw INVALID_FILE;
        
        ofstream outFile(argv[2]);
        if (!outFile.is_open()) throw INVALID_FILE;
            
        string line;
        while (getline(inFile, line)) {
            outFile << executeCommand(hashTable, line);
        }
    }
    catch (Error e) {
        if (e == USAGE) cout << "ERROR - USAGE: ./HashTable [command_file] [output_file]" << endl;
		else if (e == INVALID_FILE) cout << "ERROR - INVALID FILE" << endl;
		else if (e == INVALID_COMMAND) cout << "ERROR - INVALID COMMAND" << endl;
    }
	catch (out_of_range e) {
		cout << "ERROR - INDEX OUT OF RANGE" << endl;
	}
	
	return 0;
}