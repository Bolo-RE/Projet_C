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
  int id;
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


// Will contains the pointers
static long long p_centrale;
static long long p_ville;

// TODO? Create functions to manipulate those addresses to increase readability

// Ajouter / Retirer ville
// Should rewrite to void ? (using double pointer)
// TODO Get the ville struct from the ID
struct ville* add_ville(struct ville* ville, int code){
  // Using ANSI-C auto cast
  struct ville* temp = malloc(sizeof(struct ville));
  temp->code = code;
  ville->suivant = temp;
  return temp;
}

// Here ville argument is assumed to be the 1st link in the chain
// Implement as double pointer in order to return error code
struct ville* rm_ville(struct ville* ville, int code){
  struct ville* temp = ville;
  while(temp->suivant != NULL){
    // TODO check out how this should be done
    if (temp->suivant->code == code){
      // Need debug
      if (temp->suivant->suivant == NULL){
        temp->suivant = NULL;
      }else{
        temp->suivant = temp->suivant->suivant;
      }
    }
    else{
      temp = temp->suivant;
    }
  }
  return temp;
}
// Ajouter / Retirer centrale
struct centrale* add_centrale(struct centrale* centrale, int id){
  struct centrale* temp = malloc(sizeof(struct centrale));
  temp->id = id;
  temp->lignes = malloc(sizeof(struct ligne));
  temp->prev = centrale;
  centrale->suivant = temp;
  return temp;
}

struct centrale* rm_centrale(struct centrale* centrale, int id){
  // Checking the centrale argument is at position 0
  while(centrale->prev != NULL){
    centrale = centrale->prev;
  }
  // Start looking for value
  while(centrale->suivant != NULL){
    if(centrale->suivant->id == id){
      if (centrale->suivant == NULL){
        centrale->suivant = NULL;
      }else{
      centrale->suivant = centrale->suivant->suivant;
      centrale->suivant->prev = centrale;
      }
    }
  }
  return centrale;
}
// Ajouter / Retirer ligne
struct ligne* add_ligne(struct ligne* lignes, int puissance, struct ville* ville){
  struct ligne* temp = malloc(sizeof(struct ligne));
  temp->puissance = puissance;
  temp->ville = ville;
  lignes->suivant = temp;
  return temp;
}
struct ligne* rm_ligne(struct ligne* lignes, int id){
  struct ligne* temp = lignes;
  while(temp->suivant != NULL){
    // TODO check out how this should be done
    if (temp->suivant->id == id){
      // Need debug
      if (temp->suivant->suivant == NULL){
        temp->suivant = NULL;
      }else{
        temp->suivant = temp->suivant->suivant;
      }
    }
    else{
      temp = temp->suivant;
    }
  }
  return temp;
}
// Modification de la puissance d'une centrale
void change_power(struct centrale* centrale, int puissance){
  struct ligne* lignes = centrale->lignes;
  while(lignes != NULL){
    lignes->puissance = puissance;
    lignes = lignes->suivant;
  }
}
// Recuperer la valeur de puissance d'une ville et la repartition / centrale
void power_display(struct centrale* centrales, int code_ville){
  int p_total = 0;
  while(centrales->suivant != NULL){
    
    struct ligne* lignes = centrales->lignes;
    while(lignes->suivant != NULL){
      if(lignes->ville->code == code_ville){
        printf("La centrale %d apporte %d energie a la ville %d", centrales->id, lignes->puissance, code_ville);  
        // TODO how to print this information on the screen ?
        p_total += lignes->puissance;
      }
      lignes= lignes->suivant;
    }
    centrales = centrales->suivant;  
  }
  printf("Au total, la ville %d recoit %d energie", code_ville, p_total);
}
// TODO Enregistrer le reseau
void save(struct centrale* centrales, struct ville* villes, char* fichier){
  // TODO Check if this network has already been saved ;if true then overwrite else create new file
  // fileexists(fichier);
  if(fichier == NULL){
    fichier = "network.bck";
  }
  char* head = "#VILLES";
  char* tail = "#FINVILLES";
  // TODO Check if centrale, ville and lignes are at first position (get the addr of the first link)
  if(villes == (struct ville*)p_ville){
    // TODO Setup if not working
  }
  FILE* fp;
  if(fp == NULL){
    printf("Error while trying to create file !\n");
  }
  fp = fopen(fichier, "w");
  fputs("#VILLES\n", fp);
  
  while(villes->suivant != NULL){
    fprintf(fp, "%d\n", villes->code);
    villes = villes->suivant;
  }
  fputs("#FINVILLE\n#CENTRALE", fp);
  while(centrales->suivant != NULL){
    fprintf( )
  }
  fputs("#FINCENTRALE",fp);
}
// TODO Charger le reseau depuis un fichier
void load(FILE* file){
  // First loads up towns then links then centrals
}

int main(void){
  struct ville* v = malloc(sizeof(struct ville));
  struct centrale* c = malloc(sizeof(struct centrale));
  struct ville* chain = v;
  chain = add_ville(v, 12);
  chain = add_ville(chain, 13);
  chain = add_ville(chain, 14);
  save(c, v,NULL);
//   chain = rm_ville(v, 13);
//   struct ville* buf = v;
//   while(buf != NULL){
//     printf("value is %d\n", buf->code);
//     buf = buf->suivant;
//   }
//   // TODO Free
}