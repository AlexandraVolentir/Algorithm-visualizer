#include <iostream>
#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include "design.h"
#include "algo.h"

extern Object Block[30];
extern int l;

int main()
{
    char file_name[100] = "SchemeFile.sch";
    bool file_loaded = false;

    char cWhatDoUwant = '\0';
    while(1)
    {
        printf("\n..:: MENU ::..\n");
        printf("1. Press L to load SchemeFile.sch;\n");
        printf("2. Press D to Draw content of scheme file.\n");
        printf("3. Press R to run C++ code.\n\n");
        scanf(" %c", &cWhatDoUwant);
        switch(cWhatDoUwant)
        {
        case 'q':
        case 'Q':
            printf("Bye, bye !\n");
            return 0;
            break;
        case 'l':
        case 'L':
            if(file_loaded == false)
            {
                if(ReadFromFile(file_name))
                {
                    printf("File has been successfully opened.\n");
                    file_loaded = true;
                    MakeVectorOfLinks(0);
                }
                else
                    printf("File %s cannot be processed.\n", file_name);
            }
            else
                printf("You already loaded scheme file !\n");
            break;
        case 'd':
        case 'D':
            if(file_loaded == false)
                printf("You need to load scheme file first !\n");
            else
            {
                InitWindow();
            }
            break;
        case 'r':
        case 'R':
            if(file_loaded == false)
                printf("You need to load scheme file first !\n");
            else
                Eval();
            break;
        default:
            printf("Unknown command: %c. Please try again.\n", cWhatDoUwant);
        }
    }

    return 0;
}
