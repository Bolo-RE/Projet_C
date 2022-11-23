#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//to change for the real TUI
#define LIGNES 27
#define COLONNES 104

char* screen[LIGNES][COLONNES];
// Those two arrays needs to be synchronized -> find a way to do that
// Contains all the functions names usable in the toolbar
char* functionnalities[] = {"test", "test2", "test3", "NULL"};
// Control which part of the bar is shown
int toolbar_offset = 0;
// Character used to equilibrate the size of the parts
char padding = '-';
int test();
// Contains all the functions pointers to fonctionnalities from the toolbar
int (*functions[])() = {test};
void drawscreen(char* screen[LIGNES][COLONNES]);
// TODO Find a way to make it less handmade
int parts[3] = {LIGNES-26, LIGNES-25, LIGNES};

int test(){
  printf("test");
  return 0;
}

// Not working for the moment
// This need to be call after every modification of padding
char* toolbar_compose(int padding){
  size_t toolbar_len = 0;
  int index = 0;
  char* toolbar = "|";
  // while(toolbar_len <= LIGNES && strcmp(functionnalities[index], "NULL")!=0){
  while(toolbar_len < 3){
    // TODO Implement checking for the value of functionnalities
    printf("%d ", index);
    // toolbar_len+=strlen(functionnalities[index]);
    strcat(toolbar, strcat(functionnalities[index], "|"));
    index++;
  }  
  printf("%s", toolbar);
  return toolbar;
}



// TODO Choose which part should be refreshed
void drawparts(char* screen[LIGNES][COLONNES], int parts[3]){
  if(parts[0] > 0){
    for(int i = 0; i < parts[0]; i++){
      
    }

  }  
  drawscreen(screen);
}

void drawscreen(char* screen[LIGNES][COLONNES]){
  for(int i = 0; i < LIGNES; i++){
    for(int j = 0; j < COLONNES; j++){
      printf("%s\n", screen[i][j]);
    }
  }
}

void printinput(char* text, char* screen[LIGNES][COLONNES], int parts[3]){
  int size = parts[3];
  if(size == -1)
    size = LIGNES - 1;
  // Handle UTF-8
  while(1){
    // TODO To change to work in Windows -> getch
    char c = getc(stdin);
    if(c == '\n')
      break;
    if(c == '\t')
      break;
    if(c < 'a' && c > 'Z'){
      printf("Unknown char\n");
      continue;
    }
    fflush(stdin);
    printf("%c\n", c);
    // puts("test");
  }
}

void clear(char* screen[LIGNES][COLONNES]){
  for(int i = 0;i < LIGNES; i++){
    for(int j = 0; j < COLONNES; j++){
      screen[i][j]  = " ";
    }
    screen[i][COLONNES-1] = "\n";
    printf("%s", screen[i][COLONNES-1]);
  }
}



int main(void){
  // toolbar_compose(0);  
  printinput(NULL, screen, parts);
}