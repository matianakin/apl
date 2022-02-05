#include <iostream>
#include "stdio.h"
#include <fstream>
using namespace std;

#ifdef _cplusplus
extern "C" {
#endif
#ifdef BUILD_MY_DLL
#define MyFunctions _declspec(dllexport)
#else
#define MyFunctions _declspec(dllimport)
#endif
	MyFunctions void TestFile();
#ifdef _cplusplus
}
#endif
void TestFile()
{
	ofstream myfile;
	myfile.open("example.txt");
	myfile << "Writing this to a file.\n";
	myfile.close();
}