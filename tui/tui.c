#include "protocol.h"
#include "backend.c"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define NOCOLOR -1
#define LIGNES 27
#define BORDER 1
#define COLONNES 104
#define OFFSET 2
#define PADDING "\033[22;34m|"
#define XCENTRALE 4
#define XVILLE 80
struct func {
  char* name;
  int (*fun)();
  char mapped;
};

// TODO Border management
// TODO Setup toolbar drawing
// TODO Fix backspace formatting issue
// TODO Input handling (best result function)
// TODO Options read from backup file
// TODO Fix keymap
// TODO View everything
// TODO View one particular node of network
// TODO Error code handling
// TODO Log clearing at starting
int baroffset = 0;
void draw(void);
int x,y;
char* getinput(void);
int drawtoolbar(int n);
int incrtoolbar(void);
int dincrtoolbar(void);
void permtoolbar(void);
// void drawnetwork(struct centrale*);
int test1(void);
int test2(void);
int test3(void);

struct func funcs[] = {{"function1",incrtoolbar,'a'}, {"function2",dincrtoolbar, 'b'}, {"test3", test3, 'c'}};
char* data[] = {"test1", "test2", "test3"};
#define SFUNC sizeof(funcs) / sizeof(struct func)


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

int test1(void){
  printf("Test 1\n");
  return 0;
}

int test2(void){
  printf("Test 2");
  return 0;
}

int test3(void){
  printf("Test 3\n");
  return 0;
}

void clearline(int x, int y){
  locate(x,y);
  char clear[COLONNES];
  memset(clear, '-', COLONNES * sizeof(char));
  setString(clear);
}

int drawtoolbar(int n){
  // TODO Choose toolbar position
  locate(0,0);
  int i = n;
  int size = COLONNES;
  // get the number of functions global
  int len = sizeof(funcs) / sizeof(struct func);
  char toolbar[COLONNES];
  // TODO Setup all checks
  while(1){
    int j = i % len;
    // Adding the [space]|[space] 
    // TODO Handle the sigv -> verify params exists
    size = size - strlen(funcs[j-1].name) - 3;
    if (size < 0){
      break;
    }
    sprintf(toolbar, "%s | %s",toolbar, funcs[j].name);
    i++;
  }
  // clearline(0,0);
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
  // Show message on the middle of the screen; need to save prev state show the message and 
  locate(LIGNES/2, COLONNES/2);
  anykey(message);
  cls();
  // clear on return key
}


char* getinput(void){
  locate(0,27);
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
        }
        strcat(input, &k);
        locate(x, 27);
        setChar(k);
        x++;
      }
      // else{
       // locate(0,0);
        // printf("%d\n", k);
        // locate(27,x);
      // }
    }
  // }
}


//TODO Moving around could be implemented but not mandatory for the moment
//TODO Choose the id to draw 
void drawnetwork(struct centrale* centrales){
  locate(XCENTRALE,1);
  int ycentrale = 1;
  int yville = 1;
  //TODO Take in account that may be needed at the end of file
  while(centrales != NULL && ycentrale <= LIGNES && yville <= LIGNES){
    setChar('C');
    struct ligne* lignes = centrales->lignes;
    while(lignes != NULL){
      if(lignes->ville->y == -1){
        lignes->ville->y = yville;
        locate(XVILLE, yville);
        setChar(lignes->ville->name[0]);
        yville++;
      }
      lignes = lignes->suivant;
    }
    centrales = centrales->suivant;
    locate(XCENTRALE,ycentrale);
    ycentrale++;
  }
  // Blue is for centrale; 
  // White is for the town;
  // Trying to use the most different colors for the link
}

// void drawnode(struct centrale* centrales){
//   // Blue is for centrale; 
//   // White is for the town;
//   // Trying to use the most different colors for the link
// }

void init(void){
  cls();
  int i = 0;
  // drawtoolbar(1);
  if(BORDER){
    for(int i =0; i < LIGNES;i++){
      printXY(0,i, PADDING);
      printXY(COLONNES,i, PADDING);
    }
  }
}

void draw(void){
  // init();
  drawtoolbar(1);
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

int main(void){

  logmsg("---------------------------------------------");
  struct centrale* centrales = malloc(sizeof(struct centrale));
  struct ville* villes = malloc(sizeof(struct ville));
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
  // add_ligne(get_centrale(centrales, 1), 120, get_ville(2));
  hidecursor();
  saveDefaultColor();
  // init();
  cls();
  drawnetwork(centrales);
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
  // }
}