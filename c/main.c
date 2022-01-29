#include <stdio.h>
#include <fstream>
#include <chrono>
#include "stdbool.h"

struct ListOfCities {
    struct ListOfCities* next;
    struct ListOfCities* prev;
    char* city;
    char* prevCity;
    int distance;
    bool visited;
};

struct ListOfConnections{
        struct ListOfConnections * next;
        struct ListOfConnections* prev;
        char* city1;
        char* city2;
        int distance;
};

struct ListOfCities* headCity=NULL;
struct ListOfConnections* headConnection = NULL;

void importFromFile(char* filename)
{
    FILE* file = fopen(filename, "r");
    if(file)
    {
        char* temp;
        char* tempcity1;
        char* tempcity2;
        int tempdistance;
        int j=0;
        int i=0;
        while(fgets(temp, sizeof(temp), file))
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
                    tempdistance=strtol(temp,  NULL, 10);
                    break;
                }
            }
            if(i==2) {
                if (j == 0) {
                    /*auto city1 = new ListOfCities(tempcity1);
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
                    auto con1 = new ListOfConnections(tempcity1, tempcity2, tempdistance, tempCon);*/
                }
                i=0;
                j++;
            }
            else
            {
                i++;
            }
        }
        //checkForDuplicateCities(headCity);
        //checkForDuplicateConnections(headConnection);
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
    printf("Hello, World!\n");
    return 0;
}
