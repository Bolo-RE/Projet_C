/* Print des formes carrés pour les centrales, cercles pour les villes et traçage des 
lignes reliant les villes et les centrales. Les lignes seront de la couleur de la centrale */

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

void gotoligcol( int lig, int col )
	{
		COORD mycoord;
		
		mycoord.X = col;
		mycoord.Y = lig;
		SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), mycoord );
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

