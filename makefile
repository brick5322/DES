example	  : example.o DES_x64.o DESStream_x64.o
		g++ -o example example.o DES_x64.o DESStream_x64.o
a		  : libDES_x64.a	libDES_x86.a
so		  : libDES_x64.so	libDES_x86.so
libDES_x64.so : DESStream.cpp DES.cpp
		g++ DESStream.cpp DES.cpp -fPIC -shared -o libDES_x64.so
libDES_x86.so : DESStream.cpp DES.cpp
		g++ DESStream.cpp DES.cpp -m32 -fPIC -shared -o libDES_x86.so
libDES_x64.a  : DES_x64.o DESStream_x64.o
		ar cr libDES_x64.a DES_x64.o DESStream_x64.o
libDES_x86.a  : DES_x86.o DESStream_x86.o
		ar cr libDES_x86.a DES_x86.o DESStream_x86.o
example.o : example.cpp DESStream.h
		g++ -c example.cpp
DES_x64.o 	  :	DES.cpp
		g++ -c DES.cpp -o DES_x64.o 
DES_x86.o 	  :	DES.cpp
		g++ -c DES.cpp -o DES_x86.o 
DESStream_x64.o :DESStream.cpp DES.h
		g++ -c DESStream.cpp -o DESStream_x64.o
DESStream_x86.o :DESStream.cpp DES.h
		g++ -c -m32 DESStream.cpp -o DESStream_x86.o 
cleanObj :
	rm *.o
clean		:
	rm *.a *.so *.o example
