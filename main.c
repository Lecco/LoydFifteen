#include <stdio.h>
#include <stdlib.h>

int rows = 0;

#include "constants.h"
#include "pq.h"
#include "game_states.h"



/**
Test inputs:
    "7 2 6 5; 1 3 4 8; 10 11 9 12; 13 14 15 0"
    "1 2 3 4; 5 6 7 8; 9 10 11 12; 13 14 0 15"
    "1 2 3 4; 5 6 7 8; 9 10 12 15; 13 14 11 0"
    "1 2 3 4; 5 6 7 8; 9 10 11 12; 13 14 15 0"
    "1 2 3 4; 5 6 7 8; 9 10 11 12; 13 15 14 0"
    "1 2 3; 4 5 6; 7 0 8"
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
    for (i = 0; i < rows; i++)
    {
        s->tilesPosition[i] = (int *)malloc(sizeof(int *) * rows);
        
        for (j = 0; j < rows; j++)
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
    Print solution in this format:
        moveNumber. [movedTile] MOVE_DIRECTION
*/
void printSolution(struct GameState *queue)
{
    int i, j;
    struct GameState *goal = queue;
    int moves[queue->distance];
    int movedTiles[queue->distance];
    // fill arays of moves and tiles, which moved - we'll have to go through them
    // from the end
    for (i = 0; goal != NULL; i++)
    {
        moves[i] = getLastMove(goal);
        movedTiles[i] = getMovedTile(goal);
        goal = goal->prev;
    }
    char *move;
    for (i = queue->distance - 1, j = 1; i >= 0; i--, j++)
    {
        // moves are named after moving of space (empty tile), but we want to know, 
        // which direction moved tile to swap with
        if (moves[i] == MOVE_LEFT)
        {
            move = "RIGHT";
        }
        if (moves[i] == MOVE_UP)
        {
            move = "DOWN";
        }
        if (moves[i] == MOVE_RIGHT)
        {
            move = "LEFT";
        }
        if (moves[i] == MOVE_DOWN)
        {
            move = "UP";
        }
        printf("%d. [%d] %s\n", j, movedTiles[i], move);
    }
}

/**
    Searches for solution
*/
int solveFifteen()
{
    struct GameState *queue;
    
    while (notEmptyPQ())
    {        
        queue = getQueueTop();
        
        if (queue->manhattanDistance == 0)
        {
            printSolution(queue);
            return 0;
        }
        else
        {
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
        }
    }
    return 1;
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
    
    if (rows == -1)
    {
        printf("ERR#2: Malformed input!");
        return MALFORMED_INPUT;
    }
    
    // there is one less semicolon than rows
    rows++;
        
    // allocation of memory for initial state of game
    state.tilesPosition = (int **)malloc(rows * sizeof(int *));
    for (i = 0; i < rows; i++)
    {
        state.tilesPosition[i] = (int *)malloc(rows * sizeof(int *));   
        for (j = 0; j < rows; j++)
        {
            state.tilesPosition[i][j] = 0;    
        } 
    }
    
    int usedNumbers[rows * rows];
    for (i = 0; i < rows * rows; i++)
    {
        usedNumbers[i] = 0;
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
                return MALFORMED_INPUT;
            }
            state.tilesPosition[rows][cols] = number;
            usedNumbers[number] = 1;
            
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
            usedNumbers[number] = 1;
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
        usedNumbers[number] = 1;
    }
    rows++;
    
    for (i = 0; i < rows * rows; i++)
    {
        if (usedNumbers[i] == 0)
        {
            printf("ERR#2: Malformed input!");
            return MALFORMED_INPUT;
        }
    }
    
    int isSolvable = isSolvableState(&state);
    if (isSolvable == 0)
    {
        printf("ERR#4: Non-existent solution!");
        return NONEXISTENT_SOLUTION;
    }
    
    // init of first element in priority queue
    state.manhattanDistance = getManhattanDistance(state);
    state.distance = 0;
    state.next = (struct GameState *) malloc(sizeof(struct GameState *));
    state.prev = (struct GameState *) malloc(sizeof(struct GameState *));
    state.next = NULL;
    state.prev = NULL;
    
    // insert first element to priority queue
    insertPQ(&state);
    
    int solved = solveFifteen();
    
    if (solved == 0)
    {
        printf("OH YEAH!\n");
    }
    else
    {
        printf("Toz nic\n");
    }
        
    system("PAUSE");
    return 0;
}
