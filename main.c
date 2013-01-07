#include <stdio.h>
#include <stdlib.h>


#include "constants.h"
#include "game_states.h"
#include "pq.h"

int rows = 0;

/**
Test inputs:
    "7 2 6 5; 1 3 4 8; 10 11 9 12; 13 14 15 0"
    "1 2 3 4; 5 6 7 8; 7 10 11 12; 13 14 0 15"
*/


/**
    Creates new game state (copies values from argument)
*/
struct GameState *createNewState(struct GameState *queue)
{
    struct GameState *s;
    int i, j;
    
    s = (struct GameState *) malloc(sizeof(struct GameState) + sizeof(int *) * sizeof(int *) * rows);
    s->distance = queue->distance + 1;
    
    s->tilesPosition = (int **)malloc(sizeof(int *) * sizeof(int *) * rows);
    int length = sizeof(int *);
    for (i = 0; i < length; i++)
    {
        s->tilesPosition[i] = (int *)malloc(length * sizeof(int *) * rows);
        
        for (j = 0; j < length; j++)
        {
            s->tilesPosition[i][j] = queue->tilesPosition[i][j];
        }
    }
    
    s->prev = (struct GameState *) malloc(sizeof(struct GameState *));
    s->prev = queue;
    s->next = (struct GameState *) malloc(sizeof(struct GameState *));
    
    return s; 
}

/**
    Searches for solution
*/
void solveFifteen()
{
    struct GameState *queue;
    int i = 0;
    
    while (notEmptyPQ())
    {        
        queue = getQueueTop();
        
        if (queue->manhattanDistance == 0)
        {
            printMatrix(queue->tilesPosition);
            printf("Nalezena cesta!\n");   
            system("PAUSE"); 
        }
        else
        {
            //printMatrix(queue->tilesPosition);
            int lastMove = getLastMove(queue);
            if (canMoveLeft(queue) && lastMove != MOVE_RIGHT)
            {
                struct GameState *s = createNewState(queue);
                moveLeft(s);
                s->manhattanDistance = getManhattanDistance(*s);
                insertPQ(s);
            }
            if (canMoveRight(queue) && lastMove != MOVE_LEFT)
            {
                struct GameState *s = createNewState(queue);
                moveRight(s);
                s->manhattanDistance = getManhattanDistance(*s);
                insertPQ(s);
            }
            if (canMoveUp(queue) && lastMove != MOVE_DOWN)
            {
                struct GameState *s = createNewState(queue);
                moveUp(s);
                s->manhattanDistance = getManhattanDistance(*s);
                insertPQ(s);
            }
            if (canMoveDown(queue) && lastMove != MOVE_UP)
            {
                struct GameState *s = createNewState(queue);
                moveDown(s);
                s->manhattanDistance = getManhattanDistance(*s);
                insertPQ(s);
            }
            //printPQ();
            //system("PAUSE");
            //printf("%d. %d\n", i++, queue->manhattanDistance);
        }
    }
    printf("fronta je prazdna\n");
}


/********************** MAIN PROGRAM ********************/

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
             puts("ERR#1: Missing argument!");
             return MISSING_PARAMETER;
    }
        
    // copy argv[1] to variable initState
    char initState[strlen(argv[1])];
    strcpy(initState, argv[1]);
    
    int i, j;
    int length = strlen(initState);
    struct GameState state;
    
    // count rows
    rows = getRowsCount(initState);
    
    // there is one less semicolon than rows
    rows++;
    
    // allocation of memory for initial state of game
    state.tilesPosition = (int **)malloc(rows * sizeof(int *));
    for (i = 0; i < rows; i++)
    {
        state.tilesPosition[i] = (int *)malloc(rows * sizeof(int *));   
        for (j = 0; j < sizeof(state.tilesPosition[i]); j++)
        {
            state.tilesPosition[i][j] = 0;    
        } 
    }
    
    rows = 0;
    int cols = 0;
    int prevCols = 0;
    int number = 0;
    for (i = 0; i < length; i++)
    {
        if (initState[i] == ';')
        {
            // if count of columns doesn't match, input format isn't right
            if (cols != prevCols && prevCols != 0)
            {
                printf("ERR#2: Malformed input!");
                system("PAUSE");
                return MALFORMED_INPUT;
            }
            state.tilesPosition[rows][cols] = number;
            
            // init for next row
            rows++;
            prevCols = cols;
            cols = 0;
            number = 0;
            
            // if next character after semicolon is space, jump after it
            if (initState[i + 1] == ' ')
            {
                i += 2;    
            }
        }
        if (initState[i] == ' ')
        {
            state.tilesPosition[rows][cols] = number;
            cols++;    
            number = 0;
        }
        if (isdigit(initState[i]))
        {
            number = number * 10 + (initState[i] - 48);
        }
    }
    if (number != 0)
    {
        state.tilesPosition[rows][cols] = number;
    }
    
    printMatrix(state.tilesPosition);
        
    // init of first element in priority queue
    state.manhattanDistance = getManhattanDistance(state);
    state.distance = 0;
    state.next = (struct GameState *) malloc(sizeof(struct GameState *));
    state.prev = (struct GameState *) malloc(sizeof(struct GameState *));
    state.next = NULL;
    state.prev = NULL;
    
    // insert first element to priority queue
    insertPQ(&state);
    
    /*
    for (i = 0; i < length; i++)
    {
        free(state.tilesPosition[i]);
    }
    free(state.tilesPosition);
    */
    solveFifteen();
        
    system("PAUSE");
    return 0;
}
