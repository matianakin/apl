//
// Created by matia on 24.01.2022.
//

#include "ListOfConnections.h"

ListOfConnections::ListOfConnections()
{
    next = nullptr;
    prev = nullptr;
}

ListOfConnections::ListOfConnections(std::string city1, std::string city2, int distance)
{
    next = nullptr;
    prev = nullptr;
    this->city1 = std::move(city1);
    this->city2 = std::move(city2);
    this->distance = distance;
}

ListOfConnections::ListOfConnections(std::string city1, std::string city2, int distance, ListOfConnections* current)
{
    next = nullptr;
    prev = current;
    current->next = this;
    this->city1 = std::move(city1);
    this->city2 = std::move(city2);
    this->distance = distance;
}