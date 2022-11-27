#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIGNES 27
#define COLONNES 104

// #define PADDING "-"

void init(FILE* fp){
  int i = 0;
  while(i < COLONNES){
    fputs("\n", fp);
    i = i + 1;
    printf("test");
  }
}

int main(void){
  char* filename = "screen";
  printf("test");
  // FILE* fp = fopen(filename, "w");
  // if(fp != 0){
  //   printf("Issue ! \n");
  //   return 0;
  // }
  // // init(fp);
  // fclose(fp);
}