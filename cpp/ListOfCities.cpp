//
// Created by matia on 24.01.2022.
//
#include <iostream>
#include <utility>
#include "ListOfCities.h"

ListOfCities::ListOfCities()
{
    next = nullptr;
    prev = nullptr;
}

ListOfCities::ListOfCities(std::string name, int distance)
{
    next = nullptr;
    prev = nullptr;
    this->city=std::move(name);
    this->distance=distance;
}

ListOfCities::ListOfCities(std::string name, int distance, ListOfCities* current)
{
    next = nullptr;
    prev = current;
    current->next= this;
    this->city=std::move(name);
    this->distance=distance;
}

ListOfCities::ListOfCities(std::string city)
{
    next = nullptr;
    prev = nullptr;
    this->city=std::move(city);
    this->distance=0;
}

ListOfCities::ListOfCities(std::string city, ListOfCities* current)
{
    next = nullptr;
    prev = current;
    current->next= this;
    this->city=std::move(city);
    this->distance=0;
}