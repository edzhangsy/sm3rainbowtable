#pragma once
#include<string>
#include<vector>
#include<string>
const char characterSet[] = {
	'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l',
	'm', 'n', 'o', 'p', 'q', 'r',
	's', 't', 'u', 'v', 'w', 'x',
	'y', 'z', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9'
};
static const int setSize = 36;

void genetateTable(const char* filename, int chainLength);

class rainbowTable {
public:
	void readTable(const char* fileName);
	std::string queryTable(const char* hash, int chainLength);
	void generateTable(int chainLength);
	void writeout(const char* filename);
private:
	void reset();
	std::vector<std::string> beginString;
	std::vector<std::string> endHash;
};
