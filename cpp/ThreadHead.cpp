//
// Created by matia on 25.01.2022.
//

#include "ThreadHead.h"
#include "ListOfCities.h"

ThreadHead::ThreadHead()
{
    this->next=nullptr;
    this->prev=nullptr;
    this->subHead=nullptr;
    this->subEnd=nullptr;
}

ThreadHead::ThreadHead(ListOfCities *subHead, ListOfCities *subEnd)
{
    this->next=nullptr;
    this->prev=nullptr;
    this->subHead=subHead;
    this->subEnd=subEnd;
}

ThreadHead::ThreadHead(ThreadHead* curHead, ListOfCities *subHead, ListOfCities *subEnd)
{
    this->next=curHead;
    this->prev=nullptr;
    this->subHead=subHead;
    this->subEnd=subEnd;
    curHead->prev=this;
}

void ThreadHead::overrider(ListOfCities *subHead, ListOfCities *subEnd)
{
    this->subHead=subHead;
    this->subEnd=subEnd;
}