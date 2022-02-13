#ifndef CPP_LISTOFCONNECTIONS_H
#define CPP_LISTOFCONNECTIONS_H
#include <iostream>

class ListOfConnections {
public:
    ListOfConnections* next;
    ListOfConnections* prev;
    std::string node1;
    std::string node2;
    int distance;

    ListOfConnections();

    ListOfConnections(std::string node1, std::string node2, int distance);

    ListOfConnections(std::string node1, std::string node2, int distance, ListOfConnections* current);
};

#endif //CPP_LISTOFCONNECTIONS_H