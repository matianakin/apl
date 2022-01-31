//
// Created by matia on 25.01.2022.
//

#ifndef CPP_THREADHEAD_H
#define CPP_THREADHEAD_H
#include "ListOfCities.h"


class ThreadHead {
    public:
    ThreadHead *next;
    ThreadHead *prev;
    ListOfCities *subHead;
    ListOfCities *subEnd;

    ThreadHead();

    ThreadHead(ListOfCities *subHead, ListOfCities *subEnd);

    ThreadHead(ThreadHead* curHead, ListOfCities *subHead, ListOfCities *subEnd);

    //public: void overrider(ListOfCities *subHead, ListOfCities *subEnd);
};


#endif //CPP_THREADHEAD_H
