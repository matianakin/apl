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


ListOfNodes* headCity=nullptr;
ListOfConnections* headConnection = nullptr;
ThreadHead* tHead = nullptr;
std::mutex m;
ListOfNodes*current = nullptr;

void checkForDuplicateCities()
{
    auto temp = headCity;
    ListOfNodes* temp2 = nullptr;
    if(headCity)
    {
        temp2 = headCity->next;
    }
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
            if(!changed)
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

void checkForDuplicateConnections()
{
    auto temp = headConnection;
    ListOfConnections* temp2 = nullptr;
    if(headConnection)
    {
        temp2 = headConnection->next;
    }
    while(temp)
    {
        bool changed = false;
        while(temp2) {

            if ((temp->city1 == temp2->city1 && temp->city2 == temp2->city2)||(temp->city1 == temp2->city2 && temp->city2 == temp2->city1)) {
                auto temp3 = temp2;
                temp2 = temp3->next;
                temp3->prev->next = temp2;
                if(temp2) {
                    temp2->prev = temp3->prev;
                }
                delete temp3;
                changed = true;
            }
            if(!changed)
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
    std::fstream file (filename);
    if(file.is_open())
    {
        std::string temp;
        std::string tempcity1;
        std::string tempcity2;
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
                    tempdistance=atoi( temp.c_str() );
                    break;
                }
            }
            if(i==2) {
                if (j == 0) {
                    auto city1 = new ListOfNodes(tempcity1);
                    delete headCity;
                    headCity = city1;
                    auto con1 = new ListOfConnections(tempcity1, tempcity2, tempdistance);
                    delete headConnection;
                    headConnection = con1;
                    auto tempCity = headCity;
                    while (tempCity->next) {
                        tempCity = tempCity->next;
                    }
                    auto *city2 = new ListOfNodes(tempcity2, tempCity);
                } else {
                    auto tempCity = headCity;
                    while (tempCity->next) {
                        tempCity = tempCity->next;
                    }
                    auto city1 = new ListOfNodes(tempcity1, tempCity);
                    auto city2 = new ListOfNodes(tempcity2, tempCity->next);
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
        checkForDuplicateCities();
        checkForDuplicateConnections();
    }
    file.close();
}

void readListOfCities()
{
    auto tempCity = headCity;
    while (tempCity)
    {
        std::cout << "city: " + tempCity->city << " Distance from source: " << tempCity->distance << " Previous city: " << tempCity->prevCity<< std::endl;
        tempCity = tempCity->next;
    }
}

void readListofConnections()
{
    auto tempConnection = headConnection;
    while (tempConnection)
    {
        std::cout << "city1: " + tempConnection->city1 << "city1: " + tempConnection->city1 << " Distance  " << tempConnection->distance << std::endl;
        tempConnection = tempConnection->next;
    }
}

void linearVersion(std::string start)
{

    auto startCity = headCity;
    bool finished = false;
    while(startCity)
    {
        if(startCity->city==start)
        {
            startCity->distance =0;
            startCity->visited=true;
            startCity->prevCity = "<---  This is the starting vector";
            break;
        }
        startCity=startCity->next;
        if(!startCity)
        {
            std::cout<<"Incorrect city provided as a start point"<<std::endl;
            exit(1);
        }
    }

    current = startCity;

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
                if(!analyzedCity->visited && analyzedCity->distance > road) {
                    analyzedCity->prevCity = current->city;
                    //changeDistance(analyzedCity, road);
                    analyzedCity->distance=road;
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
                if(!analyzedCity->visited && analyzedCity->distance > road) {
                    analyzedCity->prevCity = current->city;
                    //changeDistance(analyzedCity, road);
                    analyzedCity->distance=road;
                }
            }
            connectionIterator = connectionIterator->next;
        }


            int tempDist=INT32_MAX;
            auto tempCity = headCity;
            while(tempCity)
            {
                if(!tempCity->visited && tempCity->distance < tempDist) {
                    current = tempCity;
                    tempDist=tempCity->distance;
                    finished = false;
                }
                tempCity = tempCity->next;
            }
            current->visited = true;

    }while(!finished);
}

void prepareParallelization(int threadCount)
{
    if(threadCount<1)
    {
        std::cout<<"Minimal number of threads is 1";
        exit(1);
    }
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
        std::cout<<"Too many threads requested. Number of threads cannot exceed number of vertices."<<std::endl;
        exit(1);
    }
    int vertPerThread =i/threadCount;
    int q=0;
    bool created = false;
    auto tempCityHead = headCity;
    auto tempCityTail = headCity;
    while(tempCity)
    {
        if(q==vertPerThread-1)
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
}

void oneThreadVer1(ThreadHead* th, std::string citySearch, int road)
{
    auto analyzedCity = th->subHead;
    bool broken = false;
    while (analyzedCity) {
        if(th->subEnd&&analyzedCity==th->subEnd->next)
        {
            break;
        }
        if (analyzedCity->city == citySearch) {
            broken = true;
            break;
        }
        analyzedCity = analyzedCity->next;
    }
    if(broken) {
        if (!analyzedCity->visited&&analyzedCity->distance > road) {
            m.lock();
            analyzedCity->prevCity = current->city;
            analyzedCity->distance=road;
            m.unlock();
        }
    }
}

void oneThreadVer2(ThreadHead* th, int &tempDist,  bool &finished)
{
    auto tempCity = th->subHead;
    bool broken = false;
    while (tempCity) {
        if(th->subEnd && tempCity == th->subEnd->next)
        {
            break;
        }
        if(!tempCity->visited && tempCity->distance < tempDist) {
            m.lock();
            current = tempCity;
            tempDist=tempCity->distance;
            finished = false;
            m.unlock();
        }
        tempCity = tempCity->next;
    }
}

void parallelVersionVer1(std::string start, int nth)
{
    auto startCity = headCity;
    bool finished = false;
    while(startCity)
    {
        if(startCity->city==start)
        {
            startCity->distance =0;
            startCity->visited=true;
            startCity->prevCity = "<---  This is the starting vector";
            break;
        }
        startCity=startCity->next;
        if(!startCity)
        {
            std::cout<<"Incorrect city provided as a start point"<<std::endl;
            exit(1);
        }
    }
    current = startCity;
    int noOfThreads =nth;

    prepareParallelization(noOfThreads);

    do {
        finished=true;
        auto connectionIterator = headConnection;
        while (connectionIterator) {
            if (connectionIterator->city1 == current->city) {

                int road = current->distance + connectionIterator->distance;
                std::vector<std::thread*> ThreadVector;
                auto tempT = tHead;
                std::string cit = connectionIterator->city2;
                while(tempT)
                {
                    auto* myThread = new std::thread(oneThreadVer1, tempT, cit, road);
                    ThreadVector.push_back(myThread);
                    tempT=tempT->next;
                }
                for(auto & i : ThreadVector)
                {
                    i->join();
                }

            } else if (connectionIterator->city2 == current->city) {

                int road = current->distance + connectionIterator->distance;
                std::vector<std::thread*> ThreadVector;
                auto tempT = tHead;
                std::string cit = connectionIterator->city1;
                while(tempT)
                {
                    auto* myThread = new std::thread(oneThreadVer1, tempT, cit, road);
                    ThreadVector.push_back(myThread);
                    tempT=tempT->next;
                }
                for(auto & i : ThreadVector)
                {
                    i->join();
                }

            }
            connectionIterator = connectionIterator->next;
        }

        int tempDist=INT32_MAX;
        auto tempCity = headCity;
        while(tempCity)
        {
            if(!tempCity->visited && tempCity->distance < tempDist) {
                current = tempCity;
                tempDist=tempCity->distance;
                finished = false;
            }
            tempCity = tempCity->next;
        }
        current->visited = true;
    }while(!finished);
}

void parallelVersionVer2(std::string start, int nth)
{
    auto startCity = headCity;
    bool finished = false;
    while(startCity)
    {
        if(startCity->city==start)
        {
            startCity->distance =0;
            startCity->visited=true;
            startCity->prevCity = "<---  This is the starting vector";
            break;
        }
        startCity=startCity->next;
        if(!startCity)
        {
            std::cout<<"Incorrect city provided as a start point"<<std::endl;
            exit(1);
        }
    }
    current = startCity;
    int noOfThreads =nth;

    prepareParallelization(noOfThreads);

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
                if(!analyzedCity->visited && analyzedCity->distance > road) {
                    analyzedCity->prevCity = current->city;
                    //changeDistance(analyzedCity, road);
                    analyzedCity->distance=road;
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
                if(!analyzedCity->visited && analyzedCity->distance > road) {
                    analyzedCity->prevCity = current->city;
                    //changeDistance(analyzedCity, road);
                    analyzedCity->distance=road;
                }
            }
            connectionIterator = connectionIterator->next;
        }

        int tempDist=INT32_MAX;
        auto tempT = tHead;
        std::vector<std::thread*> ThreadVector;
        while(tempT)
        {
            auto* myThread = new std::thread(oneThreadVer2, tempT, std::ref(tempDist), std::ref(finished));
            ThreadVector.push_back(myThread);
            tempT=tempT->next;
        }
        for(auto & i : ThreadVector)
        {
            i->join();
        }
        current->visited = true;

    }while(!finished);

}

void parallelVersionVer3(std::string start, int nth)
{
    auto startCity = headCity;
    bool finished = false;
    while(startCity)
    {
        if(startCity->city==start)
        {
            startCity->distance =0;
            startCity->visited=true;
            startCity->prevCity = "<---  This is the starting vector";
            break;
        }
        startCity=startCity->next;
        if(!startCity)
        {
            std::cout<<"Incorrect city provided as a start point"<<std::endl;
            exit(1);
        }
    }
    current = startCity;
    int noOfThreads =nth;

    prepareParallelization(noOfThreads);

    do {
        finished=true;
        auto connectionIterator = headConnection;
        while (connectionIterator) {
            if (connectionIterator->city1 == current->city) {

                int road = current->distance + connectionIterator->distance;
                std::vector<std::thread*> ThreadVector;
                auto tempT = tHead;
                std::string cit = connectionIterator->city2;
                while(tempT)
                {
                    auto* myThread = new std::thread(oneThreadVer1, tempT, cit, road);
                    ThreadVector.push_back(myThread);
                    tempT=tempT->next;
                }
                for(auto & i : ThreadVector)
                {
                    i->join();
                }

            } else if (connectionIterator->city2 == current->city) {

                int road = current->distance + connectionIterator->distance;
                std::vector<std::thread*> ThreadVector;
                auto tempT = tHead;
                std::string cit = connectionIterator->city1;
                while(tempT)
                {
                    auto* myThread = new std::thread(oneThreadVer1, tempT, cit, road);
                    ThreadVector.push_back(myThread);
                    tempT=tempT->next;
                }
                for(auto & i : ThreadVector)
                {
                    i->join();
                }

            }
            connectionIterator = connectionIterator->next;
        }

        int tempDist=INT32_MAX;
        auto tempT = tHead;
        std::vector<std::thread*> ThreadVector;
        while(tempT)
        {
            auto* myThread = new std::thread(oneThreadVer2, tempT, std::ref(tempDist), std::ref(finished));
            ThreadVector.push_back(myThread);
            tempT=tempT->next;
        }
        for(auto & i : ThreadVector)
        {
            i->join();
        }
        current->visited = true;

    }while(!finished);
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
    auto start = std::chrono::steady_clock::now();
    //linearVersion("Krakow");
    parallelVersionVer2("Krakow", 3);
    readListOfCities();
    auto end = std::chrono::steady_clock::now();       // end timer (starting & ending is done by measuring the time at the moment the process started & ended respectively)
    auto time_span = static_cast<std::chrono::duration<double>>(end - start);   // measure time span between start & end
    std::cout<<"\n"<<time_span.count()<<" s\n";
    return 0;
}
