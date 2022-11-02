//Goussard Philemon et Iglesias Guillaume
//Projet electricite GIF 2022 
//Wed Oct 19 02:17:08 PM CEST 2022

#include <stdio.h>
#include <stdlib.h>

struct ville {
  int code;
  struct ville* suivant;
};

struct ligne {
  int puissance;
  struct ville* ville;
  struct ligne* suivant;
};

struct centrale {
  int id;
  struct ligne* lignes;
  struct centrale* suivant;
  struct centrale* prev;
};

// TODO Ajouter / Retirer ville
// Should rewrite to void ? (using double pointer)
struct ville* add_ville(struct ville* ville, int code){
  // Using ANSI-C auto cast
  struct ville* temp = malloc(sizeof(struct ville));
  temp->code = code;
  ville->suivant = temp;
  return temp;
}

// Here ville argument is assumed to be the 1st link in the chain
// TODO Implement as double pointer in order to return error code
struct ville* rm_ville(struct ville* ville, int code){
  struct ville* temp = ville;
  while(temp->suivant != NULL){
    if (temp->code == code){
      ;
    }
    else{
      temp = temp->suivant;
    }
  }
  return ville;
}
// TODO Ajouter / Retirer centrale
// TODO Ajouter / Retirer ligne
// Modification de la puissance d'une centrale
void change_power(struct centrale* centrale, int power){
  struct ligne* lignes = centrale->lignes;
  while(lignes != NULL){
    lignes->puissance = power;
    lignes = lignes->suivant;
  }
}
// TODO Enregistrer le reseau
// TODO Charger le reseau depuis un fichier
// TODO Recuperer la valeur de puissance d'une ville et la repartition / centrale
