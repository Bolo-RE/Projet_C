//Goussard Philemon et Iglesias Guillaume
//Projet electricite GIF 2022 
//Wed Oct 19 02:17:08 PM CEST 2022


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

// WARNING Cannot change the simple/double chained structure

/////////  Liste des villes  ////////////////////////
	typedef struct ville {
		int codePostal;
		struct ville * villeSuivante;
	} Ville;

	typedef Ville * PVille;
	
/////////  Liste des lignes electriques /////////////
	typedef struct ligne {
		int puissance;
		// pointeur sur une ville
		PVille villeDesservie;	
		// liste simplement chainee
		struct ligne * ligneSuivante ;
	} ligneElec;
	
	// typedef ligneElec * li;
	
//////////  Liste des centrales electriques ////////	
	typedef struct centrale{
		int codeCentrale;
		// Pointeur sur la liste des lignes 
		ligneElec* villesDependantes;
		int puissance;
		// int coeurs;
		// Liste doublement chainee 
		struct centrale * suivant;
		struct centrale * precedent;
	}Tcentrale;
	
	typedef Tcentrale * PTcentrale;


// TODO Ajouter / Retirer des centrales et des villes
// void insert(void* list, struct <type>, ... (init values))
// TODO Modifier la puissance des centrales
void change_power(Tcentrale centrale, int power){	
	ligneElec* lignes = centrale.villesDependantes;
	while(lignes != NULL){
		lignes->puissance = power;
		lignes = lignes->ligneSuivante;
	}
}
// TODO Ajouter / Retirer des lignes electriques
// TODO Enregistrer le reseau / Charger un nouveau

// TODO Menu


int main(void){
	;
}
