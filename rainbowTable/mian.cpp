#include "sm3.h"
#include "rainbowtable.h"
#include <string>
#include <cstdio>
#include <fstream>
#include <chrono>
using namespace std;

int main()
{
	auto start = std::chrono::high_resolution_clock::now();
	sm3 test;
	char output[32];
	rainbowTable table;
	table.generateTable(100);
	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;
	auto start2 = std::chrono::high_resolution_clock::now();
	table.writeout("rainbowtable.txt");
	auto finish2 = std::chrono::high_resolution_clock::now();
	printf("%f s\n", elapsed.count());
	std::chrono::duration<double> elapsed2 = finish2 - start2;
	printf("\n");
	printf("%f s\n", elapsed2.count());

	char hash[] = "c72b37ee 64134cc0 182e9110 0b6e116b f87391ca d2d06af5 e464bd40 55bec081";

	char output[32];
	convertShash(hash, output);
	show(output);
	//rainbowTable table;
	//table.readTable("testbi.txt");
	//test.output(reinterpret_cast<const unsigned char*>(s.c_str()), s.size(), output);
	//string result = table.queryTable(output, 1000);
	//if (result.size() != 0) {
	//	printf("%s", result.c_str());
	//}
	//else {
	//	printf("empty string");
	//}


	return 0;
}