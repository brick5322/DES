#pragma once

typedef unsigned long long uint64_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint48_t;

class DES
{
	union
	{
		uint64_t data64;
		uint32_t data32[2];
	}data;
	uint48_t buffer;
	uint64_t currentKey;
	bool currentOperatorSide;
	int Keycnt;
public:
	DES(uint64_t data,uint64_t key);
	uint64_t Encrypt();
	uint64_t Decrypt();
private:
	void Initial_Permutations();
	void Final_Permutations();
	void Expand();
	uint32_t Substitution();
	void PBox();
	void DoXor(uint64_t&,const uint64_t&);
	void DoXor(uint32_t&,const uint32_t&);
	uint48_t GetKey();
};
