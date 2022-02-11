#include "pch.h"
#include "ThreadHead.h"
#include "ListOfNodes.h"

ThreadHead::ThreadHead()
{
    this->next = nullptr;
    this->prev = nullptr;
    this->subHead = nullptr;
    this->subEnd = nullptr;
}

ThreadHead::ThreadHead(ListOfNodes* subHead, ListOfNodes* subEnd)
{
    this->next = nullptr;
    this->prev = nullptr;
    this->subHead = subHead;
    this->subEnd = subEnd;
}

ThreadHead::ThreadHead(ThreadHead* curHead, ListOfNodes* subHead, ListOfNodes* subEnd)
{
    this->next = curHead;
    this->prev = nullptr;
    this->subHead = subHead;
    this->subEnd = subEnd;
    curHead->prev = this;
}