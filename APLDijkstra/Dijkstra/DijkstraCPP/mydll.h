#ifndef CPP_MYDLL_H
#define CPP_MYDLL_H
#include "ListOfConnections.h"
#include "ListOfNodes.h"
#include "ThreadHead.h"

extern "C" __declspec(dllexport) void importFromFile(char* filename);

extern "C" __declspec(dllexport) void alphabet();

extern "C" __declspec(dllexport) void parallelVersionVer2(char* start, int nth);

extern "C" __declspec(dllexport) void readListOfCities(char* buffer);

extern "C" __declspec(dllexport) bool nodeCheck(char* name);

extern "C" __declspec(dllexport) bool checkNoOfThreads(int threadCount);

#endif