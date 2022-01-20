// mpiorisc, Piorischin, Maxim, 501015327, 8
// nbenavid, Benavides, Nicolas, 500944671, 7
// cuddin, Uddin, Choyon, 501031767, 6

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "list.h"
#define MAX 100 //General MAX
#define MAXARR 5 //Max for the card array
#define RED "\e[0;31m"
#define NC "\e[0m"

//Headers for functions
int genCardNum();
char* getFullNum (int num);
void update(List * call_list, int card_array[][MAXARR]);
void checkinput(int argnum, char* args[]);
int checkWin(int card_array[MAXARR][MAXARR]);
int checkColumnWin(int card_array[MAXARR][MAXARR]);
int checkRowWin(int card_array[MAXARR][MAXARR]);
int checkFourCornerWin(int card_array[MAXARR][MAXARR]);
void createCardArray(int card_array[MAXARR][MAXARR], char *filename);
void prettyPrint(List call_list, int card_array[MAXARR][MAXARR]);
void myExit(int num);
void validateFurther(int card_array[MAXARR][MAXARR]);

int main(int argc, char* argv[]) {
    // Initial Variables
    List call_list;
    int card_array[5][5];
    int won = 0;
    int seed;
    char user_input;

    init(&call_list);

    checkinput(argc, argv); //Check exit 1-3
    char *filename = argv[2];
    createCardArray(card_array, filename); //Create the card array from the text file
    validateFurther(card_array); //Validate Exit 4
    // FUnction to check dupes letters, etc
    seed = atoi(argv[1]); //set seed variable
    srand(seed); // Set Random Seed
    prettyPrint(call_list, card_array); //Print nicely
	do {
		scanf(" %c",&user_input); //Take user input, if q, exit
        if (user_input == 'q'){
            myExit(0);
        }
        system("clear"); //Clear screen
        update(&call_list, card_array); //Update function which adds a to calllist and marks the card
        prettyPrint(call_list, card_array); //Prints the card nicely
        if (checkWin(card_array) == 1)
        {
            printf("WINNER\n");
            break;
        }
	} 
	while(user_input!='q'); 

    return 0;
}

void checkinput(int argnum, char* args[]) { // Do all input checks
    char seednum[MAX];
    char filename[MAX];
    if (argnum != 3) { // Check exit 1, for enough args
        myExit(1);
    }
    strcpy(seednum, args[1]);
    strcpy(filename, args[2]); // Set variables to command line args
    for (int i = 0; seednum[i] != 0; i++) // Check exit 2
    {
        if (!isdigit(seednum[i])) //Check if not integer
            myExit(2);
    }
    FILE* file = fopen(filename, "r"); // Check exit 3
    if (file == NULL){ myExit(3);}
    fclose(file);

}

void createCardArray(int card_array[][MAXARR],  char *filename){
    FILE* card_file; 
    card_file = fopen(filename, "r"); //open file
    char line[MAX] = {0}; //Var holds the line array
    int line_count = 0; //Line counter
    int num_count = 0; //Number in line counter
    if (!card_file) //Check if file non existant
    {
        myExit(3);
    }
    while (fgets(line, MAX, card_file))
    {
        if (line_count > 4){ //Check if more than 5 lines
            myExit(4);
        }
        char * token = strtok(line, " ");
        while(token != NULL) { 
            if (num_count > 4){ //Check if more than 5 numbers
                myExit(4);
            }
            int intval = atoi(token); //Turn the value into an int
            card_array[num_count][line_count] = intval; //Set the card array location to the value
            // printf(" %s\n", token );
            token = strtok(NULL, " "); //Go to next token in line
            ++num_count; //inc num counter
        }
        if (num_count < 4){ //Check if less than 5 numbers
            myExit(4);
        
        }
        num_count = 0; //Reset num counter
        ++line_count; //increment line counter
        //printf("line[%i]: %s", ++line_count, line);
        
    }
    if (line_count < 4){ //Check if less than 5 lines
        myExit(4);
    }

    fclose(card_file); //Close file

}

int genCardNum(List call_list) { //Generate a random number between 01 and 75, if number exists, regenerate!

    int r = rand() % 75 + 1;
    if (length(call_list) > 73){ //Avoid all possible numbers generating (cant be theoretically reached because of win conditions, but still safe)
        printf("Maximum Numbers reached!");
        exit(5);
    }
    if (check(call_list, r)){ //Checks if number was already generated, if it was, re-call the function
        return genCardNum(call_list);
    }
    return r; //return random num
}

void update(List *call_list, int card_array[][MAXARR]){
    int rand_number = genCardNum(*call_list); //get random number
    add(call_list, rand_number); //add thenumber to the call list
    for (int i = 0; i < MAXARR; i++)
    {
        for (int j = 0; j < MAXARR; j++)
        {
            if (rand_number == card_array[i][j]){ //if the number in the 2d array
                int newval = card_array[i][j] + 100; // The way I "mark" the value is by adding 100 to it, which is detected when printing the board.
                card_array[i][j] = newval; //set the number
            }
        }
    }

}

void prettyPrint(List call_list, int card_array[MAXARR][MAXARR]) {
    char * full_num; //The number with prefix
    print(call_list); //Printing the call list
    printf("L   I   N   U   X\n");
    for (int i = 0; i < MAXARR; i++) //Iterate through the 2d array
    {
        for (int j = 0; j < MAXARR; j++)
        {
            full_num = getFullNum(card_array[j][i]); //Num with prefix
            printf("%s", full_num);
            free(full_num); //Free the allocated memory from getFullNum
        }
        putchar('\n');
    }
}

char* getFullNum (int num) { // Get number from the array, return number with marking, which is if its over 100 or 0.
    char *full_num = malloc(10); //Allocate memory for the number
    if (full_num == NULL){
        printf("MEMORY ISSUE GETFULLNUM");
    }
    if (num == 0){ //If the number is 0
        sprintf(full_num, "0%dm ", num); 
    }
    else if (num < 10) { //Number is less than 10, to print with a 0 in front
        sprintf(full_num, "0%d  ", num); 
    }
    else if (num > 109){// greater than 109 means it is marked and no 0 in front is needed
        sprintf(full_num, "%dm ", num - 100);
    }
    else if (num > 99){ //marked and 0 in front is needed.
        sprintf(full_num, "0%dm ", num - 100);
    }
    else{ //non marked number that doesnt need extra 0 in front
      sprintf(full_num, "%d  ", num);
    }
    return full_num; // Return pointer to full_num, is freed after use
}

int checkWin(int card_array[MAXARR][MAXARR]){   //Checks if any of the three win conditions have been flagged

    if (checkColumnWin(card_array) || checkRowWin(card_array) || checkFourCornerWin(card_array))
    {
        return 1;
    } 
    else
    {
        return 0;
    }
}

int checkFourCornerWin(int card_array[MAXARR][MAXARR]) //Checks the four corner numbers of the array to see if they are marked
{
    if (card_array[0][0] > 99 && card_array[0][4] > 99 && card_array[4][0] > 99 && card_array[4][4] > 99)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int checkRowWin(int card_array[MAXARR][MAXARR]) //Checks each row to see if each of its numbers have been marked
{

    for (int i =0; i < MAXARR; i++)
    {
        int count = 0;
        for (int j =0; j < MAXARR; j++)
        {
            if (card_array[j][i] > 99 || card_array[j][i] == 0)
            {
                count++;
            }
        }
        if (count == 5)
        {
            return 1;
        }
    }

    return 0;

}

int checkColumnWin(int card_array[MAXARR][MAXARR])  //Checks each column to see if each of its numbers have been marked
{

    for (int i =0; i < MAXARR; i++)
    {
        int count = 0;
        for (int j =0; j < MAXARR; j++)
        {
            if (card_array[i][j] > 99 || card_array[i][j] == 0)
            {
                count++;
            }
        }
        if (count == 5)
        {
            return 1;
        }
    }
    
    return 0;
}

void myExit(int num) { //Custom exit code messages
    if (num > 4){
        printf("Invalid Error Code");
    }
    if (num == 0){
        exit(0);
    }
    if (num == 1){
        fprintf(stderr, RED "[ERROR]"
               NC  ": Invalid Arguments Provided \n"
                   "You must provide 2 arguments \n");
        exit(1);
    }
    if (num == 2){
        fprintf(stderr, RED "[ERROR]"
               NC  ": Invalid Seed \n"
                   "Seed must be an integer \n");
        exit(2);
    }
    if (num == 3){
        fprintf(stderr, RED "[ERROR]"
               NC  ": Card file is not readable or doesn't exist \n");
        exit(3);
    }
    if (num == 4){
        fprintf(stderr, RED "[ERROR]"
               NC  ": Card file is not in proper format \n");
        exit(4);
    }
}

void validateFurther(int card_array[MAXARR][MAXARR]){//The exit 4 validations, which include proper 0 placement, number placement, and no duplicates
    List allnums; //List to add to when iterating through array, checking it every iteration to see if value already exists
    init(&allnums);
    char letters[6] = {'L', 'I', 'N', 'U', 'X', '\0'}; //Array to be referenced to check if proper letter
    for (int i = 0; i < MAXARR; i++)
    {
        for (int j = 0; j < MAXARR; j++)
        {
            if(card_array[j][i] == 0 && (i != 2 || j != 2)){ //Check if Zero out of place
                myExit(4);
            }
            if(i == 2 && j == 2 && card_array[j][i] != 0){ //check if middle is 0
                myExit(4);
            }
            else if (card_array[j][i] != 0){
                if (*getPrefix(card_array[j][i]) != letters[j]){// Check if column value matches the proper range it should be in
                    myExit(4);
                }
                if (check(allnums, card_array[j][i])){ // Check for duplicate
                    myExit(4);
                }
                add(&allnums, card_array[j][i]); //add to the allnums list
            }
        }
    }
}