#pragma once
#include "DES.h"

#define uint64_t temp_uin64_t
#include <string>
#undef uint64_t 


class DESStream :public std::string
{
private:
	uint64_t key1;
	uint64_t key2;
	bool currentkey;
public:
	DESStream(uint64_t,uint64_t = 0);
	~DESStream();
	DESStream& operator<<(std::string);
	DESStream& operator>>(std::string&);
	DESStream& operator>>(const DES::Mode&);
};

