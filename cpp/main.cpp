#include <iostream>
#include "ListOfCities.h"
#include "ListOfConnections.h"
#include "ThreadHead.h"
#include <fstream>
#include <thread>
#include <vector>

using namespace std;

ListOfCities* headCity=nullptr;
ListOfConnections* headConnection = nullptr;
ThreadHead* tHead = nullptr;


void checkForDuplicateCities(ListOfCities* head)
{
    auto temp = head;
    auto temp2 = head->next;
    while(temp)
    {
        bool changed = false;
        while(temp2) {

            if (temp->city == temp2->city) {
                auto temp3 = temp2;
                temp2 = temp3->next;
                if(temp2) {
                    temp2->prev = temp3->prev;
                }
                temp3->prev->next=temp2;
                delete temp3;
                changed = true;
            }
            if(!changed&&temp2)
            {
                temp2 = temp2->next;
            }
            else
            {
                changed = false;
            }
        }
        temp = temp->next;
        if(temp)
        temp2 = temp->next;
    }
}

void checkForDuplicateConnections(ListOfConnections* head)
{
    auto temp = head;
    auto temp2 = head->next;
    while(temp)
    {
        bool changed = false;
        while(temp2) {

            if (temp->city1 == temp2->city1 && temp->city2 == temp2->city2) {
                auto temp3 = temp2;
                temp2 = temp3->next;
                if(temp2) {
                    temp2->prev = temp3->prev;
                    temp2->prev->next = temp2;
                }
                delete temp3;
                changed = true;
            }
            if(!changed&&temp2)
            {
                temp2 = temp2->next;
            }
            else
            {
                changed = false;
            }
        }
        temp = temp->next;
        if(temp)
            temp2 = temp->next;
    }
}

void importFromFile(std::string filename)
{
    fstream file (filename);
    if(file.is_open())
    {
        string temp;
        string tempcity1;
        string tempcity2;
        int tempdistance;
        int j=0;
        int i=0;
        while(file>>temp)
        {
            switch (i) {
                case 0:
                {
                    tempcity1 =temp;
                    break;
                }
                case 1:
                {
                    tempcity2 =temp;
                    break;
                }
                case 2:
                {
                    tempdistance=std::stoi(temp);
                    break;
                }
            }
            if(i==2) {
                if (j == 0) {
                    auto city1 = new ListOfCities(tempcity1);
                    delete headCity;
                    headCity = city1;
                    auto con1 = new ListOfConnections(tempcity1, tempcity2, tempdistance);
                    delete headConnection;
                    headConnection = con1;
                    auto tempCity = headCity;
                    while (tempCity->next) {
                        tempCity = tempCity->next;
                    }
                    auto *city2 = new ListOfCities(tempcity2, tempCity);
                } else {
                    auto tempCity = headCity;
                    while (tempCity->next) {
                        tempCity = tempCity->next;
                    }
                    auto city1 = new ListOfCities(tempcity1, tempCity);
                    auto city2 = new ListOfCities(tempcity2, tempCity->next);
                    auto tempCon = headConnection;
                    while (tempCon->next) {
                        tempCon = tempCon->next;
                    }
                    auto con1 = new ListOfConnections(tempcity1, tempcity2, tempdistance, tempCon);
                }
                i=0;
                j++;
            }
            else
            {
                i++;
            }
        }
        checkForDuplicateCities(headCity);
        checkForDuplicateConnections(headConnection);
    }
    file.close();
}

void readListOfCities()
{
    auto tempCity = headCity;
    while (tempCity)
    {
        cout << "city: " + tempCity->city << " Distance from source: " << tempCity->distance << " Previous city: " << tempCity->prevCity<< endl;
        tempCity = tempCity->next;
    }
}

void readListofConnections()
{
    auto tempConnection = headConnection;
    while (tempConnection)
    {
        cout << "city1: " + tempConnection->city1 << "city1: " + tempConnection->city1 << " Distance  " << tempConnection->distance << endl;
        tempConnection = tempConnection->next;
    }
}

void changeDistance(ListOfCities* city, int newDistance)
{
    city->distance = newDistance;
    auto tempCity = headCity;
    while (tempCity)
    {
        if(tempCity->prevCity==city->city)
        {
            auto tempCon = headConnection;
            while(tempCon)
            {
                if((tempCon->city1==city->city && tempCon->city2==tempCity->city)||(tempCon->city2==city->city && tempCon->city1==tempCity->city))
                {
                    break;
                }
                tempCon = tempCon->next;
            }
            int newSubDistance=city->distance+tempCon->distance;
            changeDistance(tempCity, newSubDistance);
        }
        tempCity = tempCity->next;
    }
}

void changeDistanceThread(ListOfCities* city, int newDistance)
{
    city->distance = newDistance;
    auto tempT = tHead;
    while(tempT)
    {
        auto tempCity = tempT->subHead;
        while(tempCity) {
        if(tempCity->prevCity==city->city)
        {
            auto tempCon = headConnection;
            while(tempCon)
            {
                if((tempCon->city1==city->city && tempCon->city2==tempCity->city)||(tempCon->city2==city->city && tempCon->city1==tempCity->city))
                {
                    break;
                }
                tempCon = tempCon->next;
            }
            int newSubDistance=city->distance+tempCon->distance;
            changeDistance(tempCity, newSubDistance);
        }
            tempCity=tempCity->next;
        }
        tempT = tempT->next;
    }
}

void linearVersion(std::string start)
{

    auto startCity = headCity;
    bool found = false;
    bool finished = false;
    while(startCity)
    {
        if(startCity->city==start)
        {
            found=true;
            startCity->distance =0;
            startCity->visited=true;
            startCity->prevCity = "This is the starting vector";
            break;
        }
        startCity=startCity->next;
    }
    if(!found)
    {
        cout<<"Incorrect city provided as a start point"<<endl;
        exit(1);
    }

    auto neighbor = headCity;
    auto current = startCity;
    if(startCity->next) {
        neighbor = startCity->next;
    }

    do {
        finished=true;
        auto connectionIterator = headConnection;
        while (connectionIterator) {
            if (connectionIterator->city1 == current->city) {
                auto analyzedCity = headCity;
                int road = current->distance + connectionIterator->distance;
                while (analyzedCity) {
                    if (analyzedCity->city == connectionIterator->city2) {
                        break;
                    }
                    analyzedCity=analyzedCity->next;
                }
                if(!analyzedCity->visited) {
                    if (analyzedCity->distance > road) {
                        analyzedCity->prevCity = current->city;
                        changeDistance(analyzedCity, road);
                    }
                    if (analyzedCity->distance < neighbor->distance) {
                        neighbor = analyzedCity;
                    }
                }
            }
            else if(connectionIterator->city2 == current->city)
            {
                auto analyzedCity = headCity;
                int road = current->distance + connectionIterator->distance;
                while (analyzedCity) {
                    if (analyzedCity->city == connectionIterator->city1) {
                        break;
                    }
                    analyzedCity=analyzedCity->next;
                }
                if(!analyzedCity->visited) {
                    if (analyzedCity->distance > road) {
                        analyzedCity->prevCity = current->city;
                        changeDistance(analyzedCity, road);
                    }
                    if (analyzedCity->distance < neighbor->distance) {
                        neighbor = analyzedCity;
                    }
                }
            }
            connectionIterator = connectionIterator->next;
        }

        if(neighbor!=current)
        {
            current=neighbor;
            current->visited=true;
        }
        else
        {
            int tempDist=INT32_MAX;
            auto tempCity = headCity;
            bool extra=false;
            while(tempCity)
            {
                if(!tempCity->visited)
                {
                    if(tempCity->distance<tempDist) {
                        current = tempCity;
                        extra=true;
                        tempDist=tempCity->distance;
                    }
                }
                tempCity = tempCity->next;
            }
            if(extra) {
                current->visited = true;
                finished = false;
            }
        }

        if(finished) {
            auto tempCity = headCity;
            while (tempCity) {
                if (!tempCity->visited) {
                    finished = false;
                    break;
                }
                tempCity = tempCity->next;
            }
        }
    }while(!finished);
}

void prepareParallelization(int threadCount)
{
    auto tempCity = headCity;
    int i =0;
    while(tempCity)
    {
        i++;
        tempCity = tempCity->next;
    }
    tempCity = headCity;
    if(threadCount>i)
    {
        cout<<"Too many threads requested. Number of threads cannot exceed number of vertices."<<endl;
        exit(1);
    }
    int vertPerThread =i/threadCount;
    int q=0;
    bool created = false;
    auto tempCityHead = headCity;
    auto tempCityTail = headCity;
    while(tempCity)
    {
        if(q==vertPerThread)
        {
            tempCityTail = tempCity;
            if(!created)
            {
                created = true;
                tHead= new ThreadHead(tempCityHead, tempCityTail);
                if(tempCity->next)
                {
                    tempCityHead=tempCity->next;
                }
            }
            else
            {
                auto tempThreader = new ThreadHead(tHead, tempCityHead, tempCityTail);
                tHead=tempThreader;
                if(tempCity->next)
                {
                    tempCityHead=tempCity->next;
                }
            }
            q=-1;
        }
        q++;
        tempCity = tempCity->next;
    }
    if(vertPerThread*threadCount!=i)
    {
        tempCityTail = tempCity;
        auto tempThreader = new ThreadHead(tHead, tempCityHead, tempCityTail);
        tHead=tempThreader;
    }
    auto threadIndex = tHead;
    while (threadIndex)
    {
        if(threadIndex->subEnd)
        threadIndex->subEnd->next=nullptr;
        threadIndex = threadIndex->next;
    }
}

void unThread() {
    auto tempCityTail = tHead->subHead;
    while (tempCityTail->next)
    {
        tempCityTail=tempCityTail->next;
    }
    while(tempCityTail)
    {
        if(tempCityTail->prev)
        {
            tempCityTail->prev->next=tempCityTail;
        }
        tempCityTail=tempCityTail->prev;
    }
}

void oneThread(int road, ThreadHead* th, ListOfCities* current, ListOfCities* neighbor, string citySearch)
{
    auto analyzedCity = th->subHead;
    bool broken = false;
    while (analyzedCity) {
        if (analyzedCity->city == citySearch) {
            broken = true;
            break;
        }
        analyzedCity = analyzedCity->next;
    }
    if(broken) {
        if (!analyzedCity->visited) {
            if (analyzedCity->distance > road) {
                analyzedCity->prevCity = current->city;
                changeDistanceThread(analyzedCity, road);
            }
            if (analyzedCity->distance < neighbor->distance) {
                neighbor = analyzedCity;
            }
        }
    }
    
}

void parallelVersion(std::string start)
{
    auto startCity = headCity;
    bool found = false;
    bool finished = false;
    while(startCity)
    {
        if(startCity->city==start)
        {
            found=true;
            startCity->distance =0;
            startCity->visited=true;
            startCity->prevCity = "This is the starting vector";
            break;
        }
        startCity=startCity->next;
    }
    if(!found)
    {
        cout<<"Incorrect city provided as a start point"<<endl;
        exit(1);
    }

    auto neighbor = headCity;
    auto current = startCity;
    if(startCity->next) {
        neighbor = startCity->next;
    }

    int noOfThreads =2;

    prepareParallelization(noOfThreads);

    do {
        finished=true;
        auto connectionIterator = headConnection;
        while (connectionIterator) {
            if (connectionIterator->city1 == current->city) {

                int road = current->distance + connectionIterator->distance;
                vector<>
            } else if (connectionIterator->city2 == current->city) {
                auto analyzedCity = headCity;
                int road = current->distance + connectionIterator->distance;
                while (analyzedCity) {
                    if (analyzedCity->city == connectionIterator->city1) {
                        break;
                    }
                    analyzedCity = analyzedCity->next;
                }
                if (!analyzedCity->visited) {
                    if (analyzedCity->distance > road) {
                        analyzedCity->prevCity = current->city;
                        changeDistance(analyzedCity, road);
                    }
                    if (analyzedCity->distance < neighbor->distance) {
                        neighbor = analyzedCity;
                    }
                }
            }
            connectionIterator = connectionIterator->next;
        }








        if(neighbor!=current)
        {
            current=neighbor;
            current->visited=true;
        }
        else
        {
            int tempDist=INT32_MAX;
            auto tempT = tHead;
            bool extra=false;
            while(tempT)
            {
                auto tempCity = tempT->subHead;

                while(tempCity) {
                    if (!tempCity->visited) {
                        if (tempCity->distance < tempDist) {
                            current = tempCity;
                            extra = true;
                            tempDist = tempCity->distance;
                        }
                    }
                    tempCity=tempCity->next;
                }
                tempT = tempT->next;
            }
            if(extra) {
                current->visited = true;
                finished = false;
            }
        }

        if(finished) {
            auto tempT = tHead;
            while(tempT)
            {
                auto tempCity = tempT->subHead;
                while(tempCity) {
                    if (!tempCity->visited) {
                        finished = false;
                        break;
                    }
                    tempCity=tempCity->next;
                }
                tempT = tempT->next;
                if(!finished)
                {
                    break;
                }
            }
        }
    }while(!finished);

    unThread();
}

void alphabet()
{
    auto tempCity = headCity;
    auto tempCity2=tempCity;
    int i=0;
    while(tempCity2)
    {
        i++;
        tempCity2=tempCity2->next;
    }
    for(int j=0; j<i; j++) {
        while (tempCity&&tempCity->next){
            if (tempCity->city > tempCity->next->city) {
                auto tempCityNext = tempCity->next;
                auto tempCityNextNext = tempCityNext->next;
                auto tempCityPrev = tempCity->prev;
                if (tempCityNextNext) {
                    tempCityNextNext->prev = tempCity;
                }
                tempCityNext->next = tempCity;
                tempCity->prev = tempCityNext;
                tempCity->next = tempCityNextNext;
                tempCityNext->prev = tempCityPrev;
                if (tempCityPrev) {
                    tempCityPrev->next = tempCityNext;
                }
            }
            tempCity = tempCity->next;
        }
        while(headCity->prev)
        {
            headCity=headCity->prev;
        }
        tempCity = headCity;
    }
}

int main() {
    importFromFile("D:/GitHub/apl/cpp/test.txt");
    alphabet();
    linearVersion("Wroclaw");
    prepareParallelization(5);
    unThread();
    readListOfCities();
    return 0;
}
