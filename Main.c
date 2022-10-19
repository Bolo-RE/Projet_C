//Goussard Philemon et Iglesias Guillaume
//Projet electricite GIF 2022 
//Wed Oct 19 02:17:08 PM CEST 2022


#include <stdio.h>
#include <stdlib.h>


// WARNING Cannot change the simple/double chained structure

/////////  Liste des villes  ////////////////////////
	typedef struct ville {
		int codePostal;
		struct ville * villeSuivante;
	} Ville;

	typedef Ville * PVille;
	
/////////  Liste des lignes electriques /////////////
	typedef struct lignesElectrique {
		int puissance;
		// pointeur sur une ville
		PVille villeDesservie;	
		// liste simplement chainee
		struct lignesElectrique * ligneSuivante ;
	} TlignesElectrique;
	
	typedef TlignesElectrique * PTligneElectrique;
	
//////////  Liste des centrales electriques ////////	
	typedef struct centrale{
		int codeCentrale;
		// Pointeur sur la liste des lignes 
		PTligneElectrique villeDependante;
		int puissance;
		// int coeurs;
		// Liste doublement chainee 
		struct centrale * ptsuivant;
		struct centrale * ptprecedent;
	}Tcentrale;
	
	typedef Tcentrale * PTcentrale;


// TODO Ajouter / Retirer des centrales et des villes
// void insert(void* list, )
// TODO Modifier la puissance des centrales
void change_power(Tcentrale centrale, int power){	
	centrale.puissance = power;
}
// TODO Ajouter / Retirer des lignes electriques
// TODO Enregistrer le reseau / Charger un nouveau
// TODO Menu


int main(void){
	;
}
