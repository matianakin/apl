g++ -c -DBUILD_MY_DLL MyFunctions.cpp
g++ -shared -o MyFunctions.dll MyFunctions.o -Wl,-out-implib,MyFunctions.a