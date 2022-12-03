//Goussard Philemon et Iglesias Guillaume
//Projet electricite GIF 2022 
//Wed Oct 19 02:17:08 PM CEST 2022

// TODO Replace all the printfs by log function
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LOGFILE "dump.log"

struct ville {
  int code;
  char* name;
  struct ville* suivant;
  int y;
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

int logmsg(char* message){
  FILE* fp = fopen(LOGFILE, "a");
  if(fp == NULL){
    return -1;
  }
  fputs(message, fp);
  return 0;
}

void set_p_ville(unsigned long ptr){
  p_ville = ptr;
}

struct ville* get_p_ville(){
  return (struct ville*) p_ville;
}

struct ville* get_ville(int index){
  struct ville* ville = get_p_ville();
  while(ville != NULL){
    if (ville->code == index){
      return ville;
    }
     ville = ville->suivant;   
  }
  return NULL;
}

int reset_ville_position(){
  struct ville* villes = get_p_ville();
  while(villes != NULL){
    villes->y = -1;
    villes = villes->suivant;
  }
  return 0;
}


// Ajouter / Retirer ville
int add_ville(int code, char* name){
  // printf("name is %s\n", name);
  struct ville* next = get_p_ville();
  while(next->suivant != NULL){
    next = next->suivant;
  }
  struct ville* temp = (struct ville*) malloc(sizeof(struct ville));
  if(temp == NULL){
    return -1;
  }
  // This code is reserved to identify the first link and shall not be used elsewhere
  if(next->code==0){
    next->name = name;
    next->code = code;
    //Spot value for the drawing functions
    next->y = -1;
  }else{
    temp->name = name;
    temp->code = code;
    temp->y = -1;
    next->suivant = temp;
  }
  return 0;
}

int rm_ville(int code){
  struct ville* p = get_p_ville();
  struct ville** ville = &p;
  // TODO Free to setup
  while((*ville)->suivant != NULL){
    if ((*ville)->suivant->code == code){
      if((*ville) == get_p_ville()){
        logmsg("removing the first node; changing the value of p_ville");
        set_p_ville((unsigned long)(*ville)->suivant);
      }
      if ((*ville)->suivant->suivant == NULL){
        (*ville)->suivant = NULL;
      }else{
        (*ville)->suivant = (*ville)->suivant->suivant;
      }
      return 0;
    }
    else{
      (*ville) = (*ville)->suivant;
    }
  }
  return -1;
}



struct centrale* get_centrale(struct centrale* centrales, int id){
  struct centrale* res = NULL;
  while(centrales->prev != NULL){
    centrales = centrales->prev;
  }
  while(centrales != NULL){
    if(centrales->id == id){
      if (res == NULL){
        res = centrales;        
      }
      else{
        logmsg("Found double value ! ID should be unique, else undefined behavoir can happen. You have been warned.\n");
      }
    }
    centrales = centrales->suivant;
  }
  return res;
}
// Ajouter / Retirer centrale
int add_centrale(struct centrale** centrale, int id){
  // Checking centrale is at the last position
  // Evaluating if the centrale is not the only block in the chain
  if((*centrale)->prev == NULL && (*centrale)->suivant == NULL && (*centrale)->id == 0){
    (*centrale)->id = id;
    (*centrale)->lignes = NULL;
    return 0;
  }
  while((*centrale)->suivant != NULL){
    (*centrale) = (*centrale)->suivant;
  }
  struct centrale* temp = (struct centrale*) malloc(sizeof(struct centrale));
  temp->id = id;
  // Useful to identify when lignes is empty
  temp->lignes = NULL;
  temp->prev = (*centrale);
  (*centrale)->suivant = temp;
  // while((*centrale)->prev !=NULL){
  //   (*centrale) = (*centrale)->prev;
  // }
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
    logmsg("Issue with ville argument : NULL value\n");
    return -1;
  }
  struct ligne* lignes = centrale->lignes;
  if(lignes == NULL){
    lignes = (struct ligne*) malloc(sizeof(struct ligne));
  }
  while(lignes->suivant != NULL){
    lignes = lignes->suivant;
  }
  struct ligne* temp = (struct ligne*) malloc(sizeof(struct ligne));
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
    if (lignes->suivant->id == id){
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

// TODO For the moment the last saved file is wiped and we write everything back ; maybe thing of append?
int save(struct centrale* centrales,  char* fichier){
  FILE* fp  = fopen(fichier, "w");
  char counter = '0';
  char* base = "network.bck";
  while(fp == NULL){
    strcat(strdup(base), &counter);
    fp = fopen(base, "w");
    counter++;
  }
  struct ville* villes = get_p_ville();
  fputs("#VILLES\n", fp);
  
  while(villes != NULL){
    fprintf(fp, "%d:%s\n", villes->code, villes->name);
    villes = villes->suivant;
  }
  fputs("#FINVILLE\n#CENTRALE\n", fp);
  while(centrales != NULL){
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
  fclose(fp);
  return 0;
}

// The centrale argument is not used, only here for homogenity
int load_ville(char* data, struct centrale* padding){
  int id;
  char* idv  = strtok(data, ":");
  char* name = strtok(NULL, ":");
  if(name == NULL){
    // printf("%s\n", idv);
    return -1;
  }else{
    // printf("created struct ville with values %s and %s\n", idv, name);
    id = strtol(idv, (char**)NULL,  10);
    // printf("created struct ville with values %d and %s \n", id, name);
    add_ville(id, strdup(name));
  }
  return 0;  
}

int load_centrale(char* data, struct centrale* chain){
  int id;
  // Check if line only contains what we need -> an int
  if(strlen(data)>2){
    // printf("str %s\n", data);
    return -1;
  }
  id = strtol(data, (char**)NULL, 10);
  printf("%d\n",id);
  add_centrale(&chain, id);
  return 0;  
}

int load_ligne(char* data, struct centrale* centrale){
  int id;
  int power;
  char* idv  = strtok(data, ":");
  char* powerv = strtok(NULL, ":");
  if(powerv == NULL){
    return -1;
  }else{
    id = strtol(idv, (char**)NULL,  10);
    power = strtol(powerv, (char**)NULL, 10);
    // printf("created struct ligne with values %d and %d in centrale %d\n", id, power, centrale->id);
    add_ligne(centrale, power, get_ville(id));
  }
  return 0;  
}

// Charger le reseau depuis un fichier
int load(char* filename, struct ville* villes, struct centrale* centrales){
  if(filename == NULL)
    filename = "network.bck";
  FILE* fp;
  fp = fopen(filename, "r");
  if(fp==NULL){
    // printf("Error while opening file %s\n", filename);
    return -1;
  }
  char line[100];
  int steps = 0;
  int (*loaders[3])(char*, struct centrale*) = {load_ville, load_centrale, load_ligne};
  char* name;
  int id;
  int power;
  int central_id = 0;
  while(fgets(line, sizeof(line), fp)!=NULL){
    if(strcmp(line, "#FINVILLE\n") == 0){
      steps = 1;
    }
    if(strcmp(line, "#LIGNE\n") == 0){
      steps = 2;
    }
    if(strcmp(line, "#FINLIGNE\n") == 0){
      steps = 1;
      central_id++;
    }

    if(strcmp(line, "#FINCENTRALE\n") == 0){
      fclose(fp);
      return 0;
    }
    if(get_centrale(centrales, central_id)== NULL){
      loaders[steps](line, centrales);
    }else{
      loaders[steps](line, get_centrale(centrales, central_id));
    }
  }
  fclose(fp);
  return 0;
}

