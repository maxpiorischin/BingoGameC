#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#define MAX 100

void init(List *L) { //Initialize list
   L->length=0;
   L->head=NULL;
}
int length(List L) { //Get length of the list
  return L.length;
}
void add(List *L, int i) { //Add to the END of a list
  if (L->length < MAX) {
    NodeType *new;
    new=(NodeType *) malloc(sizeof(NodeType)); //get memoryfor new node
    if (new == NULL) { //Check to see if memory issues
      printf("Memory Issue");
    }
    else {
      new->num = i; //initialize new node values
      new->next = NULL;
      if (L->head == NULL){ //check if list is empty
        L->head = new; //make new node the head if empty
        L->length++;
        return;
      }
      NodeType *last = L->head; // last node will be the last node in the list
      while (last->next != NULL) {
        last = last->next; //traverse list until last node is reached
      }
      last->next = new; //Set the last to new node
      L->length++; //add 1 to the length
    }
  }
  else { //if over the max
    printf("Error adding: Too many elements in list");
  }
}
void print(List L) { //Print the list
 int i; NodeType *p; char*full_num;
 printf("CallList: ");
 for (i=0,p=L.head; i<L.length; i++,p=p->next) { //Iterate through all the heads and print their value
   printf("%c%i ", *getPrefix(p->num), p->num);
 }
 putchar('\n');
}

char* getPrefix (int num) { // Get the letter prefix to assist when printing list.
    char * letter;
    if (num == 0){
        printf("Error, call number cant be 0");
    }
    else if (num < 16) {
        letter = "L";
    }
    else if (num > 15 && num < 31){
        letter = "I";
    }
    else if (num > 30 && num < 46){
        letter = "N";
    }
    else if (num > 45 && num < 61){
        letter = "U";
    }
    else if (num > 60 && num < 76){
        letter = "X";
    }
    else { //Invalid number, contains no letter
      letter = "Z"; // INVALID
    }
    return letter;
}

int check (List L, int num) { // Check if value in list
  int i; NodeType *p;
  for (i=0,p=L.head; i<L.length; i++,p=p->next) { //Iterate through the values
    if (p->num == num) { //If the node value is the num argument, then return 1 for true
      return 1;
    }
 }
 return 0; //else return 0 for false
}