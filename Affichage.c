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

void afficheMenu(HANDLE hConsole){
		
		system("cls");
		SetConsoleTextAttribute(hConsole, 16*3+4);
		gotoligcol(5,5);printf("*********************************");
		gotoligcol(6,5);printf("*             MENU              *");
		gotoligcol(7,5);printf("*********************************");	
		SetConsoleTextAttribute(hConsole, 16*0+15);			
		gotoligcol(10,9);printf("1 - Afficher les centrales ");	
		gotoligcol(11,9);printf("2 - Afficher les villes ");
		gotoligcol(12,9);printf("3 - Afficher les lignes ");
		gotoligcol(13,9);printf("4 - Ajouter/Retirer une centrale ");
		gotoligcol(14,9);printf("5 - Ajouter/Rajouter une ville ");
		gotoligcol(15,9);printf("0 - quit");		
	}
    