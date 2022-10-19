//Goussard Philemon et Iglesias Guillaume
//Projet electricite GIF 2022 

```c
#include <stdio.h>



/////////  Liste des villes  ////////////////////////
	typedef struct ville {
		int codePostal;
		struct ville * villeSuivante;
	} Tville;

	typedef Tville * PTville;
	
/////////  Liste des lignes electriques /////////////
	typedef struct lignesElectrique {
		int puissance;
		// pointeur sur une ville
		PTville villeDesservie;	
		// liste simplement chain�e
		struct lignesElectrique * ligneSuivante ;
	} TlignesElectrique;
	
	typedef TlignesElectrique * PTligneElectrique;
	
//////////  Liste des centrales electriques ////////	
	typedef struct centrale{
		int codeCentrale;
		// Pointeur sur la liste des lignes 
		PTligneElectrique villeDependante;
		// Liste doublement chain�e 
		struct centrale * ptsuivant;
		struct centrale * ptprecedent;
	}Tcentrale;
	
	typedef Tcentrale * PTcentrale;
```
