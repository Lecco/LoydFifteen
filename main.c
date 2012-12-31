#include <stdio.h>
#include <stdlib.h>

#define INITIAL_BOARD_SIZE 1
#define NO_PARAMETER -2


/***************** DATA STRUCTURES ***************************/
typedef struct 
{
        int tilesPosition[INITIAL_BOARD_SIZE][INITIAL_BOARD_SIZE];
} GameState;

/**************** FUNCTION PROTOTYPES ********************/
int canMoveLeft(GameState* thisState);
int canMoveRight(GameState* thisState);
int canMoveUp(GameState* thisState);
int canMoveDown(GameState* thisState);


/********************** FUNCTION DEFINITIONS *****************/
/**
   Returns true (1) if in current game tree state it is possible
   to move empty space left
*/
int canMoveLeft(GameState* thisState)
{
     return 1;
}

/**
   Returns true (1) if in current game tree state it is possible
   to move empty space right
*/
int canMoveRight(GameState* thisState)
{
    return 1;
}

/**
   Returns true (1) if in current game tree state it is possible
   to move empty space up
*/
int canMoveUp(GameState* thisState)
{
    return 1;
}

/**
   Returns true (1) if in current game tree state it is possible
   to move empty space down
*/
int canMoveDown(GameState* thisState)
{
    return 1;
}


/********************** MAIN PROGRAM********************/

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
             printf("Nebyl zadan parametr.\n");
             return NO_PARAMETER;
    }
    
    // copy argv[1] to variable initState
    char initState[strlen(argv[1])];
    strcpy(initState, argv[1]);
    
    
    int rows = 0;
    int cols = 0;
    char * pch;
    printf ("Splitting string \"%s\" into tokens:\n", initState);
    pch = strtok(initState,";");
    while (pch != NULL)
    {
          printf ("%s\n",pch);
          pch = strtok(NULL, ";");
    }
    
    
    
    printf("%s\n", initState);
    
    system("PAUSE");
    return 0;
}
