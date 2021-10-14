#include "DES.h"

/**
 * @brief 生成一个DES对象用于解密或加密
 * @param data 要加密的数据（64位）
 * @param key  密钥（64位）
 * @details 不应该生成非临时对象，因为密钥不会被保存
 **/
DES::DES(uint64_t data,uint64_t key) :buffer((uint64_t)0),currentKey(key),Keycnt(0),currentOperatorSide(0)
{
	this->data.data64 = data;
	bool stack[56];
	bool* sp = stack;
	const char pc1[56] =
	{
		60,52,44,36,59,51,43,35,
		27,19,11, 3,58,50,42,34,
		26,18,10, 2,57,49,41,33,
		25,17, 9, 1,28,20,12, 4,
		61,53,45,37,29,21,13, 5,
		62,54,46,38,30,22,14, 6,
		63,55,47,39,31,23,15, 7
	};
	for (int i = 0; i < 56; i++)
	{
		*sp = currentKey & ((uint64_t)1 << (pc1[i]));
		sp++;
	}
	currentKey = 0;
	for (int i = 0; i < 56; i++)
	{
		currentKey <<= 1;
		sp--;
		currentKey |= *sp;
	}
}


/**
 * @brief 根据数据和密钥生成一个64位的密文
 * @return 生成的密文（64位）
 **/
uint64_t DES::Encrypt()
{
	currentOperatorSide = 0;
	Initial_Permutations();
	for (int i = 0; i < 16; i++)
	{
		Expand();
		DoXor(buffer, GetKey());
		uint32_t Li = Substitution();
		PBox();
		DoXor(data.data32[!currentOperatorSide], data.data32[currentOperatorSide]);
		data.data32[currentOperatorSide] = Li;
		buffer = 0;

	}
	Final_Permutations();
	return data.data64;
}

/**
 * @brief 根据密文和密钥解密出64位的数据
 * @return 解出的数据（64位）
 **/
uint64_t DES::Decrypt()
{
	currentOperatorSide = 1;
	uint48_t keys[16];

	for (int i = 0; i < 16; i++)
		keys[i] = GetKey();

	Initial_Permutations();

	for (int i = 0; i < 16; i++)
	{
		Expand();
		DoXor(buffer, keys[15-i]);
		uint32_t Ri = Substitution();
		PBox();
		DoXor(data.data32[!currentOperatorSide], data.data32[currentOperatorSide]);
		data.data32[currentOperatorSide] = Ri;
		buffer = 0;
	}

	Final_Permutations();
	return data.data64;
}

void DES::Initial_Permutations()
{
	bool stack[64];
	bool* sp = stack;

	const char bit[64] = {
		57,49,41,33,25,17, 9, 1,
		59,51,43,35,27,19,11, 3,
		61,53,45,37,29,21,13, 5,
		63,55,47,39,31,23,15, 7,
		56,48,40,32,24,16, 8, 0,
		58,50,42,34,26,18,10, 2,
		60,52,44,36,28,20,12, 4,
		62,54,46,38,30,22,14, 6,
	};

	for (int i = 0; i < 64; i++)
	{
		*sp = data.data64 & ((uint64_t)1 << (bit[i]));
		sp++;
	}

	data.data64 = 0;

	for (int i = 0; i < 64; i++)
	{
		data.data64 <<= 1;
		sp--;
		data.data64 |= *sp;
	}
}

void DES::Final_Permutations()
{
	bool stack[64];
	bool* sp = stack;

	const char bit[64] = {
		39, 7,47,15,55,23,63,31,
		38, 6,46,14,54,22,62,30,
		37, 5,45,13,53,21,61,29,
		36, 4,44,12,52,20,60,28,
		35, 3,43,11,51,19,59,27,
		34, 2,42,10,50,18,58,26,
		33, 1,41, 9,49,17,57,25,
		32, 0,40, 8,48,16,56,24
	};

	for (int i = 0; i < 64; i++)
	{
		*sp = data.data64 & ((uint64_t)1 << (bit[i]));
		sp++;
	}

	data.data64 = 0;

	for (int i = 0; i < 64; i++)
	{
		data.data64 <<= 1;
		sp--;
		data.data64 |= *sp;
	}
}

void DES::Expand()
{

	bool stack[48];
	bool* sp = stack;

	const char bit[48] = {
		31, 0, 1, 2, 3, 4,
		 3, 4, 5, 6, 7, 8,
		 7, 8, 9,10,11,12,
		11,12,13,14,15,16,
		15,16,17,18,19,20,
		19,20,21,22,23,24,
		23,24,25,26,27,28,
		27,28,29,30,31, 0
	};

	for (int i = 0; i < 48; i++)
	{
		*sp = data.data32[!currentOperatorSide] & ((uint64_t)1 << (bit[i]));
		sp++;
	}
	buffer = 0;

	for (int i = 0; i < 48; i++)
	{
		buffer <<= 1;
		sp--;
		buffer |= *sp;
	}
}

uint32_t DES::Substitution()
{
	uint32_t ret = data.data32[!currentOperatorSide];
	const char Sub[8][4][16] =
	{
		{
			{14, 4,13, 1, 2,15,11, 8, 3,10, 6,12, 5, 9, 0, 7},
			{ 0,15, 7, 4,14, 2,13, 1,10, 6,12,11, 9, 5, 3, 8},
			{ 4, 1,14, 8,13, 6, 2,11,15,12, 9, 7, 3,10, 5, 0},
			{15,12, 8, 2, 4, 9 ,1 ,7, 5,11 ,3,14,10, 0, 6,13}
		},
		{
			{15, 1, 8,14, 6,11, 3, 4, 9, 7, 2,13,12, 0, 5,10},
			{ 3,13, 4, 7,15, 2, 8,14,12, 0, 1,10, 6, 9,11, 5},
			{ 0,14, 7,11,10, 4,13, 1, 5, 8,12, 6, 9, 3, 2,15},
			{13, 8,10, 1, 3,15, 4, 2,11, 6, 7,12, 0, 5,14, 9}
		},
		{
			{10, 0, 9,14, 6, 3,15, 5, 1,13,12, 7,11, 4, 2, 8},
			{13, 7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
			{13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
			{1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}
		},
		{
			{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
			{13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
			{10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
			{3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}
		},
		{
			{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
			{14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
			{4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
			{11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}
		},
		{
			{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
			{10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
			{9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
			{4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}
		},
		{
			{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
			{13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
			{1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
			{6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}
		},
		{
			{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
			{1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
			{7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
			{2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
		},

	};
	data.data32[!currentOperatorSide] = 0;
	for (int i = 7; i >= 0; i--)
	{
		data.data32[!currentOperatorSide] <<= 4;

		int row = 0;
		int column = 0;

		row = (buffer & ((uint48_t)1 << (6 * i))) ? 1 : 0;
		row <<= 1;

		row |= ((buffer & ((uint48_t)1 << (6 * i + 5))) ? 1 : 0);

		for (int j = 1; j <5; j++)
		{
			column <<= 1;
			column |= ((buffer & ((uint48_t)1 << (6 * i + j))) ? 1 : 0);
		}

		data.data32[!currentOperatorSide] |= Sub[i][row][column];
	}
	return ret; 
}

void DES::PBox()
{
	bool stack[32];
	bool* sp = stack;

	const char bit[32] =
	{
		15, 6,19,20,28,11,27,16,
		 0,14,22,25, 4,17,30, 9,
		 1, 7,23,13,31,26, 2, 8,
		18,12,29, 5,21,10, 3,24
	};

	for (int i = 0; i < 32; i++)
	{
		*sp = data.data32[!currentOperatorSide] & ((uint64_t)1 << (bit[i]));
		sp++;
	}

	data.data32[!currentOperatorSide] = 0;

	for (int i = 0; i < 32; i++)
	{
		data.data32[!currentOperatorSide] <<= 1;
		sp--;
		data.data32[!currentOperatorSide] |= *sp;
	}
}

inline void DES::DoXor(uint64_t& dst,const uint64_t& b)
{
	dst ^= b;
}

inline void DES::DoXor(uint32_t& dst,const uint32_t& b)
{
	dst ^= b;
}

uint48_t DES::GetKey()
{
	bool stack[48];
	bool* sp = stack;

	uint32_t l = ((currentKey & 0xfffffff0000000) >> 28);
	uint32_t r = currentKey & 0xfffffff;

	const char shl[16] =
	{
		1,1,2,2,2,2,2,2,
		1,2,2,2,2,2,2,1
	};

	for (int i = 0; i < shl[Keycnt]; i++)
	{
		bool t = (l & 0x8000000);
		l <<= 1;
		l |= t;
		t = (r & 0x8000000);
		r <<= 1;
		r |= t;
	}
	Keycnt++;
	l &= 0xfffffff;
	r &= 0xfffffff;
	currentKey = r;
	currentKey |= ((uint64_t)l << 28);

	uint48_t ret = currentKey;

	const char pc2[48] =
	{
		24,27,20, 6,14,10, 3,22,
		 0,17, 7,12, 8,23,11, 5,
		16,26, 1, 9,19,25, 4,15,
		54,43,36,29,49,40,48,30,
		52,44,37,33,46,35,50,41,
		28,53,51,55,32,45,39,42
	};

	for (int i = 0; i < 48; i++)
	{
		*sp = ret & ((uint64_t)1 << (pc2[i]));
		sp++;
	}
	ret = 0;
	for (int i = 0; i < 48; i++)
	{
		ret <<= 1;
		sp--;
		ret |= *sp;
	}
	return ret;
}
