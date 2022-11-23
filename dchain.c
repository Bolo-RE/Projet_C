// Philemon GOUSSARD
// Fri Oct  7 02:04:50 PM CEST 2022
// Sun Oct  9 10:02:54 PM CEST 2022
// Wed Oct 12 03:12:10 PM CEST 2022
// TODO Rewrite this thing properly


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct DoubleChain {
  int value;
  struct DoubleChain* next;
  struct DoubleChain* prev;
};


struct DoubleChain* insert(struct DoubleChain* pointer, int value){
  struct DoubleChain* chain = malloc (sizeof(struct DoubleChain));
  pointer->value = value;
  chain->prev = pointer;
  pointer->next = chain;
  chain->value = -0xfff; // This value is mockup for to avoid 0
  return chain;
}

void free_chain(struct DoubleChain* pointer){
  while(pointer != NULL){
    struct DoubleChain* buf = pointer;
    printf("value is %d \n", pointer->value);
    pointer = pointer->prev;
    free(buf);
  }
}


void resetpos(struct DoubleChain* pointer, int pos){
  if (pos == -1){
  while(pointer->next != NULL)
    pointer = pointer->next;
  }
  else{
  while(pointer->prev != NULL)
    pointer = pointer->prev;
  while(pos-- && pointer->next != NULL){
    pointer = pointer->next;
    }
  }
}
void setpos(struct DoubleChain** pointer, int pos){
  if (pos == -1){
  while((*pointer)->next != NULL)
    *pointer = (*pointer)->next;
  }
  else{
  while((*pointer)->prev != NULL)
    *pointer = (*pointer)->prev;
  while(pos-- && (*pointer)->next != NULL){
    *pointer = (*pointer)->next;
    }
  }
}



struct DoubleChain* search(struct DoubleChain* list);
struct DoubleChain* replace(struct DoubleChain* list);
struct DoubleChain* rm(struct DoubleChain* list);
struct DoubleChain* cut(struct DoubleChain* list);
struct DoubleChain* copy(struct DoubleChain* list);

int main(void){
  // List used to choose function  to exec from menu ; 
  struct DoubleChain* (*choices[5])() = {search, replace, rm, cut, copy};
  struct DoubleChain* first = malloc(sizeof(struct DoubleChain));
  char c;
  int option;
  while((c = getchar()) != '0'){
    if(c == '\n')
      continue;
    first = insert(first, atoi(&c));
  }
  printf("Please do something with your chain : \n 1 - Search value \n 2 - Replace value \n 3 - Remove node \n 4 - Cut part of list \n 5 - Copy part of list \n 0 - quit \n");
  while(1){
    printf("Choose new thing to do : ");
    scanf("%d", &option);
    if(option == 0){
      break;
      }
    if(option >= 1 && option <= 5)
    (*choices[--option])(first); // TODO Failsafe wierd side effect with strings
    else
      printf("this option doesnt exist (yet)\n");
  }
  resetpos(first, -1);
  free_chain(first);
}

struct DoubleChain* search(struct DoubleChain* list){
  int value =  0;
  int matches[20] = {0}; // init array at 0 value TODO Handle case if more than 20 matches
  int index = 0;
  int size = 0;
  printf("Input the value you're searching for : ");
  scanf("%d", &value);
  setpos(&list, 0);
  while(list->next != NULL){
    if(list->value == value){
      matches[index++] = size;
    }
    list = list->next;
    size++;
  }
  for(int i = 0; i<=index;i++){
    int r_pos = size-matches[i];
    printf("size : %d\n", size);
    printf("Found match at offset %d : %d \n", matches[i], r_pos);
  }
  return 0;
}
struct DoubleChain* replace(struct DoubleChain* list){
  int value;
  int nvalue;
  printf("Input the value you're replacing : ");
  scanf("%d", &value);
  printf("Input the new value : ");
  scanf("%d", &nvalue);
  setpos(&list, -1);
  while(list->prev != NULL){
    if(list->value == value){
      list->value = nvalue;
    }
    list=list->prev;
  }
  return list;
}
// struct DoubleChain* rm(struct DoubleChain* list){
//   int a;
//   int b;
//   struct DoubleChain* poubelle = list;
//   struct DoubleChain* poubelle2 = list;
//   printf("Input the first offset from poubelle : ");
//   scanf("%d", &a);
//   printf("Input the other offset : ");
//   scanf("%d", &b);
//   if(a<=0)
//     return list;
//   setpos(&poubelle, a-1);
//   setpos(&poubelle2, b+1);
//   poubelle->next = poubelle2;
//   poubelle2->prev = poubelle;
//   b = b -a ;
//   if(b < 0){
//     printf("b must be greater than a \n");
//     free(poubelle);
//     return list;
//   }
//   while(b-- && poubelle != NULL){
// ;
//   }
//   printf("Poubelle value : \n");

//   return list;
// }


struct DoubleChain* rm(struct DoubleChain* list){
  int a,b;
  struct DoubleChain* buffer = list;
  struct DoubleChain* poubelle = malloc(sizeof(struct DoubleChain));
  printf("Input the first offset from poubelle :  ");
  scanf("%d", &a); 
  printf("Input the second offset from poubelle :  ");
  scanf("%d", &b); 
  if(b-a < 0){ // TODO Check a < 0
    printf("b must be greater than a");
  }
  setpos(&buffer, a);
  struct DoubleChain* prev = buffer->prev;
  int d = b - a;
  while(d-- && buffer->next != NULL){
    printf("node value is %d\n", buffer->value);
    printf("d value is %d\n", d);
    poubelle->next = buffer;
    buffer->prev = poubelle;
    poubelle = buffer;
    buffer = buffer->next;
  }
  // TODO Find a cleaner way to handle this side effect
  if(a == 0){
    buffer->prev = NULL;
  }else{
  buffer->prev = prev;
  prev->next = buffer;
   }
  free_chain(poubelle);
  return list;
}


struct DoubleChain* copy(struct DoubleChain* list){
  int a,b,c;
  struct DoubleChain* buffer = list;
  struct DoubleChain* paste = list;
  struct DoubleChain* poubelle = malloc(sizeof(struct DoubleChain));
  printf("Input the first offset from poubelle :  ");
  scanf("%d", &a); 
  printf("Input the second offset from poubelle :  ");
  scanf("%d", &b); 
  printf("Input the paste position :  ");
  scanf("%d", &c); 
  if(b-a < 0){ // TODO Check a < 0
    printf("b must be greater than a");
  }
  setpos(&buffer, a);
  struct DoubleChain* prev = buffer->prev;
  int d = b - a;
  while(d-- && buffer->next != NULL){
    printf("node value is %d\n", buffer->value);
    printf("d value is %d\n", d);
    poubelle->next = buffer;
    buffer->prev = poubelle;
    poubelle = buffer;
    buffer = buffer->next;
  }
  // TODO Find a cleaner way to handle this side effect
  if(a == 0){
    buffer->prev = NULL;
  }else{
  buffer->prev = prev;
  prev->next = buffer;
   }
  setpos(&paste,c);
  setpos(&poubelle, 0);
  paste->next = poubelle;
  poubelle->prev= paste;
  setpos(&paste,c+d);
  setpos(&poubelle, -1);
  paste->prev = poubelle;
  poubelle->next = paste;
  free_chain(list);
  return 0;
}

struct DoubleChain* cut(struct DoubleChain* list){
  int a,b;
  struct DoubleChain* buffer = list;
  struct DoubleChain* poubelle = malloc(sizeof(struct DoubleChain));
  printf("Input the first offset from poubelle :  ");
  scanf("%d", &a); 
  printf("Input the second offset from poubelle :  ");
  scanf("%d", &b); 
  if(b-a < 0){ // TODO Check a < 0
    printf("b must be greater than a");
  }
  setpos(&buffer, a);
  struct DoubleChain* prev = buffer->prev;
  int d = b - a;
  while(d-- && buffer->next != NULL){
    printf("node value is %d\n", buffer->value);
    printf("d value is %d\n", d);
    poubelle->next = buffer;
    buffer->prev = poubelle;
    poubelle = buffer;
    buffer = buffer->next;
  }
  // TODO Find a cleaner way to handle this side effect
  if(a == 0){
    buffer->prev = NULL;
  }else{
  buffer->prev = prev;
  prev->next = buffer;
   }
  free_chain(poubelle);
  return list;
}

