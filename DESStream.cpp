#include "DESStream.h"

void memcpy(char *dst,const char *src, int len)
{
	for (int i = 0; i < len; i++)
		dst[i] = src[i];
}

void memset(char *dst, char ch, int len)
{
	for (int i = 0; i < len; i++)
		dst[i] = ch;
}

DESStream::DESStream(uint64_t key1, uint64_t key2) : key1(key1), key2(key2), currentkey(false)
{
}

DESStream::~DESStream()
{
}

DESStream &DESStream::operator<<(std::string str)
{
	this->append(str);
	return *this;
}

DESStream &DESStream::operator>>(std::string &str)
{
	str = *this;
	return *this;
}

DESStream &DESStream::operator>>(const DES::Mode &mode)
{
	uint64_t key;
	if (key2)
	{
		if (currentkey)
			key = key1;
		else
			key = key2;
		currentkey = !currentkey;
	}

	int len = length() / 8 + !!(length() % 8);

	char *buf = new char[len * 8];
	memcpy(buf, this->c_str(), length());
	memset(&buf[length()], -1, len * 8 - length());

	uint64_t *buf64 = (uint64_t *)buf;

	switch (mode)
	{
	case DES::encrypt:
		for (int i = 0; i < len; i++)
			buf64[i] = DES(buf64[i], key1).Encrypt();
		break;
	case DES::decrypt:
		for (int i = 0; i < len; i++)
			buf64[i] = DES(buf64[i], key1).Decrypt();
		break;
	default:
		break;
	}
	this->assign(buf, len * 8);
	return *this;
}
