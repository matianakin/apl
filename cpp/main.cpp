#include <iostream>
#include "ListOfCities.h"
#include "ListOfConnections.h"
#include <fstream>
using namespace std;

ListOfCities* headCity=nullptr;
ListOfConnections* headConnection = nullptr;

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

void readListofCities()
{
    auto tempCity = headCity;
    while (tempCity)
    {
        cout << "city: " + tempCity->city << " Distance from source: " << tempCity->distance << endl;
        tempCity = tempCity->next;
    }
}

void readListofConnections()
{
    auto tempConnection = headConnection;
}

int main() {
    importFromFile("D:/GitHub/apl/cpp/test.txt");
    readListofCities();
    return 0;
}
