//Goussard Philemon et Iglesias Guillaume
//Projet electricite GIF 2022 
//Wed Oct 19 02:17:08 PM CEST 2022

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct ville {
  int code;
  char* name;
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


// Contient le pointeur vers la premiere ville
static unsigned long p_ville;

void set_p_ville(unsigned long ptr){
  p_ville = ptr;
}

struct ville* get_p_ville(){
  return (struct ville*) p_ville;
}

// Ajouter / Retirer ville
struct ville* get_ville(int index){
  struct ville* ville = get_p_ville();
  while(ville->suivant != NULL){
    if (ville->code == index){
      return ville;
    }
     ville = ville->suivant;   
  }
  return NULL;
}


int add_ville(int code, char* name){
  struct ville* next = get_p_ville();
  while(next->suivant != NULL){
    next = next->suivant;
  }
  struct ville* temp = malloc(sizeof(struct ville));
  if(temp == NULL){
    return -1;
  }
  // This code is reserved to identify the first link and shall not be used elsewhere
  if(next->code==0){
    next->name = name;
    next->code = code;
  }else{
    temp->name = name;
    temp->code = code;
    next->suivant = temp;
  }
  return 0;
}

// Here ville argument is assumed to be the 1st link in the chain
int rm_ville(struct ville** ville, int code){
  while((*ville)->suivant != NULL){
    // TODO check out how this should be done
    if ((*ville)->suivant->code == code){
      // Need debug
      if ((*ville)->suivant->suivant == NULL){
        (*ville)->suivant = NULL;
      }else{
        (*ville)->suivant = (*ville)->suivant->suivant;
      }
    }
    else{
      (*ville) = (*ville)->suivant;
    }
  }
  return 0;
}

struct centrale* get_centrale(struct centrale* centrales, int id){
  while(centrales->prev != NULL){
    centrales = centrales->prev;
  }
  while(centrales->suivant != NULL){
    if(centrales->id == id){
      return centrales;
    }
    centrales = centrales->suivant;
  }
  return NULL;
}
// Ajouter / Retirer centrale
int add_centrale(struct centrale** centrale, int id){
  // Checking centrale is at the last position
  while((*centrale)->suivant != NULL){
    (*centrale) = (*centrale)->suivant;
  }
  struct centrale* temp = malloc(sizeof(struct centrale));
  temp->id = id;
  // Useful to identify when lignes is empty
  temp->lignes = NULL;
  temp->prev = (*centrale);
  (*centrale)->suivant = temp;
  while((*centrale)->prev !=NULL){
    (*centrale) = (*centrale)->prev;
  }
  return 0;
}

int rm_centrale(struct centrale** centrale, int id){
  // Checking the centrale argument is at position 0
  while((*centrale)->prev != NULL){
    (*centrale) = (*centrale)->prev;
  }
  // Start looking for value
  while((*centrale)->suivant != NULL){
    if((*centrale)->suivant->id == id){
      if ((*centrale)->suivant == NULL){
        (*centrale)->suivant = NULL;
      }else{
      (*centrale)->suivant = (*centrale)->suivant->suivant;
      (*centrale)->suivant->prev = (*centrale);
      }
    }
  }
  return 0;
}
// Ajouter / Retirer ligne
int add_ligne(struct centrale* centrale, int puissance, struct ville* ville){
  if (ville == NULL){
    printf("Issue with ville argument : NULL value\n");
    return -1;
  }
  struct ligne* lignes = centrale->lignes;
  if(lignes == NULL){
    lignes = malloc(sizeof(struct ligne));
  }
  while(lignes->suivant != NULL){
    lignes = lignes->suivant;
  }
  struct ligne* temp = malloc(sizeof(struct ligne));
  temp->puissance = puissance;
  temp->ville = ville;
  lignes->suivant = temp;
  if(centrale->lignes == NULL){
    centrale->lignes = temp;
  }else{
    centrale->lignes = lignes;
  }
  return 0;
}
int rm_ligne(struct centrale* centrale, int id){
  struct ligne* lignes = centrale->lignes;
  while(lignes->suivant != NULL){
    // TODO check out how this should be done
    if (lignes->suivant->id == id){
      // Need debug
      if (lignes->suivant->suivant == NULL){
        lignes->suivant = NULL;
      }else{
        lignes->suivant = lignes->suivant->suivant;
      }
    }
    else{
      lignes= lignes->suivant;
    }
  }
  return 0;
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
void save(struct centrale* centrales,  char* fichier){
  // TODO Check if this network has already been saved ;if true then overwrite else create new file
  // fileexists(fichier); -> if .bck file exists; 
  // TODO For the moment the last saved file is wiped and we write everything back ; maybe thing of append?
  struct ville* villes = get_p_ville();
  if(fichier == NULL){
    fichier = "network.bck";
  }
  // TODO Check if centrale, ville and lignes are at first position (get the addr of the first link)
  FILE* fp;
  if(fp == NULL){
    printf("Error while trying to create file !\n");
  }
  fp = fopen(fichier, "w");
  fputs("#VILLES\n", fp);
  
  while(villes->suivant != NULL){
    fprintf(fp, "%d:%s\n", villes->code, villes->name);
    villes = villes->suivant;
  }
  fputs("#FINVILLE\n#CENTRALE\n", fp);
  while(centrales->suivant != NULL){
    // printf("central id %d\n", centrales->id);
    fprintf(fp, "%d\n", centrales->id);
    printf("lignes : %p\n", centrales->lignes);
    if(centrales->lignes == NULL){
      puts("no lignes\n");
      centrales = centrales->suivant;
      continue;
    }
    fprintf(fp, "#LIGNE\n");
    struct ligne* ligne = centrales->lignes;
    while(ligne != NULL){
      printf(" code %d\n", ligne->ville->code);
      fprintf(fp, "%d:%d\n", ligne->ville->code, ligne->puissance);
      ligne = ligne->suivant;
    }
    fprintf(fp, "#FINLIGNE\n");
    centrales = centrales->suivant;
  }
  fputs("#FINCENTRALE",fp);
}
// TODO Charger le reseau depuis un fichier
void load(char* filename){
  // First loads up towns then links then centrals
}

int main(void){
  struct ville* v = malloc(sizeof(struct ville));
  // TODO What if this address change ? Maybe setup check in get_addr
  set_p_ville((unsigned long)v);
  struct centrale* c = malloc(sizeof(struct centrale));
  add_ville(1, "Ville");
  add_ville(2, "Ville");
  add_ville(3, "test3");
  add_ville(5, "test2");
  add_centrale(&c, 1);
  add_centrale(&c, 2);
  add_centrale(&c, 3);
  add_centrale(&c, 4);
  add_ligne(get_centrale(c, 1), 100, get_ville(1));
  add_ligne(get_centrale(c, 1), 100, get_ville(2));
  printf("test ligne %d", get_centrale(c, 1)->lignes->ville->code);
  // add_centrale(&c, 2);
  add_ligne(get_centrale(c,2), 100, get_ville(2));
  save(get_centrale(c,1),NULL);
  // chain = rm_ville(v, 13);
  // struct ville* buf = get_p_ville();
  // while(buf != NULL){
  //   printf("value is %s\n", buf->name);
  //   buf = buf->suivant;
  // }
//   // TODO Free
}