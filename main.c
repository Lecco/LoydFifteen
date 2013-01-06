#include <stdio.h>
#include <stdlib.h>


#include "constants.h"
#include "game_states.h"
#include "pq.h"

/**
    Creates new game state (copies values from argument)
*/
struct GameState *createNewState(struct GameState queue)
{
    struct GameState *s;
    int i, j;
    
    s = (struct GameState *) malloc(sizeof(struct GameState) + sizeof(int *) * sizeof(int *) * 100);
    s->distance = queue.distance + 1;
    
    s->tilesPosition = (int **)malloc(sizeof(int *) * sizeof(int *) * 100);
    int length = sizeof(int *);
    for (i = 0; i < length; i++)
    {
        s->tilesPosition[i] = (int *)malloc(length * sizeof(int *) * 100);
        
        for (j = 0; j < length; j++)
        {
            s->tilesPosition[i][j] = queue.tilesPosition[i][j];
        }
    }
    
    s->manhattanDistance = getManhattanDistance(*s);
    
    s->prev = malloc(sizeof(queue) * 100);
    s->prev = &queue;
    s->next = malloc(sizeof(queue) * 100);
    
    return s; 
}

/**
    Searches for solution
*/
void solveFifteen()
{
    struct GameState *queue;
    int i, j;
    
    while (notEmptyPQ())
    {        
        queue = getQueueTop();
            
        if (queue->manhattanDistance == 0)
        {
            printf("Nalezena cesta!\n");    
        }
        else
        {
            printMatrix(queue->tilesPosition);
            printf("pred vlevo\n");
            if (canMoveLeft(queue))
            {
                struct GameState *s = createNewState(*queue);
                moveLeft(s);
                insertPQ(s);
            }
            printf("pred vpravo\n");
            if (canMoveRight(queue))
            {
                struct GameState *s = createNewState(*queue);
                moveRight(s);
                insertPQ(s);
            }
            printf("pred hore\n");
            if (canMoveUp(queue))
            {
                struct GameState *s = createNewState(*queue);
                moveUp(s);
                insertPQ(s);
            }
            printf("pred dole\n");
            if (canMoveDown(queue))
            {
                struct GameState *s = createNewState(*queue);
                moveDown(s);
                insertPQ(s);
            }
            printPQ();
            system("PAUSE");
        }
    }
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
    int rows = getRowsCount(initState);
    
    // there is one less semicolon than rows
    rows++;
    
    // allocation of memory for initial state of game
    state.tilesPosition = (int *)malloc(rows * sizeof(int *));
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
            
            // if next character after semicolon is space, jumo after it
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
    
    printMatrix(state.tilesPosition);
    
    // init of first element in priority queue
    state.manhattanDistance = getManhattanDistance(state);
    state.distance = 0;
    state.next = NULL;
    state.prev = NULL;
    
    // insert first element to priority queue
    insertPQ(&state);
    
    solveFifteen();
    
    /*
    http://voho.cz/wiki/informatika/algoritmus/graf/a-star/
    http://stackoverflow.com/questions/9917366/priority-queue-in-c
    http://www.indiastudychannel.com/resources/13012-C-Program-priority-queue-using-linked-list.aspx
    */
    
    system("PAUSE");
    return 0;
}
