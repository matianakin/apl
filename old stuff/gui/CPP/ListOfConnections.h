//
// Created by matia on 24.01.2022.
//

#ifndef CPP_LISTOFCONNECTIONS_H
#define CPP_LISTOFCONNECTIONS_H
#include <iostream>

class ListOfConnections {
public:
    ListOfConnections* next;
    ListOfConnections* prev;
    std::string city1;
    std::string city2;
    int distance;

    ListOfConnections();

    ListOfConnections(std::string city1, std::string city2, int distance);

    ListOfConnections(std::string city1, std::string city2, int distance, ListOfConnections* current);
};


#endif //CPP_LISTOFCONNECTIONS_H
