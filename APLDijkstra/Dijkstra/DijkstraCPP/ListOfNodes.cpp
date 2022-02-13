#include "pch.h"
#include <iostream>
#include <utility>
#include "ListOfNodes.h"

ListOfNodes::ListOfNodes()
{
    next = nullptr;
    prev = nullptr;
    visited = false;
    prevNode = "";
}

ListOfNodes::ListOfNodes(std::string name, int distance)
{
    next = nullptr;
    prev = nullptr;
    this->node = std::move(name);
    this->distance = distance;
    visited = false;
    prevNode = "";
}

ListOfNodes::ListOfNodes(std::string name, int distance, ListOfNodes* current)
{
    next = nullptr;
    prev = current;
    current->next = this;
    this->node = std::move(name);
    this->distance = distance;
    visited = false;
    prevNode = "";
}

ListOfNodes::ListOfNodes(std::string node)
{
    next = nullptr;
    prev = nullptr;
    this->node = std::move(node);
    this->distance = INT32_MAX;
    visited = false;
    prevNode = "";
}

ListOfNodes::ListOfNodes(std::string node, ListOfNodes* current)
{
    next = nullptr;
    prev = current;
    current->next = this;
    this->node = std::move(node);
    this->distance = INT32_MAX;
    visited = false;
    prevNode = "";
}