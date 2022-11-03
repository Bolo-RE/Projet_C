#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void gotoligcol( HANDLE hConsole, int lig, int col )
{
    COORD mycoord;
    
    mycoord.X = col;
    mycoord.Y = lig;
    SetConsoleCursorPosition( hConsole, mycoord );
}

void clear(/* HANDLE hConsole */) {
    system("cls");
    // Solution de la gitanerie dessous si cls ne marche pas
    // gotoligcol(hConsole, 0, 0);
    // for (int i = 0; i < 120*30; i++) // 120 caractères de large pour 30 lignes de haut
    // {
    //    printf_s(" "); 
    // }
}

unsigned char color(unsigned char text, unsigned char bkg) {
    return bkg*16+text;
}

void noise(HANDLE hConsole) {
    gotoligcol(hConsole, 0, 0);
    for (int i = 0; i < 120*3; i++)
    {
        printf_s("-/\\():;,!$");
    }
}

void clearWindow(HANDLE hConsole, int from_lig, int from_col, int to_lig, int to_col) {
    gotoligcol(hConsole, from_lig, from_col);
    for (int i = 0; i < to_lig - from_lig; i++)
    {
        for (int i = 0; i < to_col - from_col; i++)
        {
            printf_s(" ");
        }
        gotoligcol(hConsole, from_lig + i, from_col);
    }
}

void refreshBase(HANDLE hConsole, unsigned char winBase[120][30][2]) {
    clear();
    for (char i = 0; i < 30; i++)
    {
        for (char j = 0; j < 120; j++)
        {
            gotoligcol(hConsole, i, j);
            SetConsoleTextAttribute(hConsole, (WORD)winBase[i][j][1]);
            printf_s("%c", (char)winBase[i][j][0]);
        }
        
    }
    
};
