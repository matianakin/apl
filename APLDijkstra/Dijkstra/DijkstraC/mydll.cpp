#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "stdbool.h"
#include "time.h"
#include "mydll.h"
#include <chrono>
#include <string>

struct ListOfNodes {
    struct ListOfNodes* next;
    struct ListOfNodes* prev;
    char node[256];
    char prevNode[256];
    int distance;
    bool visited;
};

struct ListOfConnections {
    struct ListOfConnections* next;
    struct ListOfConnections* prev;
    char node1[256];
    char node2[256];
    int distance;
};

//including inLoop function from Assembly
extern "C" void inLoop(struct ListOfNodes** headNode, struct ListOfNodes* current, struct ListOfConnections* connectionIterator, char* nameNode);

//including linearVersion (the main algorith) function from Assembly
extern "C" void linearVersion(struct ListOfNodes** headNode, struct ListOfConnections** headConnection, char* start);

//function sorting the list of nodes in alphabetical order
void alphabet(struct ListOfNodes** headNode)
{
    struct ListOfNodes* tempNode = *headNode;
    struct ListOfNodes* tempNode2 = tempNode;
    int i = 0;
    while (tempNode2)
    {
        i++;
        tempNode2 = tempNode2->next;
    }
    for (int j = 0; j < i; j++) {
        while (tempNode && tempNode->next) {
            if (strcmp(tempNode->node, tempNode->next->node) > 0) {
                struct ListOfNodes* tempNodeNext = tempNode->next;
                struct ListOfNodes* tempNodeNextNext = tempNodeNext->next;
                struct ListOfNodes* tempNodePrev = tempNode->prev;
                if (tempNodeNextNext) {
                    tempNodeNextNext->prev = tempNode;
                }
                tempNodeNext->next = tempNode;
                tempNode->prev = tempNodeNext;
                tempNode->next = tempNodeNextNext;
                tempNodeNext->prev = tempNodePrev;
                if (tempNodePrev) {
                    tempNodePrev->next = tempNodeNext;
                }
            }
            tempNode = tempNode->next;
        }
        while ((*headNode)->prev)
        {
            *headNode = (*headNode)->prev;
        }
        tempNode = *headNode;
    }
}

//function reading list of nodes to the buffer
void readListOfNodes(struct ListOfNodes** headNode, char* buffer)
{
    struct ListOfNodes* tempNode = *headNode;
    int offset = 0;
    while (tempNode)
    {
        char tmp[32];
        sprintf(buffer + offset, "Node: %s, Distance from source: %i, Previous node: %s\n", tempNode->node, tempNode->distance, tempNode->prevNode);
        _itoa(tempNode->distance, tmp, 10);
        offset += strlen(tempNode->node) + strlen(tmp) + strlen(tempNode->prevNode) + 48;
        tempNode = tempNode->next;
    }
}

//function checking the list of nodes for duplicates
void checkForDuplicateNodes(struct ListOfNodes** headNode)
{
    struct ListOfNodes* temp = *headNode;
    struct ListOfNodes* temp2 = NULL;
    if (*headNode) {
        temp2 = (*headNode)->next;
    }
    while (temp)
    {
        bool changed = false;
        while (temp2) {

            if (strcmp(temp->node, temp2->node) == 0) {
                struct ListOfNodes* temp3 = temp2;
                temp2 = temp3->next;
                if (temp2) {
                    temp2->prev = temp3->prev;
                }
                temp3->prev->next = temp2;
                free(temp3);
                changed = true;
            }
            if (!changed)
            {
                temp2 = temp2->next;
            }
            else
            {
                changed = false;
            }
        }
        temp = temp->next;
        if (temp)
            temp2 = temp->next;
    }
}

//function checking the list of connections for duplicates
void checkForDuplicateConnections(struct ListOfConnections** headConnection)
{
    struct ListOfConnections* temp = *headConnection;
    struct ListOfConnections* temp2 = NULL;
    if (*headConnection) {
        temp2 = (*headConnection)->next;
    }
    while (temp)
    {
        bool changed = false;
        while (temp2) {

            if ((temp->node1 == temp2->node1 && temp->node2 == temp2->node2) || (temp->node1 == temp2->node2 && temp->node2 == temp2->node1)) {
                struct ListOfConnections* temp3 = temp2;
                temp2 = temp3->next;
                temp3->prev->next = temp2;
                if (temp2) {
                    temp2->prev = temp3->prev;
                }
                free(temp3);
                changed = true;
            }
            if (!changed)
            {
                temp2 = temp2->next;
            }
            else
            {
                changed = false;
            }
        }
        temp = temp->next;
        if (temp)
            temp2 = temp->next;
    }
}

//void inLoop(struct ListOfNodes** headNode, struct ListOfNodes* current, struct ListOfConnections* connectionIterator, char* nameNode)
//{
//    struct ListOfNodes* analyzedNode = *headNode;
//    int road = current->distance + connectionIterator->distance;
//    while (analyzedNode) {
//        if (strcmp(analyzedNode->node, nameNode) == 0) {
//            break;
//        }
//        analyzedNode = analyzedNode->next;
//    }
//    if (!analyzedNode->visited && analyzedNode->distance > road) {
//        strcpy(analyzedNode->prevNode, current->node);
//        analyzedNode->distance = road;
//    }
//}

//void linearVersion(struct ListOfNodes** headNode, struct ListOfConnections** headConnection, char* start)
//{
//
//    struct ListOfNodes* startNode = *headNode;
//    bool finished = false;
//    while (startNode)
//    {
//        if (strcmp(startNode->node, start) == 0)
//        {
//            startNode->distance = 0;
//            startNode->visited = true;
//            strcpy(startNode->prevNode, "<---  This is the starting vector");
//            break;
//        }
//        startNode = startNode->next;
//        if (!startNode)
//        {
//            printf("%s", "Incorrect node provided as a start point\n");
//            exit(1);
//        }
//    }25
//
//    struct ListOfNodes* current = startNode;
//
//    do {
//        finished = true;
//        struct ListOfConnections* connectionIterator = *headConnection;
//        while (connectionIterator) {
//            if (strcmp(connectionIterator->node1, current->node) == 0) {
//                inLoop(headNode, current, connectionIterator, connectionIterator->node2);
//            }
//            else if (strcmp(connectionIterator->node2, current->node) == 0)
//            {
//                inLoop(headNode, current, connectionIterator, connectionIterator->node1);
//            }
//            connectionIterator = connectionIterator->next;
//        }
//
//        int tempDist = INT_MAX;
//        struct ListOfNodes* tempNode = *headNode;
//        while (tempNode)
//        {
//            if (!tempNode->visited && tempNode->distance < tempDist) {
//                current = tempNode;
//                tempDist = tempNode->distance;
//                finished = false;
//            }
//            tempNode = tempNode->next;
//        }
//        current->visited = true;
//
//    } while (!finished);
//}

//function importing from file and strting entire algorithm, including the Assembly section
void importFromFile(char* filename, char* start, char* buffer, char*time)
{
    struct ListOfNodes* headNode = NULL;
    struct ListOfConnections* headConnection = NULL;
    FILE* file = fopen(filename, "r");
    if (file)
    {
        char tempnode1[155];
        char tempnode2[155];
        int tempdistance = 0;
        int j = 0;
        while (fscanf(file, "%s %s %i", tempnode1, tempnode2, &tempdistance) > 0) // NOLINT(cert-err34-c)
        {
            if (j == 0) {
                struct ListOfNodes* node1 = (ListOfNodes*)malloc(sizeof(struct ListOfNodes));
                node1->next = NULL;
                node1->prev = NULL;
                strcpy(node1->node, tempnode1);
                node1->distance = INT_MAX;
                node1->visited = false;
                strcpy(node1->prevNode, "");
                headNode = node1;
                struct ListOfConnections* con1 = (ListOfConnections*)malloc(sizeof(struct ListOfConnections));
                con1->next = NULL;
                con1->prev = NULL;
                strcpy(con1->node1, tempnode1);
                strcpy(con1->node2, tempnode2);
                con1->distance = tempdistance;
                headConnection = con1;
                struct ListOfNodes* tempNode = headNode;
                while (tempNode->next) {
                    tempNode = tempNode->next;
                }
                struct ListOfNodes* node2 = (ListOfNodes*)malloc(sizeof(struct ListOfNodes));
                node2->next = NULL;
                node2->prev = tempNode;
                tempNode->next = node2;
                strcpy(node2->node, tempnode2);
                node2->distance = INT_MAX;
                node2->visited = false;
                strcpy(node2->prevNode, "");
            }
            else {
                struct ListOfNodes* tempNode = headNode;
                while (tempNode->next) {
                    tempNode = tempNode->next;
                }
                struct ListOfNodes* node1 = (ListOfNodes*)malloc(sizeof(struct ListOfNodes));
                struct ListOfNodes* node2 = (ListOfNodes*)malloc(sizeof(struct ListOfNodes));
                struct ListOfConnections* con1 = (ListOfConnections*)malloc(sizeof(struct ListOfConnections));
                node1->prev = tempNode;
                tempNode->next = node1;
                strcpy(node1->node, tempnode1);
                node1->distance = INT_MAX;
                node1->visited = false;
                strcpy(node1->prevNode, "");
                node2->next = NULL;
                node2->prev = node1;
                strcpy(node2->node, tempnode2);
                node2->distance = INT_MAX;
                node2->visited = false;
                strcpy(node2->prevNode, "");
                node1->next = node2;
                struct ListOfConnections* tempCon = headConnection;
                while (tempCon->next) {
                    tempCon = tempCon->next;
                }
                con1->next = NULL;
                con1->prev = tempCon;
                tempCon->next = con1;
                strcpy(con1->node1, tempnode1);
                strcpy(con1->node2, tempnode2);
                con1->distance = tempdistance;
            }
            j++;


        }
        checkForDuplicateNodes(&headNode);
        checkForDuplicateConnections(&headConnection);
    }
    else
    {
        fprintf(stderr, "Error opening file '%s'\n", filename);
        exit(1);
    }
    fclose(file);

    alphabet(&headNode);
    auto begin = std::chrono::steady_clock::now();
    linearVersion(&headNode, &headConnection, start);
    auto end = std::chrono::steady_clock::now();
    auto time_span = static_cast<std::chrono::duration<double>>(end - begin);   // measure time span between start & end
    std::string s = std::to_string(time_span.count()) + " s";
    strcpy(time, s.c_str());
    readListOfNodes(&headNode, buffer);
}
