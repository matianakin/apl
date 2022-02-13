#include <iostream>
#ifndef CPP_LISTOFNODES_H
#define CPP_LISTOFNODES_H


class ListOfNodes {
public:
    ListOfNodes* next;
    ListOfNodes* prev;
    std::string node;
    std::string prevNode;
    int distance;
    bool visited;

    ListOfNodes();

    ListOfNodes(std::string node, int distance);

    ListOfNodes(std::string node);

    ListOfNodes(std::string node, ListOfNodes* current);

    ListOfNodes(std::string node, int distance, ListOfNodes* current);
};


#endif //CPP_LISTOFNODES_H