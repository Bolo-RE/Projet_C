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

int main(void){
	HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);