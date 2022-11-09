//Goussard Philemon et Iglesias Guillaume
//Projet electricite GIF 2022 
//Wed Oct 19 02:17:08 PM CEST 2022


#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "defs.h"
#include "menu.h"
#include <conio.h>

// TODO Ajouter / Retirer des centrales et des villes


// TODO Modifier la puissance des centrales

// TODO Ajouter / Retirer des lignes electriques
// TODO Enregistrer le reseau / Charger un nouveau

int main(void){
	HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);

	// switch(mainMenu(hConsole)) {
	// 	case 0:
	// 		clear();
	// 		printf_s("Nouveau projet !");
	// 		break;
	// 	case 1:
	// 		clear();
	// 		printf_s("Charger projet !");
	// 		break;
	// 	case 2:
	// 		clear();
	// 		printf_s("Fermeture...");
	// 		return 0;
	// 	default:
	// 		printf_s("Erreur dans la selection du menu");
	// }

	unsigned char winBase[120][30][2]; // Matrice à manipuler pour le fond
	// Lig, Col, Texte

	for (unsigned char i = 0; i < 120; i++)
	{
		for (unsigned char j = 0; j < 30; j++)
		{
			winBase[i][j][0] = ' ';
			winBase[i][j][1] = color(15,0);
		}
	}

	refreshBase(hConsole, winBase);

	getch();
    SetConsoleTextAttribute(hConsole, 16*0+15);
	return 0;
}


// Faire le menu

