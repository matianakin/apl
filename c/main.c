#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "stdbool.h"

struct ListOfCities {
    struct ListOfCities* next;
    struct ListOfCities* prev;
    char city[15];
    char prevCity[15];
    int distance;
    bool visited;
};

struct ListOfConnections{
        struct ListOfConnections * next;
        struct ListOfConnections* prev;
        char city1[15];
        char city2[15];
        int distance;
};

struct ListOfCities* headCity=NULL;
struct ListOfConnections* headConnection = NULL;

void readListOfCities()
{
    struct ListOfCities* tempCity = headCity;
    while (tempCity)
    {
        char text1[256];
        strcpy(text1,"\ncity: ");
        char text2[256];
        strcpy(text2, tempCity->city);
        char text3[256];
        strcpy(text3, " Distance from source: ");
        char text5[256];
        strcpy(text5," Previous city: ");
        char text6[256];
        strcpy(text6,tempCity->prevCity);
        strcat(text1, text2);
        strcat(text1, text3);
        strcat(text5, text6);
        printf("%s %i %s", text1, tempCity->distance, text5);
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

void importFromFile(char* filename)
{
    FILE* file = fopen(filename, "r");
    if(file)
    {
        char tempcity1[15];
        char tempcity2[15];
        int tempdistance=0;
        int j=0;
        while(fscanf(file,"%s %s %i", tempcity1, tempcity2, &tempdistance)>0)
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


int main() {
    importFromFile("D:/GitHub/apl/cpp/test.txt");
    readListOfCities();
    return 0;
}
