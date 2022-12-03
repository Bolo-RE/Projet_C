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
struct func {
  char* name;
  int (*fun)();
  char mapped;
};

struct screen {
  int toolbar;  // If non 0 draw the toolbar at given offset
  int infobar;  // If non 0 call the infobar
  int display; // If non 0 show the view for given centrale ID
  int input;  // If 1 show input else no
};

// TODO Fix backspace formatting issue
// TODO Input handling (best result function)
// TODO Options read from backup file -> hard to do + not that useful -> garbage (can be)
// setup on running though
// TODO Fix keymap
// TODO Error code handling
// TODO Save screen state to be redrawn after printing message
static int baroffset = 0;
static struct ville* villes; 
static struct centrale* centrales;
static struct screen scr;
char* getinput(char* msg, int type);
int drawtoolbar(int n);
int incrtoolbar(void);
int dincrtoolbar(void);
int getinfo(void);
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
void draw(void);
struct func getfnc(char map);


// Toolbar is using the numbers to select the function needed
// User can also define keybindings to some functions
// Color of lines are defined by the power they contain
// 3 levels : . x X for lines ; with != colors
// TODO Complete this
struct func funcs[] = {
  {"function1",incrtoolbar,'a'}, 
  {"function2",dincrtoolbar, 'b'}, 
  {"test3", NULL, 'c'}  
};


int incrtoolbar(void){
  baroffset++;
  drawtoolbar(baroffset);
  return 0;
}
int dincrtoolbar(void){
  baroffset--;
  drawtoolbar(baroffset);
  return 0;
}


// Find out how to get integer inputs
int createVille(void){
  char* name = getinput("Nom de la ville :", 0);
  int id = getinput("Id :", 1);
  add_ville(id, name);
  return 0;
}

int createCentrale(void){
  int id = getinput("Id :", 1);
  add_centrale(&centrales, id);
  return 0;
}

int createLigne(void){
  int cid = getinput("Id de la centrale :", 1);
  int vid = getinput("Id de la ville : ", 1);
  int power = getinput("Puissance :", 1);
  add_ligne(get_centrale(centrales, cid), power, get_ville(vid));
  return 0;
}


// TODO Setup resize ?
int drawtoolbar(int n){
  locate(0,scr.toolbar);
  int i = n;
  int size = COLONNES;
  // get the number of functions global
  int len = sizeof(funcs) / sizeof(struct func);
  char toolbar[COLONNES];
  // Change the pointer to functions for the 0-9 keys
  while(1){
    int j = i % len;
    if(funcs[j-1].name == NULL)
      continue;
    size = size - strlen(funcs[j-1].name) - 3;
    if (size < 0){
      break;
    }
    sprintf(toolbar, "%s | %s",toolbar, funcs[j].name);
    i++;
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
char* getinput(char* msg, int type){
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
      else if ('A' <= k && k <= 'z'){
        if(x >= 100){
          showWarning("Ooops... Seems command you're typing is a bit too long");
          // TODO Do something about it
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

void drawpanel(char* data[]){
  // Check if all the datas can be displayed if not inform user 
  // Draw everything on the right of the screen
}

//TODO Moving around could be implemented but not mandatory for the moment
void drawnetwork(struct centrale* centrales){
  locate(XCENTRALE,1);
  int ycentrale = 1;
  int yville = 1;
  //TODO Take in account that may be needed at the end of file -> just inform user if this is the case
  //If time try to implement something looking like moving around
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
      // TODO Draw line
      int xline = XCENTRALE+2;
      int yline = yville < ycentrale ? yville : ycentrale;
      int ymax = yville > ycentrale ? yville : ycentrale;
      // If on the same line; just have to draw straight line 
      // TODO Find the right number of cells to fill
      if(lignes->ville->y == ycentrale){
        while(xline <= XVILLE-1){
          locate(xline, ycentrale);
          setChar('.');
          xline++;
        }
      }
      else{
        int offset = rand() % COLONNES;
        while(xline <= offset){
          locate(xline, yville);
          colorPrint(RED, WHITE, ".");
          xline++;
        }
        while(yline <= ymax){
          locate(xline, yline);
          colorPrint(RED, WHITE, ".");
          yline++;
        }
        while(xline <= XVILLE-1){
          locate(xline, yline);
          colorPrint(RED, WHITE, ".");
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

void draw(void){
  init();
  drawtoolbar(1);
  drawnetwork(centrales);
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