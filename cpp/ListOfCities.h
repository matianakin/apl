//
// Created by matia on 24.01.2022.
//
#include <iostream>
#ifndef CPP_LISTOFCITIES_H
#define CPP_LISTOFCITIES_H


class ListOfCities {
    public: ListOfCities* next;
    public: ListOfCities* prev;
    public: std::string city;
    public: std::string prevCity;
    public: int distance;
    public: bool visited;

    ListOfCities();

    ListOfCities(std::string city, int distance);

    ListOfCities(std::string city);

    ListOfCities(std::string city, ListOfCities* current);

    ListOfCities(std::string city, int distance, ListOfCities* current);
};


#endif //CPP_LISTOFCITIES_H
