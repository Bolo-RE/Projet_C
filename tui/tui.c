#include "protocol.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define NOCOLOR -1
#define LIGNES 27
#define BORDER 1
#define COLONNES 104
#define OFFSET 2
#define PADDING "\033[22;34m|"
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
int baroffset = 0;
void draw(void);
int x,y;
char* getinput(void);
int drawtoolbar(int n);
int incrtoolbar(void);
int dincrtoolbar(void);
void permtoolbar(void);
void drawnetwork(void);
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
  // clear on return key
}


char* getinput(void){
  locate(0,27);
  int x = 0;
  // Should be enough
  char input[100];
  while(1){
    if(kbhit()){
      char k = getkey();
      if(k == KEY_ENTER){
        return strdup(input);
      }
      // numpad1 is backspace in Linux
      // TODO Check if this is the case for Windows
      else if(k == KEY_NUMPAD1){
        // TODO Implement check min
        input[strlen(input)-1 == 0 ? strlen(input-1) : 0] = '\0';
        locate(x, 27);
        setChar(' ');
        x  =  x == 0 ? x : x - 1;
      }
      else if ('A' <= k && k <= 'z'){
        if(x <= 100){
          showWarning("Ooops... Seems command you're typing is a bit too long");
        }
        strcat(input, &k);
        printf("%c", k);
        x++;
      }
      else{
        printf("%d\n", k);
      }
    }
  }
}

void init(void){
  cls();
  int i = 0;
  // drawtoolbar(1);
  if(BORDER){
    for(int i =0; i < LIGNES;i++){
      // TODO Add color
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
  hidecursor();
  saveDefaultColor();
  init();
  // colorPrint(RED, NOCOLOR, "test");
  // draw();
  while(1){
    if(kbhit()){
      char k = getkey();
      struct func f = getfnc(k);
      if(strcmp(f.name,"NONE") == 0){
        // TODO Call the showhelp function
        printf("non mapped");
        continue;
      }
      // Pray pointer is pointing to some function
      f.fun();
    }
  }
}