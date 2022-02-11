//
// Created by matia on 25.01.2022.
//

#ifndef CPP_THREADHEAD_H
#define CPP_THREADHEAD_H
#include "ListOfNodes.h"


class ThreadHead {
    public:
    ThreadHead *next;
    ThreadHead *prev;
    ListOfNodes *subHead;
    ListOfNodes *subEnd;

    ThreadHead();

    ThreadHead(ListOfNodes *subHead, ListOfNodes *subEnd);

    ThreadHead(ThreadHead* curHead, ListOfNodes *subHead, ListOfNodes *subEnd);

    //public: void overrider(ListOfNodes *subHead, ListOfNodes *subEnd);
};


#endif //CPP_THREADHEAD_H
