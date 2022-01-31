#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "stdbool.h"
#include "time.h"


struct ListOfCities {
    struct ListOfCities* next;
    struct ListOfCities* prev;
    char city[256];
    char prevCity[256];
    int distance;
    bool visited;
};

struct ListOfConnections{
        struct ListOfConnections * next;
        struct ListOfConnections* prev;
        char city1[256];
        char city2[256];
        int distance;
};

struct ListOfCities* headCity=NULL;
struct ListOfConnections* headConnection = NULL;

void alphabet()
{
    struct ListOfCities* tempCity = headCity;
    struct ListOfCities* tempCity2=tempCity;
    int i=0;
    while(tempCity2)
    {
        i++;
        tempCity2=tempCity2->next;
    }
    for(int j=0; j<i; j++) {
        while (tempCity&&tempCity->next){
            if (strcmp(tempCity->city,tempCity->next->city)>0) {
                struct ListOfCities* tempCityNext = tempCity->next;
                struct ListOfCities* tempCityNextNext = tempCityNext->next;
                struct ListOfCities* tempCityPrev = tempCity->prev;
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

void readListOfCities()
{
    struct ListOfCities* tempCity = headCity;
    while (tempCity)
    {
        printf("%s %s %s %i %s %s", "\ncity: ", tempCity->city, " Distance from source: ", tempCity->distance, " Previous city: ", tempCity->prevCity);
        tempCity = tempCity->next;
    }
}

void checkForDuplicateCities()
{
    struct ListOfCities* temp = headCity;
    struct ListOfCities *temp2 = NULL;
    if(headCity) {
        temp2 = headCity->next;
    }
    while(temp)
    {
        bool changed = false;
        while(temp2) {

            if (strcmp(temp->city,temp2->city) == 0) {
                struct ListOfCities* temp3 = temp2;
                temp2 = temp3->next;
                if(temp2) {
                    temp2->prev = temp3->prev;
                }
                temp3->prev->next=temp2;
                free(temp3);
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
    struct ListOfConnections* temp = headConnection;
    struct ListOfConnections *temp2 = NULL;
    if(headConnection) {
        temp2 = headConnection->next;
    }
    while(temp)
    {
        bool changed = false;
        while(temp2) {

            if ((temp->city1 == temp2->city1 && temp->city2 == temp2->city2)||(temp->city1 == temp2->city2 && temp->city2 == temp2->city1)) {
                struct ListOfConnections* temp3 = temp2;
                temp2 = temp3->next;
                temp3->prev->next = temp2;
                if(temp2) {
                    temp2->prev = temp3->prev;
                }
                free(temp3);
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

void importFromFile(char* filename)
{
    FILE* file = fopen(filename, "r");
    if(file)
    {
        char tempcity1[15];
        char tempcity2[15];
        int tempdistance=0;
        int j=0;
        while(fscanf(file,"%s %s %i", tempcity1, tempcity2, &tempdistance)>0) // NOLINT(cert-err34-c)
        {
                if (j == 0) {
                    struct ListOfCities* city1 = malloc(sizeof(struct ListOfCities));
                    city1->next = NULL;
                    city1->prev = NULL;
                    strcpy(city1->city,tempcity1);
                    city1->distance=INT_MAX;
                    city1->visited = false;
                    strcpy(city1->prevCity,"");
                    headCity=city1;
                    struct ListOfConnections* con1 = malloc(sizeof(struct ListOfConnections));
                    con1->next=NULL;
                    con1->prev = NULL;
                    strcpy(con1->city1,tempcity1);
                    strcpy(con1->city2,tempcity2);
                    con1->distance=tempdistance;
                    headConnection=con1;
                    struct ListOfCities* tempCity = headCity;
                    while (tempCity->next) {
                        tempCity = tempCity->next;
                    }
                    struct ListOfCities* city2 = malloc(sizeof(struct ListOfCities));
                    city2->next = NULL;
                    city2->prev = tempCity;
                    tempCity->next=city2;
                    strcpy(city2->city,tempcity2);
                    city2->distance=INT_MAX;
                    city2->visited = false;
                    strcpy(city2->prevCity,"");
                } else {
                    struct ListOfCities* tempCity = headCity;
                    while (tempCity->next) {
                        tempCity = tempCity->next;
                    }
                    struct ListOfCities* city1 = malloc(sizeof(struct ListOfCities));
                    struct ListOfCities* city2 = malloc(sizeof(struct ListOfCities));
                    struct ListOfConnections* con1 = malloc(sizeof(struct ListOfConnections));
                    city1->prev = tempCity;
                    tempCity->next=city1;
                    strcpy(city1->city,tempcity1);
                    city1->distance=INT_MAX;
                    city1->visited = false;
                    strcpy(city1->prevCity,"");
                    city2->next = NULL;
                    city2->prev = city1;
                    strcpy(city2->city,tempcity2);
                    city2->distance=INT_MAX;
                    city2->visited = false;
                    strcpy(city2->prevCity,"");
                    city1->next = city2;
                    struct ListOfConnections* tempCon = headConnection;
                    while (tempCon->next) {
                        tempCon = tempCon->next;
                    }
                    con1->next=NULL;
                    con1->prev = tempCon;
                    tempCon->next=con1;
                    strcpy(con1->city1,tempcity1);
                    strcpy(con1->city2,tempcity2);
                    con1->distance=tempdistance;
                }
                j++;


        }
        checkForDuplicateCities();
        checkForDuplicateConnections();
    }
    else
    {
        fprintf(stderr, "Error opening file '%s'\n", filename);
        exit(1);
    }
    fclose(file);
}

void linearVersion(char start[256])
{

    struct ListOfCities* startCity = headCity;
    bool finished = false;
    while(startCity)
    {
        if(strcmp(startCity->city,start)==0)
        {
            startCity->distance =0;
            startCity->visited=true;
            strcpy(startCity->prevCity,"<---  This is the starting vector");
            break;
        }
        startCity=startCity->next;
        if(!startCity)
        {
            printf("%s", "Incorrect city provided as a start point\n");
            exit(1);
        }
    }

    struct ListOfCities* current = startCity;

    do {
        finished=true;
        struct ListOfConnections* connectionIterator = headConnection;
        while (connectionIterator) {
            if (strcmp(connectionIterator->city1,current->city)==0) {
                struct ListOfCities* analyzedCity = headCity;
                int road = current->distance + connectionIterator->distance;
                while (analyzedCity) {
                    if (strcmp(analyzedCity->city,connectionIterator->city2)==0) {
                        break;
                    }
                    analyzedCity=analyzedCity->next;
                }
                if(!analyzedCity->visited && analyzedCity->distance > road) {
                    strcpy(analyzedCity->prevCity, current->city);
                    analyzedCity->distance=road;
                }
            }
            else if(strcmp(connectionIterator->city2,current->city)==0)
            {
                struct ListOfCities* analyzedCity = headCity;
                int road = current->distance + connectionIterator->distance;
                while (analyzedCity) {
                    if (strcmp(analyzedCity->city,connectionIterator->city1)==0) {
                        break;
                    }
                    analyzedCity=analyzedCity->next;
                }
                if(!analyzedCity->visited && analyzedCity->distance > road) {
                    strcpy(analyzedCity->prevCity, current->city);
                    analyzedCity->distance=road;
                }
            }
            connectionIterator = connectionIterator->next;
        }

        int tempDist=INT_MAX;
        struct ListOfCities* tempCity = headCity;
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

int main() {

    clock_t start = clock();
    importFromFile("D:/GitHub/apl/cpp/test.txt");
    alphabet();
    linearVersion("Krakow");
    readListOfCities();
    clock_t end = clock();
    double seconds = (double)(end - start)/ CLOCKS_PER_SEC;
    printf("\n%f %s", seconds, "s");
    return 0;
}
