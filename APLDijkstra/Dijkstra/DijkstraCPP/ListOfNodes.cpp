#include "pch.h"
#include <iostream>
#include <utility>
#include "ListOfNodes.h"

ListOfNodes::ListOfNodes()
{
    next = nullptr;
    prev = nullptr;
    visited = false;
    prevCity = "";
}

ListOfNodes::ListOfNodes(std::string name, int distance)
{
    next = nullptr;
    prev = nullptr;
    this->city = std::move(name);
    this->distance = distance;
    visited = false;
    prevCity = "";
}

ListOfNodes::ListOfNodes(std::string name, int distance, ListOfNodes* current)
{
    next = nullptr;
    prev = current;
    current->next = this;
    this->city = std::move(name);
    this->distance = distance;
    visited = false;
    prevCity = "";
}

ListOfNodes::ListOfNodes(std::string city)
{
    next = nullptr;
    prev = nullptr;
    this->city = std::move(city);
    this->distance = INT32_MAX;
    visited = false;
    prevCity = "";
}

ListOfNodes::ListOfNodes(std::string city, ListOfNodes* current)
{
    next = nullptr;
    prev = current;
    current->next = this;
    this->city = std::move(city);
    this->distance = INT32_MAX;
    visited = false;
    prevCity = "";
}