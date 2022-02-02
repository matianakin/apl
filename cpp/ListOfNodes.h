//
// Created by matia on 24.01.2022.
//
#include <iostream>
#ifndef CPP_LISTOFCITIES_H
#define CPP_LISTOFCITIES_H


class ListOfNodes {
    public:
    ListOfNodes* next;
    ListOfNodes* prev;
    std::string city;
    std::string prevCity;
    int distance;
    bool visited;

    ListOfNodes();

    ListOfNodes(std::string city, int distance);

    ListOfNodes(std::string city);

    ListOfNodes(std::string city, ListOfNodes* current);

    ListOfNodes(std::string city, int distance, ListOfNodes* current);
};


#endif //CPP_LISTOFCITIES_H
