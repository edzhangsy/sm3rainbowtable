#pragma once
#include<vector>
#include<string>
class sm3
{
public:
	std::vector<unsigned long> state;
	sm3();
	void initialize();
	void output(const unsigned char* input, int length, char* output);
private:
	void update(unsigned char message[64]);
};

void show(char* output);//show hash value in hex code.
void writeToFile(FILE * out,char output[32]);