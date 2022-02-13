#include "pch.h"
#include <iostream>
#include "ListOfNodes.h"
#include "ListOfConnections.h"
#include "ThreadHead.h"
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <cstring>
#include "mydll.h"



ListOfNodes* headNode = nullptr;
ListOfConnections* headConnection = nullptr;
ThreadHead* tHead = nullptr;
std::mutex m;
ListOfNodes* current = nullptr;

void checkForDuplicateNodes()
{
    auto temp = headNode;
    ListOfNodes* temp2 = nullptr;
    if (headNode)
    {
        temp2 = headNode->next;
    }
    while (temp)
    {
        bool changed = false;
        while (temp2) {

            if (temp->node == temp2->node) {
                auto temp3 = temp2;
                temp2 = temp3->next;
                if (temp2) {
                    temp2->prev = temp3->prev;
                }
                temp3->prev->next = temp2;
                delete temp3;
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

void checkForDuplicateConnections()
{
    auto temp = headConnection;
    ListOfConnections* temp2 = nullptr;
    if (headConnection)
    {
        temp2 = headConnection->next;
    }
    while (temp)
    {
        bool changed = false;
        while (temp2) {

            if ((temp->node1 == temp2->node1 && temp->node2 == temp2->node2) || (temp->node1 == temp2->node2 && temp->node2 == temp2->node1)) {
                auto temp3 = temp2;
                temp2 = temp3->next;
                temp3->prev->next = temp2;
                if (temp2) {
                    temp2->prev = temp3->prev;
                }
                delete temp3;
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

void importFromFile(char* filename)
{
    std::fstream file(filename);
    if (file.is_open())
    {
        std::string temp;
        std::string tempnode1;
        std::string tempnode2;
        int tempdistance;
        int j = 0;
        int i = 0;
        while (file >> temp)
        {
            switch (i) {
            case 0:
            {
                tempnode1 = temp;
                break;
            }
            case 1:
            {
                tempnode2 = temp;
                break;
            }
            case 2:
            {
                tempdistance = atoi(temp.c_str());
                break;
            }
            }
            if (i == 2) {
                if (j == 0) {
                    auto node1 = new ListOfNodes(tempnode1);
                    delete headNode;
                    headNode = node1;
                    auto con1 = new ListOfConnections(tempnode1, tempnode2, tempdistance);
                    delete headConnection;
                    headConnection = con1;
                    auto tempNode = headNode;
                    while (tempNode->next) {
                        tempNode = tempNode->next;
                    }
                    auto* node2 = new ListOfNodes(tempnode2, tempNode);
                }
                else {
                    auto tempNode = headNode;
                    while (tempNode->next) {
                        tempNode = tempNode->next;
                    }
                    auto node1 = new ListOfNodes(tempnode1, tempNode);
                    auto node2 = new ListOfNodes(tempnode2, tempNode->next);
                    auto tempCon = headConnection;
                    while (tempCon->next) {
                        tempCon = tempCon->next;
                    }
                    auto con1 = new ListOfConnections(tempnode1, tempnode2, tempdistance, tempCon);
                }
                i = 0;
                j++;
            }
            else
            {
                i++;
            }
        }
        checkForDuplicateNodes();
        checkForDuplicateConnections();
    }
    file.close();
}

void readListOfNodes(char* buffer)
{
    //auto tempNode = headNode;
    //while (tempNode)
    //{
    //    std::cout << "node: " + tempNode->node << " Distance from source: " << tempNode->distance << " Previous node: " << tempNode->prevNode << std::endl;
    //    tempNode = tempNode->next;
    //}
    auto tempNode = headNode;
    int offset = 0;
    while (tempNode)
    {
        char tmp[32];
        sprintf(buffer + offset, "Node: %s, Distance from source: %i, Previous node: %s\n", tempNode->node.c_str(), tempNode->distance, tempNode->prevNode.c_str());
        _itoa(tempNode->distance, tmp, 10);
        offset += strlen(tempNode->node.c_str()) + strlen(tmp) + strlen(tempNode->prevNode.c_str()) + 48;
        tempNode = tempNode->next;
    }
}

void readListofConnections()
{
    auto tempConnection = headConnection;
    while (tempConnection)
    {
        std::cout << "node1: " + tempConnection->node1 << "node1: " + tempConnection->node1 << " Distance  " << tempConnection->distance << std::endl;
        tempConnection = tempConnection->next;
    }
}

void prepareParallelization(int threadCount)
{
    if (threadCount < 1)
    {
        std::cout << "Minimal number of threads is 1";
        exit(1);
    }
    auto tempNode = headNode;
    int i = 0;
    while (tempNode)
    {
        i++;
        tempNode = tempNode->next;
    }
    tempNode = headNode;
    if (threadCount > i)
    {
        std::cout << "Too many threads requested. Number of threads cannot exceed number of vertices." << std::endl;
        exit(1);
    }
    int vertPerThread = i / threadCount;
    int q = 0;
    bool created = false;
    auto tempNodeHead = headNode;
    auto tempNodeTail = headNode;
    while (tempNode)
    {
        if (q == vertPerThread - 1)
        {
            tempNodeTail = tempNode;
            if (!created)
            {
                created = true;
                tHead = new ThreadHead(tempNodeHead, tempNodeTail);
                if (tempNode->next)
                {
                    tempNodeHead = tempNode->next;
                }
            }
            else
            {
                auto tempThreader = new ThreadHead(tHead, tempNodeHead, tempNodeTail);
                tHead = tempThreader;
                if (tempNode->next)
                {
                    tempNodeHead = tempNode->next;
                }
            }
            q = -1;
        }
        q++;
        tempNode = tempNode->next;
    }
    if (vertPerThread * threadCount != i)
    {
        tempNodeTail = tempNode;
        auto tempThreader = new ThreadHead(tHead, tempNodeHead, tempNodeTail);
        tHead = tempThreader;
    }
}

void oneThreadVer2(ThreadHead* th, int& tempDist, bool& finished)
{
    auto tempNode = th->subHead;
    bool broken = false;
    while (tempNode) {
        if (th->subEnd && tempNode == th->subEnd->next)
        {
            break;
        }
        if (!tempNode->visited && tempNode->distance < tempDist) {
            m.lock();
            current = tempNode;
            tempDist = tempNode->distance;
            finished = false;
            m.unlock();
        }
        tempNode = tempNode->next;
    }
}

void parallelVersionVer2(char* start, int nth)
{
    auto startNode = headNode;
    bool finished = false;
    while (startNode)
    {
        if (startNode->node == start)
        {
            startNode->distance = 0;
            startNode->visited = true;
            startNode->prevNode = "STARTING NODE";
            break;
        }
        startNode = startNode->next;
        if (!startNode)
        {
            std::cout << "Incorrect node provided as a start point" << std::endl;
            exit(1);
        }
    }
    current = startNode;
    int noOfThreads = nth;

    prepareParallelization(noOfThreads);

    do {
        finished = true;
        auto connectionIterator = headConnection;
        while (connectionIterator) {
            if (connectionIterator->node1 == current->node) {
                auto analyzedNode = headNode;
                int road = current->distance + connectionIterator->distance;
                while (analyzedNode) {
                    if (analyzedNode->node == connectionIterator->node2) {
                        break;
                    }
                    analyzedNode = analyzedNode->next;
                }
                if (!analyzedNode->visited && analyzedNode->distance > road) {
                    analyzedNode->prevNode = current->node;
                    //changeDistance(analyzedNode, road);
                    analyzedNode->distance = road;
                }
            }
            else if (connectionIterator->node2 == current->node)
            {
                auto analyzedNode = headNode;
                int road = current->distance + connectionIterator->distance;
                while (analyzedNode) {
                    if (analyzedNode->node == connectionIterator->node1) {
                        break;
                    }
                    analyzedNode = analyzedNode->next;
                }
                if (!analyzedNode->visited && analyzedNode->distance > road) {
                    analyzedNode->prevNode = current->node;
                    //changeDistance(analyzedNode, road);
                    analyzedNode->distance = road;
                }
            }
            connectionIterator = connectionIterator->next;
        }

        int tempDist = INT32_MAX;
        auto tempT = tHead;
        std::vector<std::thread*> ThreadVector;
        while (tempT)
        {
            auto* myThread = new std::thread(oneThreadVer2, tempT, std::ref(tempDist), std::ref(finished));
            ThreadVector.push_back(myThread);
            tempT = tempT->next;
        }
        for (auto& i : ThreadVector)
        {
            i->join();
        }
        current->visited = true;

    } while (!finished);

}

void alphabet()
{
    auto tempNode = headNode;
    auto tempNode2 = tempNode;
    int i = 0;
    while (tempNode2)
    {
        i++;
        tempNode2 = tempNode2->next;
    }
    for (int j = 0; j < i; j++) {
        while (tempNode && tempNode->next) {
            if (tempNode->node > tempNode->next->node) {
                auto tempNodeNext = tempNode->next;
                auto tempNodeNextNext = tempNodeNext->next;
                auto tempNodePrev = tempNode->prev;
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
        while (headNode->prev)
        {
            headNode = headNode->prev;
        }
        tempNode = headNode;
    }
}

bool nodeCheck(char* name)
{
    auto tempNode = headNode;
    bool exists = false;
    while (tempNode)
    {
        if (tempNode->node == name)
        {
            exists = true;
            break;
        }
        tempNode = tempNode->next;
    }
    return exists;
}

bool checkNoOfThreads(int threadCount)
{
    if (threadCount < 1)
    {
        return false;
    }
    auto tempNode = headNode;
    int i = 0;
    while (tempNode)
    {
        i++;
        tempNode = tempNode->next;
    }
    tempNode = headNode;
    if (threadCount > i)
    {
        return false;
    }
    else
    {
        return true;
    }
}

//int main() {
//
//    importFromFile("D:/GitHub/apl/cpp/test.txt");
//    alphabet();
//    auto start = std::chrono::steady_clock::now();
//    //linearVersion("Krakow");
//    parallelVersionVer2("Krakow", 3);
//    readListOfNodes();
//    auto end = std::chrono::steady_clock::now();       // end timer (starting & ending is done by measuring the time at the moment the process started & ended respectively)
//    auto time_span = static_cast<std::chrono::duration<double>>(end - start);   // measure time span between start & end
//    std::cout << "\n" << time_span.count() << " s\n";
//    return 0;
//}