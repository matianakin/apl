#ifndef CPP_MYDLL_H
#define CPP_MYDLL_H
#include "ListOfConnections.h"
#include "ListOfNodes.h"
#include "ThreadHead.h"

extern "C" __declspec(dllexport) void importFromFile(char* filename);

extern "C" __declspec(dllexport) void alphabet();

extern "C" __declspec(dllexport) void linearVersion(char* start);

extern "C" __declspec(dllexport) void parallelVersionVer1(char* start, int nth);

extern "C" __declspec(dllexport) void parallelVersionVer2(char* start, int nth);

extern "C" __declspec(dllexport) void parallelVersionVer3(char* start, int nth);

extern "C" __declspec(dllexport) void readListOfCities(char* buffer);

#endif