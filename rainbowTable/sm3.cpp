#include "sm3.h"
#include <vector>
#include <string>
#include <cstdio>
#include <cstring>

//#define DEBUG
using namespace std;
unsigned long* reverseWord(unsigned long* converter) {
	unsigned char* byte, tmp;
	byte = (unsigned char*)converter;
	tmp = byte[0];
	byte[0] = byte[3];
	byte[3] = tmp;
	tmp = byte[1];
	byte[1] = byte[2];
	byte[2] = tmp;
	return converter;
}


inline unsigned long leftRotate(unsigned long converter, int bits) {
	return converter = (converter << bits) | (converter >> (32 - bits));

}


unsigned long T(int i) {
	if (i >= 0 && i <= 15) {
		return 0x79cc4519;
	}
	else if (i >= 16 && i <= 63) {
		return 0x7A879D8A;
	}
	else
	{
		return 0;
	}
}

unsigned long FF(unsigned long X, unsigned long Y, unsigned long Z, const int j) {
	if (j >= 0 && j <= 15) {
		return X ^ Y ^ Z;
	}
	else if (j>=16 && j<= 63)
	{
		return (X & Y) | (X & Z) | (Y & Z);
	}
	else
	{
		return 0;
	}
}

unsigned long GG(unsigned long X, unsigned long Y, unsigned long Z, const int j) {
	if (j >= 0 && j <= 15) {
		return X ^ Y ^ Z;
	}
	else if (j >= 16 && j <= 63)
	{
		return (X & Y) | (~X & Z);
	}
	else
	{
		return 0;
	}
}

unsigned long P0(unsigned long X) {
	return X ^ leftRotate(X, 9) ^ leftRotate(X, 17);
}

unsigned long P1(unsigned long X) {
	return X ^ (leftRotate(X, 15)) ^ (leftRotate(X, 23));
}
void sm3::initialize() {
	state.clear();
	state.push_back(0x7380166f);
	state.push_back(0x4914b2b9);
	state.push_back(0x172442D7);
	state.push_back(0xDA8A0600);
	state.push_back(0xA96F30BC);
	state.push_back(0x163138AA);
	state.push_back(0xE38DEE4D);
	state.push_back(0xB0FB0E4E);
}

sm3::sm3() {
	initialize();

}

void sm3::update(unsigned char message[64]) {
	unsigned long W[68];
	unsigned long W1[64];
	unsigned long A(state[0]), B(state[1]), C(state[2]), D(state[3]), E(state[4]), F(state[5]), G(state[6]), H(state[7]), SS1(0), SS2(0), TT1(0), TT2(0);
	for (int i = 0; i < 16; i++) {
		W[i] = *((unsigned long*)(message) + i);
		reverseWord(W + i);
	}
	for (int i = 16; i < 68; i++) {
		W[i] = P1(W[i - 16] ^ W[i - 9] ^ (leftRotate(W[i - 3], 15)) ) ^ (leftRotate(W[i - 13], 7)) ^ W[i - 6];
	} 
#ifdef DEBUG
	printf("Expanding message W0-67:\n");
	for (int i = 0; i < 68; i++)
	{
		printf("%08x ", W[i]);
		if (((i + 1) % 8) == 0) printf("\n");
	}
	printf("\n");
#endif // DEBUG



	for (int i = 0; i < 64; i++) {
		W1[i] = W[i] ^ W[i + 4];
	}
#ifdef DEBUG
	printf("Expanding message W'0-63:\n");
	for (int i = 0; i < 64; i++)
	{
		printf("%08x ", W1[i]);
		if (((i + 1) % 8) == 0) printf("\n");
	}
	printf("\n");
#endif

#ifdef DEBUG
	printf("j     A       B        C         D         E        F        G       H\n");
	printf("   %08x %08x %08x %08x %08x %08x %08x %08x\n", A, B, C, D, E, F, G, H);
	printf("\n");
#endif
	for (int i = 0; i < 64; i++) {
		SS1 = leftRotate((leftRotate(A, 12)) + E + (leftRotate(T(i), i)), 7);
		SS2 = SS1 ^ (leftRotate(A, 12));
		TT1 = FF(A, B, C, i) + D + SS2 + W1[i];
		TT2 = GG(E, F, G, i) + H + SS1 + W[i];
		D = C;
		C = leftRotate(B, 9);
		B = A;
		A = TT1;
		H = G;
		G = leftRotate(F, 19);
		F = E;
		E = P0(TT2);
	}

	state[0] ^= A;
	state[1] ^= B;
	state[2] ^= C;
	state[3] ^= D;
	state[4] ^= E;
	state[5] ^= F;
	state[6] ^= G;
	state[7] ^= H;
#ifdef DEBUG
	printf("j     A       B        C         D         E        F        G       H\n");
	printf("   %08x %08x %08x %08x %08x %08x %08x %08x\n", A, B, C, D, E, F, G, H);
#endif
}

void sm3::output(const unsigned char* input, int length, char* output) {
	unsigned char message[64];
	int size = length / 64;
	for (int i = 0; i < size; i++) {
		memcpy(message, &(input[i*64]) , 64);
		update(message);
	}
	unsigned long bitlength = length * 8;
	reverseWord(&bitlength);
	int remain = length % 64;
	memcpy(message, &(input[size * 64]), remain);
	message[remain] = 0x80;
	if (remain <= 55) {
		memset(message + remain + 1, 0, 64 - remain - 4 - 1);
		memcpy(message + 64 - 4, &bitlength, 4);
		update(message);
	}
	else
	{
		memset(message + remain + 1, 0, 64 - remain - 1);
		update(message);
		memset(message, 0, 64 - 4);
		memset(message + 64 - 4, bitlength, 4);
		update(message);
	}
	for (int i = 0; i < state.size(); i++) {
		unsigned long tmp = state[i];
		reverseWord(&tmp);
		memcpy((output + i * 4), &tmp, 4);
		
	}
	initialize();
}

void show(char* output)
{
	unsigned char* tmp = reinterpret_cast<unsigned char*>(output);
	for (int i = 0; i < 32; i++) {
		printf("%02x", tmp[i]);
		if (i % 4 == 3) {
			printf("%c", ' ');

		}
	}
}
void writeToFile(FILE * stream, char output[32])
{
	unsigned char* tmp = reinterpret_cast<unsigned char*>(output);
	for (int i = 0; i < 32; i++) {
		fprintf(stream, "%02x", tmp[i]);
		if (i % 4 == 3) {
			fprintf(stream, "%c", ' ');
		}
	}
}