#include "protocol.h"
#include "backend.c"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define NOCOLOR -1
#define LIGNES 27
#define COLONNES 104
#define OFFSET 2
#define BORDER "\033[22;34m|"
#define XCENTRALE 4
#define XVILLE 80
#define XSIDEBAR 81
#define OINDEX 19
// TODO Put that in tmp files in Windows version
#define DATAPANEL "view.dat"

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
// TODO Fix backspace formatting issue
// TODO Create enum containing the error
// TODO Input handling (best result function)
// TODO Options read from backup file -> hard to do + not that useful -> garbage (can be)
// setup on running though
// TODO Error code handling
// TODO Setup resize ?
static int baroffset = 0;
static int paneloffset = 0;
static struct ville* villes; 
static struct centrale* centrales;
static struct screen scr;
char* getinput(char* msg, char alphabet[]);
int drawtoolbar(int n);
int incrtoolbar(void);
int dincrtoolbar(void);
int incrpanel(void);
int dincrpanel(void);
int getinfoVille(void);
int getinfoCentrale(void);
int getpower(void);
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
void drawnetwork(struct centrale* centrales);
void init(void);
int draw(void);
void drawpanel(int offset);
struct func getfnc(char map);


// Toolbar is using the numbers to select the function needed
// User can also define keybindings to some functions
struct func funcs[] = {
  {NULL,incrtoolbar,'a'}, 
  {NULL,dincrtoolbar, 'b'}, 
  {"infoVille", getinfoVille, 'c'},
  {"infoCentrale", getinfoCentrale, 'd'},
  {"getPower", getpower, 'e'},
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
  {NULL, NULL, '1'},
  {NULL, NULL, '2'},
  {NULL, NULL, '3'},
  {NULL, NULL, '4'},
  {NULL, NULL, '5'},
  {NULL, NULL, '6'},
  {NULL, NULL, '7'},
  {NULL, NULL, '8'},
  {NULL, NULL, '9'},
  {NULL, NULL, '0'},
 
};


int incrtoolbar(void){
  baroffset++;
  drawtoolbar(baroffset);
  return 0;
}
int dincrtoolbar(void){
  // TODO 0 check
  baroffset--;
  drawtoolbar(baroffset);
  return 0;
}

int toggleToolbar(void){
  scr.toolbar = -1;
  draw();
  return 0;
}


// Find out how to get integer inputs
int createVille(void){
  char* name = getinput("Nom de la ville :", alphabet);
  int id = atoi(getinput("Id :",num));
  // TODO Check for error
  add_ville(id, name);
  draw();
  return 0;
}

int createCentrale(void){
  int id = atoi(getinput("Id :",num));
  add_centrale(&centrales, id);
  draw();
  return 0;
}

int createLigne(void){
  int cid = atoi(getinput("Id de la centrale :", num));
  int vid = atoi(getinput("Id de la ville : ",num));
  int power = atoi(getinput("Puissance :",num));
  add_ligne(get_centrale(centrales, cid), power, get_ville(vid));
  draw();
  return 0;
}

int loadFile(void){
  char* filename = getinput("Nom du fichier : ", alphabet);
  load(filename, villes, centrales);
  draw();
  return 0;
}

int saveFile(void){
  char* filename = getinput("Nom du fichier : ", alphabet);
  save(centrales, filename);
  return 0;
}

//TODO Possibility to add using the name
int getinfoVille(void){
  int id = atoi(getinput("Nom de la ville : ", alphabet));
  struct ville* sel = get_ville(id);
  FILE* fp  = fopen(DATAPANEL, "w");
  // fprintf(fp, "%s\n%d\n%d", sel->name, sel->code, getpower(sel->code));
  draw();
  return 0;
}

int getinfoCentrale(void){
  int id = atoi(getinput("Id centrale : ", num));
  draw();
  return 0;
}

int listVille(void){
  // Check if enough if not setup keys to move; should do that for this in general
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
int drawtoolbar(int n){
  locate(0,scr.toolbar);
  int i = n;
  int k = OINDEX;
  int size = COLONNES;
  int len = sizeof(funcs) / sizeof(struct func);
  char toolbar[COLONNES];
  while(1){
    int j = i % len;
    if(funcs[j-1].name == NULL)
      continue;
    size = size - strlen(funcs[j-1].name) - 3;
    if (size < 0){
      break;
    }
    funcs[k].fun = funcs[j-1].fun;
    sprintf(toolbar, "%s | %s",toolbar, funcs[j].name);
    i++;
    k++;
  }
  printf("%s | ",toolbar);
  return 0;
}

// TODO Should return the best match in whole name functions
char* getfuncname(char* part){
  int index = 0;
  // TODO
  return funcs[index].name;
  return NULL;
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

// TODO Fix under Windows
// TODO Add the accepted character and check against them in windows
char* getinput(char* msg, char alphabet[]){
  locate(0,LIGNES-1);
  puts(msg);
  locate(0,LIGNES);
  int x = 0;
  // Should be enough
  // TODO Fix the fact that keystrokes are evaluated one loop after
  char input[100];
  while(1){
    // Keeping it maybe useful in Windows but crashing everything in Linux
    // if(kbhit()){
      char k = getkey();
      if(k == KEY_ENTER){
        return strdup(input);
      }
      // numpad1 is backspace in Linux
      // TODO Check if this is the case for Windows
      // This needs to be implemented under Windows;; too much !=
      else if(k == KEY_NUMPAD1){
        //Remove a char
        input[strlen(input)-1] = 0;
        locate(x, 27);
        setChar(' ');
        x  =  x == 0 ? x : x - 1;
      }
      // Include numbers
      else if (memchr(alphabet, k, sizeof(alphabet))){
        if(x >= 100){
          showWarning("Ooops... Seems command you're typing is a bit too long");
          memset(input, '0', sizeof(char)*100);
          x = 0;
        }
        strcat(input, &k);
        locate(x, 27);
        setChar(k);
        x++;
      }
    }
}

void showhelp(){
  showWarning("Please refer to the manual at address : https://github.com/Bolo-RE/Projet_C/tree/dev-linux");
}
// Should it be replaced by static value ?
// Assume that newline chars are placed
//TODO Formatting
//TODO Setup offset
void drawpanel(int i){
  int y = 0;
  char s[100];
  int j  = 0;
  FILE* fp = fopen(DATAPANEL, "r");
  while(fgets(s, sizeof s, fp)!=NULL){
    if(j <= i){
      j++;
      continue;
    }
    locate(XSIDEBAR, y);
    if(strlen(s) >= COLONNES){
      printf("toolong");
    }else{
      printf("%s", s); 
    }
    y++;
  }
  fclose(fp);
}

//TODO Moving around could be implemented but not mandatory for the moment
//TODO Take in account that may be needed at the end of file -> just inform user if this is the case
void drawnetwork(struct centrale* centrales){
  locate(XCENTRALE,1);
  int ycentrale = 1;
  int yville = 1;
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
      int yline = yville < ycentrale ? yville : ycentrale;
      int ymax = yville > ycentrale ? yville : ycentrale;
      // TODO Fix srand going too far
      // TODO color changing
      char* thighness = ".";
      int offset = rand() % COLONNES;
      if (lignes->puissance > 100)
        thighness = "o";
      if(lignes->puissance > 200)
        thighness = "O";
      if(lignes->ville->y == ycentrale){
        while(xline <= XVILLE-1){
          locate(xline, ycentrale);
          colorPrint(RED, WHITE, thighness);
          xline++;
        }
      }
      else{
        while(xline <= offset){
          locate(xline, yville);
          colorPrint(RED, WHITE, ".");
          xline++;
        }
        while(yline <= ymax){
          locate(xline, yline);
          colorPrint(RED, WHITE, thighness);
          yline++;
        }
        while(xline <= XVILLE-1){
          locate(xline, yline);
          colorPrint(RED, WHITE, thighness);
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
      printXY(0,i, BORDER);
      printXY(COLONNES,i, BORDER);
  }
}

int draw(void){
  init();
  if(scr.toolbar != -1){
    drawtoolbar(scr.toolbar);
  }
  if(scr.display != -1){    
    drawnetwork(centrales);
  }
  else{
    drawnetwork(get_centrale(centrales, scr.display));
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
  set_p_ville((unsigned long)villes);
  add_ville(1, "test");
  add_ville(2, "test");
  add_ville(3, "test");
  add_ville(4, "test");
  add_centrale(&centrales, 1);
  add_centrale(&centrales, 2);
  add_centrale(&centrales, 3);
  add_centrale(&centrales, 4);
  add_ligne(get_centrale(centrales, 1), 120, get_ville(1));
  add_ligne(get_centrale(centrales, 2), 120, get_ville(4));
  add_ligne(get_centrale(centrales, 3), 120, get_ville(3));
  add_ligne(get_centrale(centrales, 2), 120, get_ville(3));
}

int main(int argc, char* argv[]){
  // DO not remove, else random always return the same number
  srand(time(NULL));
  // Set the default values
  scr.display = 1;
  scr.infobar = 0;
  scr.input = 0;
  scr.toolbar = 1;
  centrales = malloc(sizeof(struct centrale));
  villes = malloc(sizeof(struct ville));
  // test(centrales, villes);
  if(argc == 2){
    if(strcmp(argv[1], "-h") == 0){
      //TODO Print help
    }
    // TODO Think about other cases possible
    else{
      load(argv[1], villes, centrales);
    }
  }
  hidecursor();
  saveDefaultColor();
  // init();
  // cls();
  // draw();
  // draw();
  // while(1){
  //   if(kbhit()){
  //     char k = getkey();
  //     struct func f = getfnc(k);
  //     if(strcmp(f.name,"NONE") == 0){
  //       // TODO Call the showhelp function
  //       printf("non mapped");
  //       continue;
  //     }
  //     // Pray pointer is pointing to some function
  //     f.fun();
//   }
//
}