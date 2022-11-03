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

void cls(HANDLE hConsole)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    SMALL_RECT scrollRect;
    COORD scrollTarget;
    CHAR_INFO fill;

    // Get the number of character cells in the current buffer.
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        return;
    }

    // Scroll the rectangle of the entire buffer.
    scrollRect.Left = 0;
    scrollRect.Top = 0;
    scrollRect.Right = csbi.dwSize.X;
    scrollRect.Bottom = csbi.dwSize.Y;

    // Scroll it upwards off the top of the buffer with a magnitude of the entire height.
    scrollTarget.X = 0;
    scrollTarget.Y = (SHORT)(0 - csbi.dwSize.Y);

    // Fill with empty spaces with the buffer's default text attribute.
    fill.Char.UnicodeChar = TEXT(' ');
    fill.Attributes = csbi.wAttributes;

    // Do the scroll
    ScrollConsoleScreenBuffer(hConsole, &scrollRect, NULL, scrollTarget, &fill);

    // Move the cursor to the top left corner too.
    csbi.dwCursorPosition.X = 0;
    csbi.dwCursorPosition.Y = 0;

    SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition);
}

void afficheMenu(HANDLE hConsole){  //Affiche les différents choix à l'utilisateur
		 
		cls(hConsole);
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
		gotoligcol(15,9);printf("0 - Quitter");		
	}
//Pour les Ajouter/Retirer

int main() {
    HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    while (2) {
        afficheMenu(hConsole);
        switch(getchar()) {     //switch fonctionne en cascade, une fois la condition validée 
            case 'a':           // toute la suite de la boucle est exécutée. On sort grâce à break
                next();
            case 'i':
                append();
                break;
            default:
                printf_s("\nNon défini !");
        }
    }
    afficheMenu(hConsole);
    return 0;
}
