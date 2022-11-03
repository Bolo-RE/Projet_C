
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
		// Liste doublement chainee 
		struct centrale * ptsuivant;
		struct centrale * ptprecedent;
	}Tcentrale;
	
	typedef Tcentrale * PTcentrale;