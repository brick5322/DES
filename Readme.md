# DES对称加密

DES CPP实现

两行代码

一行加密，一行解密。

```c++
DES(data,key).Encrypt();
DES(cryptograph,key).Decrypt();
```

也可以双密钥流式加解密

```C++
	DESStream encrypt(key1,key2);
	std::string buffer;
	encrypt << std::string("This is a triple DES experiment.\n");
	encrypt >> DES::encrypt >> DES::decrypt >> DES::encrypt>> buffer;
	//key2加密，key1解密，key2加密
	encrypt >> DES::decrypt >> DES::encrypt >> DES::decrypt >> buffer;
	//key1解密，key2加密，key1解密
```

静态库和动态库生成

```bash
make a
#生成.a
make so
#生成.so
```

