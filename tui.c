#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//to change for the real TUI
#define LIGNES 27
#define COLONNES 104

// Contains all the functions names usable in the toolbar
char* functionnalities[] = {"test", "test2", "test3"};
int test();
// Contains all the functions pointers to fonctionnalities from the toolbar
int (*functions[])() = {test};
struct pos{
  int x;
  int y;
};

struct toolbar{
  struct pos start;
  struct pos end;

    
};

struct viewer{
  struct pos start;
  struct pos end;
  
};
struct inputbar{
  struct pos start;
  struct pos end;
};

int test(){
  printf("test");
  return 0;
}

int main(void){
  functions[0]();
}