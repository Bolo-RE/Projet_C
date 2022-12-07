#include "protocol.h"
#include "backend.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define NOCOLOR -1
#define LIGNES 30
#define COLONNES 120
#define OFFSET 2
#define XCENTRALE 4
#define XVILLE 80
#define XSIDEBAR 90
#define OINDEX 20
// TODO Put that in tmp files in Windows version
#define DATAPANEL "view.dat"
#define SALPHA 42
#define SNUM 10
#define BACKSPACE 8

struct func {
  char* name;
  int (*fun)();
  char mapped;
};

struct screen {
  int toolbar;  // If non 0 draw the toolbar at given offset
  int infobar;  // If non 0 draw the infobar -> how to keep the information ?
  int display; // If non 0 show the view for given centrale ID
  int input;  // If 1 show input else no
};

char alphabet[]={'a' ,'b' ,'c' ,'d' ,'e' ,'f' ,'g' ,'h' ,'i' ,'j' ,'k' ,'l' ,'m' ,'n' ,'o' ,'p' ,'q' ,'r' ,'s' ,'t' ,'u' ,'v' ,'w' ,'x' ,'y' ,'z', 'A' ,'B' ,'C' ,'D' ,'E' ,'F' ,'G' ,'H' ,'I' ,'J' ,'K' ,'L' ,'M' ,'N' ,'O' ,'P' ,'Q' ,'R' ,'S' ,'T' ,'U' ,'V' ,'W' ,'X' ,'Y' ,'Z'};
char num[]={'0' ,'1' ,'2' ,'3' ,'4' ,'5' ,'6' ,'7' ,'8' ,'9'};
// TODO Create enum containing the error
// TODO Error code handling
static int baroffset = 0;
static int paneloffset = 0;
struct ville* villes; 
struct centrale* centrales;
static struct screen scr;
char* getinput(char* msg, char alphabet[], int salphabet);
void showWarning(char* message);
int drawtoolbar(int n);
int incrtoolbar(void);
int dincrtoolbar(void);
int incrpanel(void);
int dincrpanel(void);
int getinfoVille(void);
int getinfoCentrale(void);
int setpower(void);
int createVille(void);
int createCentrale(void);
int createLigne(void);
int rmVille(void);
int rmCentrale(void);
int rmLigne(void);
int showHelp(void);
int toggleToolbar(void);
int listVille(void);
int listCentrale(void);
int saveFile(void);
int loadFile(void);
int drawNode(void);
int quit(void);
int drawnetwork();
int drawnode(struct centrale* centrale);
void init(void);
int draw(void);
void drawpanel(int offset);
struct func getfnc(char map);


// Toolbar is using the numbers to select the function needed
// User can also define keybindings to some functions
struct func funcs[] = {
  {"",incrtoolbar,'a'}, 
  {"",dincrtoolbar, 'b'}, 
  {"infoVille", getinfoVille, 'c'},
  {"infoCentrale", getinfoCentrale, 'd'},
  {"setPower", setpower, 'f'},
  {"createVille", createVille, 'g'},
  {"createCentrale", createCentrale, 'h'},
  {"createLigne", createLigne, 'i'},
  {"rmVille", rmVille, 'j'},
  {"rmCentrale", rmCentrale, 'k'},
  {"rmLigne", rmLigne, 'l'},
  {"help", showHelp, 'm'},
  {"toggleToolbar", toggleToolbar, 'n'},
  {"listVille", listVille, 'o'},
  {"listCentrale", listCentrale, 'p'},
  {"save", saveFile, 'q'},
  {"load", loadFile, 'r'},
  {"refresh", draw, 's'},
  {"nextpanel", incrpanel, 't'},
  {"prevpanel", dincrpanel, 'u'},
  {"drawNode", drawNode, 'v'},
  {"exit", quit, 'z'},
  {"", NULL, '1'},
  {"", NULL, '2'},
  {"", NULL, '3'},
  {"", NULL, '4'},
  {"", NULL, '5'},
  {"", NULL, '6'},
  {"", NULL, '7'},
  {"", NULL, '8'},
  {"", NULL, '9'},
  {"", NULL, '0'},
 
};

int quit(void){
  exit(0);
}

int setpower(void){
  int cid = atoi(getinput("Id: ", num, SNUM));
  struct centrale* c = get_centrale(centrales, cid);
  if(c == NULL){
    showWarning("Centrale inexistante");
  }
  else{
    int power = atoi(getinput("Puissance :", num, SNUM));
    change_power(c, power);
  }
  draw();
  return 0;
}

int incrpanel(void){
  paneloffset++;
  drawpanel(paneloffset);
  return 0;
}

int drawNode(void){
  int id = atoi(getinput("Id : ", num, SNUM));
  scr.display=id;
  drawnode(get_centrale(centrales, id));
  return 0;
}

int dincrpanel(void){
  paneloffset = paneloffset == 0 ? 0  : paneloffset-1;
  drawpanel(paneloffset);
  return 0;
}
int incrtoolbar(void){
  cls();
  baroffset++;
  drawtoolbar(baroffset);
  return 0;
}
int dincrtoolbar(void){
  baroffset = baroffset <= 1 ? 1  : baroffset-1;
  drawtoolbar(baroffset);
  return 0;
}

int toggleToolbar(void){
  scr.toolbar = -1;
  draw();
  return 0;
}


int createVille(void){
  char* name = getinput("Nom de la ville :", alphabet, SALPHA);
  int id = atoi(getinput("Id :",num, SNUM));
  int res = add_ville(id, name);
  if(res == -2){
    showWarning("Une ville avec le id existe deja");
  }
  else if (res == -1){
    showWarning("Allocation error");
  }
  draw();
  return 0;
}

int rmVille(void){
  int id = atoi(getinput("Id :",num, SNUM));
  int ret = rm_ville(id);
  if(ret == -1){
    showWarning("ID non trouve");
  }
  draw();
  return 0;
}

int rmCentrale(void){
  int id = atoi(getinput("Id :",num, SNUM));
  int ret = rm_centrale(&centrales, id);
  draw();
  return 0;
}

int createCentrale(void){
  int id = atoi(getinput("Id :",num, SNUM));
  int ret = add_centrale(&centrales, id);
  if(ret == -2){
    showWarning("Une centrale avec cet ID existe deja");
  }
  draw();
  return 0;
}

int createLigne(void){
  int cid = atoi(getinput("Id de la centrale :", num, SNUM));
  int vid = atoi(getinput("Id de la ville : ",num, SNUM));
  int power = atoi(getinput("Puissance :",num, SNUM));
  int ret = add_ligne(get_centrale(centrales, cid), power, get_ville(vid));
  if(ret == -1){
    showWarning("Ville ou centrale inexistante");
  }
  draw();
  return 0;
}

int rmLigne(void){
  int cid = atoi(getinput("Id de la centrale :", num, SNUM));
  struct centrale* c = get_centrale(centrales, cid);
  if(c->lignes == NULL){
    showWarning("Cette centrale ne contient pas de lignes");
    return 0;
  }
  else{
    int lid = atoi(getinput("Id de la ligne :", num, SNUM));
    int ret = rm_ligne(centrales, lid);
  }
  return 0;
}

int loadFile(void){
  char* filename = getinput("Nom du fichier : ", alphabet, SALPHA);
  load(filename, villes, centrales);
  draw();
  return 0;
}

int saveFile(void){
  char* filename = getinput("Nom du fichier : ", alphabet, SALPHA);
  save(centrales, filename);
  return 0;
}

int getinfoVille(void){
  int id = atoi(getinput("Id de la ville : ", alphabet, SNUM));
  struct ville* sel = get_ville(id);
  if (sel == NULL){
    showWarning("Ville non trouvee");
    return 0;
  }
  FILE* fp  = fopen(DATAPANEL, "w");
  fprintf(fp, "%s\n%d\n%d", sel->name, sel->code, getpower_ville(centrales, sel->code));
  fclose(fp);
  drawpanel(0);
  return 0;
}

int getinfoCentrale(void){
  int id = atoi(getinput("Id centrale : ", num, SNUM));
  struct centrale* c = get_centrale(centrales, id);
  if(c == NULL){
    showWarning("Centrale non trouvee ");
    return 0;
  }
  FILE* fp = fopen(DATAPANEL, "w");
  fprintf(fp, "%d\n%d", c->id, getpower_centrale(centrales, id));
  fclose(fp);
  drawpanel(0);
  return 0;
}

int listVille(void){
  listVilles(DATAPANEL); 
  drawpanel(0);
  return 0;
}

int listCentrale(){
  listCentrales(DATAPANEL, centrales);
  drawpanel(0);
  return 0;
}

// Using the real index value for numbers as should not change. 
// TODO Get the position for the '1' value and increment after that
// TODO Fix the value when decreasing
int drawtoolbar(int n){
  locate(1, scr.toolbar);
  int i = n;
  int j = 0;
  int k = OINDEX;
  int size = COLONNES;
  int len = sizeof(funcs) / sizeof(struct func);
  char toolbar[COLONNES];
  while(1){
    j = i % len;
    if(strlen(funcs[j].name) == 0){
      i++;
      continue;
    }
    size = size - strlen(funcs[j].name) - 4;
    if (size < 0){
      break;
    }
    funcs[k].fun = funcs[j].fun;
    sprintf(toolbar, "%s | %s",toolbar, funcs[j].name);
    i++;
    k++;
  }
  printf("%s | ",toolbar);
  return 0;
}

void showWarning(char* message){
  cls();
  char line[strlen(message)-1];
  memset(line, '-', sizeof(char)*strlen(message));
  int x = (COLONNES-strlen(message))/2;
  int y = LIGNES/2;
  locate(x, y++);
  printf("%s", line);
  locate(x, y++);
  printf("%s", message);
  locate(x, y);
  printf("%s", line);
  getch();
  draw();
}


char* getinput(char* msg, char alphabet[], int salpha){
  char clear[COLONNES];
  memset(clear, ' ', (COLONNES-1)*sizeof(char));
  locate(1,LIGNES-1);
  printf("%s\0", clear);
  locate(1,LIGNES);
  printf("%s\0", clear);
  locate(1,LIGNES-1);
  puts(msg);
  locate(1,LIGNES);
  int x = 1;
  // Should be enough
  char input[100] = {0};
  while(1){    
      char k = getkey();
      if(k == KEY_ENTER){
        return strdup(input);
      }
      else if(k == KEY_ESCAPE){
        return NULL;
      }
      // 8 : backspace
      // 9 : tab
      else if(k == BACKSPACE){
        //Remove a char
        input[strlen(input)-1] = 0;
        locate(x, LIGNES);
        setChar(' ');
        x  =  x == 1 ? x : x - 1;
      }
      if (memchr(alphabet, k, salpha)){
        if(x >= 100){
          showWarning("Ooops... Seems command you're typing is a bit too long");
          memset(input, '0', sizeof(char)*100);
          x = 0;
        }
        sprintf(input, "%s%c", input, k);
        locate(1, 1);
        locate(x, LIGNES);
        setChar(k);
        x++;
      }
  }
}

int showHelp(void){
  showWarning("Please refer to the manual at address : https://github.com/Bolo-RE/Projet_C/tree/dev-linux");
  return 0;
}

//TODO Setup offset; should see the next page if not then change the value of scr.panel
void drawpanel(int i){
  int y = 3;
  char s[100];
  int j  = 0;
  FILE* fp = fopen(DATAPANEL, "r");
  char line[COLONNES-XSIDEBAR];
  // TOOD Find a way to patch this
  memset(line, ' ', sizeof(char)*(COLONNES-XSIDEBAR));
  while(y <= LIGNES){
    locate(XSIDEBAR, y);
    printf("%s", line);
    y++;
  }
  y = 3;
  memset(line, '+', sizeof(char)*(COLONNES-XSIDEBAR));
  locate(XSIDEBAR, y);
  colorPrint(GREEN, NOCOLOR, "%s", line);
  y++;
  while(fgets(s, sizeof s, fp)!=NULL){
    if(j < i*LIGNES){
      j++;
      continue;
    }
    locate(XSIDEBAR, y);
    if(strlen(s) >= COLONNES){
      showWarning("Ligne trop longue pour etre affichee.");
    }else{
      printf("+ %s", s); 
    }
    y++;
  }
  locate(XSIDEBAR, y);
  colorPrint(GREEN, NOCOLOR, "%s", line);
  fclose(fp);
}

int drawnetwork(){
  resetYvilles();
  resetYcentrales(centrales);
  int ycentrale = 3;
  int yville = 3;
  struct centrale* c = centrales;
  struct centrale* l = centrales;
  struct ville* v = villes;
  locate(XCENTRALE, ycentrale);
  while(c != NULL && ycentrale <= LIGNES){
    printf("C%d", c->id);
    c->y = ycentrale;
    c = c->suivant;
    ycentrale++;
    locate(XCENTRALE,ycentrale);
  }
  while(v!=NULL && yville <= LIGNES){
    locate(XVILLE, yville);
    printf("%c%c",v->name[0], v->name[strlen(v->name)-1]);
    v->y = yville;
    v = v->suivant;
    yville++;
  }
  if(yville > LIGNES || ycentrale > LIGNES){
    showWarning("Reseau trop large pour l'ecran. Utiliser la fonction drawNode");
    return -1;
  }
  while(l!=NULL){
    struct ligne* lignes = l->lignes;
    while(lignes != NULL){
      if(lignes->ville->y == -1){
        lignes = lignes->suivant;
        continue;
      }
      char* thighness = ".";
      int offset = rand() % XVILLE;
      int xline = XCENTRALE+2;
      int ymin = lignes->ville->y > l->y ? l->y : lignes->ville->y;
      int ymax = lignes->ville->y < l->y ? l->y : lignes->ville->y;
      if (lignes->puissance > 100)
        thighness = "o";
      if(lignes->puissance > 200)
        thighness = "O";
      if(lignes->ville->y == l->y){

        while(xline <= XVILLE-1){
         
          locate(xline, l->y);
          colorPrint(RED, NOCOLOR, thighness);
          xline++;
        }
      }
      else{
        while(xline <= offset){
          locate(xline, l->y);
          colorPrint(BLUE, NOCOLOR, "%s", thighness);
          xline++;
        }
        while(ymin <= ymax){
          locate(offset, ymin);
          colorPrint(BLUE, NOCOLOR, "%s", thighness);
          ymin++;
        }
        while(xline <= XVILLE-1){
          locate(xline, lignes->ville->y);
          colorPrint(BLUE, NOCOLOR, "%s", thighness);
          xline++;
        }
    }
    lignes = lignes->suivant;
   
  }
   l = l->suivant;
  }
}

int drawnode(struct centrale* node){
  int ycentrale = 3;
  int yville = 3;
  while(centrales != NULL && ycentrale <= LIGNES && yville <= LIGNES){
    printf("C%d", centrales->id);
    struct ligne* lignes = centrales->lignes;
    while(lignes != NULL){
      if(lignes->ville->y == -1){
        lignes->ville->y = yville;
        locate(XVILLE, yville);
        printf("%c%c",lignes->ville->name[0], lignes->ville->name[strlen(lignes->ville->name)-1]);
        yville++;
      }
      int xline = XCENTRALE+2;
      int ymin = yville > ycentrale ? ycentrale : yville;
      int ymax = yville < ycentrale ? ycentrale : yville;
      // printf("min %d max %d", ymin, ymax);
      // TODO Fix srand going too far
      // TODO color changing
      char* thighness = ".";
      int offset = rand() % XVILLE;
      if (lignes->puissance > 100)
        thighness = "o";
      if(lignes->puissance > 200)
        thighness = "O";
      if(lignes->ville->y == ycentrale){
        while(xline <= XVILLE-1){
          locate(xline, ycentrale);
          colorPrint(RED, NOCOLOR, thighness);
          xline++;
        }
      }
      else{
        while(xline <= offset){
          locate(xline, ycentrale);
          colorPrint(BLUE, NOCOLOR, "%s", thighness);
          xline++;
        }
        while(ymin <= ymax){
          locate(offset, ymin);
          colorPrint(BLUE, NOCOLOR, "%s", thighness);
          ymin++;
        }
        while(xline <= XVILLE-1){
          locate(xline, yville);
          colorPrint(BLUE, NOCOLOR, "%s", thighness);
          xline++;
        }
      }
    lignes = lignes->suivant;
    }
    centrales = centrales->suivant;
    ycentrale++;

    locate(XCENTRALE,ycentrale);
  }
}

void init(void){
  cls();
  int i = 0;
    for(int i =0; i < LIGNES;i++){
      locate(0,i);
      colorPrint(BLUE, NOCOLOR, "|");
      locate(COLONNES,i);
      colorPrint(BLUE, NOCOLOR, "|");
  }
}

int draw(void){
  init();
  if(scr.toolbar != -1){
    drawtoolbar(scr.toolbar);
  }
  if(scr.infobar != -1){
    drawpanel(scr.infobar);
  }
  if(scr.display == -1){    
    drawnetwork();
  }
  else{
    // drawnode(get_centrale(centrales, scr.display));
  }
  return 0;
}

struct func getfnc(char map){
  int index = 0;
  int len = sizeof(funcs) / sizeof(struct func);
  struct func empty = {
    "NONE",
    NULL,
    '\0'
  };
  while(index < len){
    if(funcs[index].mapped == map){      
      return funcs[index];
    }
    index++;
  }
  return empty;
}

void test(struct centrale* centrales, struct ville* villes){  
  set_p_ville(villes);
  add_ville(1, "tes1");
  add_ville(2, "tes2");
  add_ville(3, "tes3");
  add_ville(4, "tes4");
  add_centrale(&centrales, 1);
  add_centrale(&centrales, 2);
  add_centrale(&centrales, 3);
  add_centrale(&centrales, 4);
  add_ligne(get_centrale(centrales, 4), 120, get_ville(3));
  add_ligne(get_centrale(centrales, 1), 120, get_ville(1));
  add_ligne(get_centrale(centrales, 2), 120, get_ville(3));
  // add_ligne(get_centrale(centrales, 2), 120, get_ville(4));
}

int main(int argc, char* argv[]){
  // DO not remove, else random always return the same number
  srand(time(NULL));
  // Set the default values
  scr.display = -1;
  scr.infobar = -1;
  scr.input = -1;
  scr.toolbar = 1;
  centrales = (struct centrale*) malloc(sizeof(struct centrale));
  centrales->id = 0;
  centrales->lignes = NULL;
  centrales->prev = NULL;
  centrales->suivant = NULL;
  villes = (struct ville*) malloc(sizeof(struct ville));
  villes->name = "";
  villes->code = 0;
  villes->suivant = NULL;
  set_p_ville(villes);
  // load("save", villes, centrales);
  if(argc == 2){
    // Not working at the moment
    if(strcmp(argv[1], "-h") == 0){
      showHelp();
      return 0;
    }
    // TODO Think about other cases possible
    else{
      load(argv[1], villes, centrales);
    }
  }

  cls();
  hidecursor();
  saveDefaultColor();
  draw();
  while(1){
      char k = getkey();
      struct func f = getfnc(k);
      if(f.fun == NULL){
        showWarning("this key is not mapped");
        continue;
      }
      // Pray pointer is pointing to the right function
      f.fun();
  }
}
