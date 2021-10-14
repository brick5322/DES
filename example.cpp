#include <cstdio>
#include "DES.h"

int main()
{
	uint64_t key = 0x133457799BBCDFF1;
	uint64_t data = 0x0123456789ABCDEF;
	uint64_t cryptograph;
	
	printf("0x%I64x\n", data);
	printf("0x%I64x\n", cryptograph = DES(data		 ,key).Encrypt());
	printf("0x%I64x\n", data		= DES(cryptograph,key).Decrypt());

}