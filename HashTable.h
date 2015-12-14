#include <iostream>
#include <string>

#include "LinkedList.h"

using namespace std;

const int NOT_FOUND = -1;
const int DISTRIBUTE = 31;
const int UP = 1;
const int DOWN = 0;
const int MAX_PER_LINE = 8;

unsigned hashCode(const string& s) {
	unsigned hashIndex = 0;
	for (const char &c : s) {
		hashIndex *= DISTRIBUTE;
		hashIndex += c;
	}
	return hashIndex;
}

template<class T>
class HashTable {
private:
	int tableSize_;
	int size_;
	LinkedList<T>* table;
	
public:
	
	HashTable() : tableSize_(0), size_(0), table(new LinkedList<T>[tableSize_]) {}
	~HashTable() { delete[] table; }
	
	void rehash(int type) {
		LinkedList<T>* oldTable = table;
		int oldTableSize = tableSize_;
		if (type == UP)
			tableSize_ = tableSize_ * 2 + 1;
		else if (type == DOWN)
			tableSize_ = (tableSize_ - 1) / 2;
		table = new LinkedList<T>[tableSize_];
		if (tableSize_ > 0) {
			for (int i = 0; i < oldTableSize; i++) {
				while (oldTable[i].size() > 0) {
					T item = oldTable[i].removeHead();
					unsigned index = hashCode(item) % tableSize_;
					table[index].insertTail(item);
				}
			}
		}
		delete[] oldTable;
	}
	
	int find(const T& item) {
		if (tableSize_ > 0) {
			unsigned index = hashCode(item) % tableSize_;
			return table[index].find(item);
		}
		else return NOT_FOUND;
	}
	
	void insert(const T& item) {
		if (find(item) != NOT_FOUND) return;
		size_++;
		if (size_ > tableSize_) { 
			rehash(UP);
		}
		unsigned index = hashCode(item) % tableSize_;
		table[index].insertTail(item);
	}
	
	void remove(const T& item) {
		if (find(item) == NOT_FOUND) return;
		unsigned index = hashCode(item) % tableSize_;
		table[index].remove(table[index].find(item));
		size_--;
		if (size_ <= tableSize_ / 2) rehash(DOWN);
	}
	
	void clear() {
		if (tableSize_ > 0) {
			size_ = 0;
			tableSize_ = 0;
			LinkedList<T>* oldTable = table;
			table = new LinkedList<T>[tableSize_];
			delete[] oldTable;
		}
	}
	
	string toString() {
		stringstream ss;
		for (int i = 0; i < tableSize_; i++) {
			int j = 0;
			do {
				ss << endl << "hash " << i << ":";
				for (int k = 0; k < MAX_PER_LINE && j < table[i].size(); k++) {
					ss << " " << table[i][j];
					j++;
				}
			} while (j < table[i].size());
		}
		ss << endl;
		return ss.str();
	}
	
	int size() { return size_; }
};