#include <iostream>
#include <string>
#include <curl/curl.h>
#include "CCurl.h"
#include <windows.h>
#include <conio.h>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_ENTER '\r'

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    CCurl curl = CCurl();

    // Menu setup
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    std::string choices[] = {"Simple info", "Detailed info", "Exit"};
    while (true)
    {
        int numChoices = 3;
        int selected = 0;
        bool selecting = true;
    
        while (selecting)
        {
            system("cls");

            for (int i = 0; i < numChoices; i++)
            {
                if (selected == i)
                {
                    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                    std::cout << "> " << i + 1 << ". " << choices[i] << "\n";
                }
                else
                {
                    std::cout << i + 1 << ". " << choices[i] << "\n";
                }
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }

            char c;
            switch ((c = _getch()))
            {
            case KEY_UP:
                if (selected > 0)
                {
                    --selected;
                }
                break;
            case KEY_DOWN:
                if (selected < numChoices - 1)
                {
                    selected++;
                }
                break;
            case KEY_ENTER:
                selecting = false;
                break;
            default:
                break;
            }
        }
        system("cls");
        std::string choice = choices[selected];

        if(choice == "Exit")
            return 0;

        std::string country;
        std::cout << "Enter name: ";
        std::cin >> country;
        std::transform(country.begin(), country.end(), country.begin(), [](unsigned char c) { return std::tolower(c);});

        system("cls");
        
        if (choice == "Simple info")
            std::cout << curl.SimpleInfo(country);
        else if(choice == "Detailed info")
            std::cout << curl.DetailedInfo(country);
        
        std::cout << "Press a button to continue...";
        _getch();
    }
}