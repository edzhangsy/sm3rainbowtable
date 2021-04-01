#include "rainbowtable.h"
#include "sm3.h"
#include <fstream>
#include <sstream>
#include<iomanip>
#include <string>
#include <cstdio>
#include <cstdlib>
//#define	DEBUG
using namespace std;

string reductionFunc(const char * hash, int i) {
	string output;
	for (int j = 0; j < 5; j++) {
		char a = characterSet[(*((unsigned long*)(hash)+j) + i) % setSize];
		output.append(1, a);
	}
	return output;
}

void rainbowTable::generateTable(int chainLength)
{

	sm3 generator;
	char output[33];
	output[32] = 0;
	//string s("01234");

	for (int i = 0; i <= 999999; i++) {
		memset(output, 0, 32);
		stringstream ss;
		ss << setw(6) << setfill('0') << i;
		string s = ss.str();
		/*fwrite(s.c_str(), sizeof(char), s.size(), out);*/
		beginString.push_back(s);
		for (int j = 0; j < chainLength; j++) {
			generator.output(reinterpret_cast<const unsigned char*>(s.c_str()), s.size(), output);
#ifdef DEBUG
		printf("%d, %s ", j, s.c_str());
#endif // DEBUG
			s = reductionFunc(output, j);
		}
		endHash.push_back(s);
		/*fwrite(output, sizeof(char), 32, out);*/
	}

}
void rainbowTable::writeout(const char* filename) {
	FILE* out;
	auto err = fopen_s(&out, filename, "wb");
	for (int i = 0; i < beginString.size(); i++) {
		fwrite(beginString[i].c_str(), sizeof(char), beginString[i].size(), out);
		fwrite(endHash[i].c_str(), sizeof(char), endHash[i].size(), out);
	}
	fclose(out);
}
void rainbowTable::reset() {
	beginString.clear();
	endHash.clear();
}

void rainbowTable::readTable(const char* fileName) {
	reset();
	FILE* in;
	char start[6];
	start[5] = 0;
	char hash[6];
	hash[5] = 0;
	auto err = fopen_s(&in, fileName, "rb");
	while (fread(start, sizeof(char), 5, in)) {
		beginString.push_back(string(start));
		fread(hash, sizeof(char), 5, in);
		endHash.push_back(string(hash));
	}
	fclose(in);
}

std::string rainbowTable::queryTable(const char* hash, int chainLength)
{
	sm3 generator;
	char tmp[33];//tmp result;
	char newhash[33];
	memcpy(tmp, hash, 32);
	memset(tmp + 32, 0, 1);
	memcpy(newhash, tmp, 33);
	string s;//store reduced result
	for (int i = 0; i < chainLength; i++) {
		memcpy(tmp, hash, 32);
		memset(tmp + 32, 0, 1);
		for (int j = i; j < chainLength; j++) {
			s = reductionFunc(tmp, j);
			generator.output(reinterpret_cast<const unsigned char*>(s.c_str()), s.size(), tmp);
		}//assume hash is in position j
		for (int k = 0; k < endHash.size(); k++) {
			if (endHash[k] == s) {
				string cmp = beginString[k];
				char output[33];//tmp string
				output[32] = 0;
				for (int l = 0; l < i; l++) {
					generator.output(reinterpret_cast<const unsigned char*>(cmp.c_str()), cmp.size(), output);
					cmp = reductionFunc(output, l);
				}
				generator.output(reinterpret_cast<const unsigned char*>(cmp.c_str()), cmp.size(), output);
				if (!strcmp(output, newhash)) {
					return cmp;
				}
			}
		}
	}
	return std::string();
}
