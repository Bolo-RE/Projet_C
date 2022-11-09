#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include "window.h"

char mainMenu( HANDLE hConsole ) {
    clear();

    char select = 0;
    char input;
    do {
		SetConsoleTextAttribute(hConsole, 16*0+15);
        gotoligcol(hConsole, 20, 48);
        printf_s(" 1. Nouveau projet    ");
        gotoligcol(hConsole, 22, 48);
        printf_s(" 2. Charger projet    ");
        gotoligcol(hConsole, 24, 48);
        printf_s(" 3.     Quitter       ");
        gotoligcol(hConsole, 29, 0);
        printf_s("               ");


        if (select < 0) {
            select = 0;
            gotoligcol(hConsole, 29, 0);
            printf_s("Start of buffer");
            SetConsoleTextAttribute(hConsole, 16*15+0);
            gotoligcol(hConsole, 20, 48);
            printf_s(" 1. Nouveau projet    ");
        } else if (select > 2) {
            select = 2;
            gotoligcol(hConsole, 29, 0);
            printf_s("End of buffer  ");
            SetConsoleTextAttribute(hConsole, 16*15+0);
            gotoligcol(hConsole, 24, 48);
            printf_s(" 3.     Quitter       ");
        } else if (select == 0) {
            SetConsoleTextAttribute(hConsole, 16*15+0);
            gotoligcol(hConsole, 20, 48);
            printf_s(" 1. Nouveau projet    ");
        } else if (select == 1) {
            SetConsoleTextAttribute(hConsole, 16*15+0);
            gotoligcol(hConsole, 22, 48);
            printf_s(" 2. Charger projet    ");
        } else if (select == 2) {
            SetConsoleTextAttribute(hConsole, 16*15+0);
            gotoligcol(hConsole, 24, 48);
            printf_s(" 3.     Quitter       ");
        } 

        gotoligcol(hConsole, 29, 0);
        input = getch();

        switch (input)
        {
        case 106:
            select += 1;
            break;
        case 107:
            select -= 1;
            break;
        }


    } while (input != 13);
    SetConsoleTextAttribute(hConsole, 16*0+15);
    return select;
}
