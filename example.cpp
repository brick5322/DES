#include "DESStream.h"

#include <iostream>


#if 0
int main()
{
	uint64_t key = 0x133457799BBCDFF1;
	uint64_t data = 0x0123456789ABCDEF;
	uint64_t cryptograph;


	printf("0x%I64x\n", data);
	printf("0x%I64x\n", cryptograph = DES(data		 ,key).Encrypt());
	printf("0x%I64x\n", data		= DES(cryptograph,key).Decrypt());

}
#else
int main()
{
	const uint64_t key1 = 0x0123456789ABCDEF;
	const uint64_t key2 = 0x326487628342431;

	DESStream encrypt(key1,key2);
	std::string buffer;

	encrypt << std::string("This is a triple DES experiment.\n");

	encrypt >> DES::encrypt >> DES::decrypt >> DES::encrypt>> buffer;

	std::cout << "Encrypted:\n" << buffer.c_str() << "\n\n\n\n";

	encrypt >> DES::decrypt >> DES::encrypt >> DES::decrypt >> buffer;

	std::cout << buffer;
}
#endif
