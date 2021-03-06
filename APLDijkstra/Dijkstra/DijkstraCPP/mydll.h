#ifndef CPP_MYDLL_H
#define CPP_MYDLL_H
#include "ListOfConnections.h"
#include "ListOfNodes.h"
#include "ThreadHead.h"

extern "C" __declspec(dllexport) void importFromFile(char* filename);

extern "C" __declspec(dllexport) bool nodeCheck(char* name);

extern "C" __declspec(dllexport) bool checkNoOfThreads(int threadCount);

extern "C" __declspec(dllexport) void callCPP(char* filename, char* start, int nth, char* buffer, char* time);

#endif