#include "pch.h"
#include "ListOfConnections.h"

ListOfConnections::ListOfConnections()
{
    next = nullptr;
    prev = nullptr;
}

ListOfConnections::ListOfConnections(std::string node1, std::string node2, int distance)
{
    next = nullptr;
    prev = nullptr;
    this->node1 = std::move(node1);
    this->node2 = std::move(node2);
    this->distance = distance;
}

ListOfConnections::ListOfConnections(std::string node1, std::string node2, int distance, ListOfConnections* current)
{
    next = nullptr;
    prev = current;
    current->next = this;
    this->node1 = std::move(node1);
    this->node2 = std::move(node2);
    this->distance = distance;
}