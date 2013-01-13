#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "constants.h"


/**
Test inputs:
    "7 2 6 5; 1 3 4 8; 10 11 9 12; 13 14 15 0"
    "1 2 3 4; 5 6 7 8; 9 10 11 12; 13 14 0 15"
    "1 2 3 4; 5 6 7 8; 9 10 12 15; 13 14 11 0"
    "1 2 3 4; 5 6 7 8; 9 10 11 12; 13 14 15 0"
    "1 2 3 4; 5 6 7 8; 9 10 11 12; 13 15 14 0"
    "1 2 0 4; 5 6 3 8; 9 10 7 11; 13 14 15 12"
    "1 2 3; 4 5 6; 7 0 8"
    "0 1 3; 4 2 5; 7 8 6"
    "8 7 6; 5 4 3; 2 1 0"
*/

/**
    This structure represents any state of game in state tree including
    how to get to it and its position in priority queue.
*/
struct GameState
{
        /** 2D array, on coordinates of this array are tiles*/
        int** tilesPosition;
        
        /** manhattan distance of current state (see documentation) */
        int manhattanDistance;
        
        /** distance of current state from beginnning (move increments distance) */
        int distance;
        
        /** pointer to next state in priority queue */
        struct GameState *next;
        
        /** pointer to previous state (is used to determine, how we got to this state) */
        struct GameState *prev;
} *front = NULL;

int rows = 0;

/**
    Inserts game state to priority queue (depending on its manhattan distance)
*/
void insertPQ(struct GameState *state)
{
    /* if priority queue is empty or if this new state has lower (distance + manhattan distance), 
       place it on top of priority queue */
    if(front == NULL || (state->manhattanDistance + state->distance) < (front->manhattanDistance + front->distance))
    {
        state->next = front;
        front = state;
    }
    else
    {
        struct GameState *q;
        q = front;
        /* walk through priority queue until we reach  element with too high sum of manhattan distance and distance */
        while(q->next != NULL && (q->next->manhattanDistance + q->next->distance) <= (state->manhattanDistance + state->distance))
        {
            q = q->next;
        }
        /* lets place this state before state with bigger (distance + manhattan distance) */
        state->next = q->next;
        q->next = state;
    }
}

/**
    Print priority queue from beginning in format: Position. ManhattanDistance
*/
void printPQ()
{
    struct GameState *tmp;
    tmp = front;
    int i;
    for (i = 1; tmp != NULL; i++)
    {
        printf("%d. %d\n", i, (tmp->manhattanDistance + tmp->distance));  
        tmp = tmp->next; 
    }
}

/**
    Returns true (1) if priority queue isn't empty
*/
int notEmptyPQ()
{
    return (front != NULL);    
}

/**
    Returns top of priority queue (element with lowest sum of manhattan distance and distance)
    and makes top of queue next element
*/
struct GameState *getQueueTop()
{
    struct GameState *top;
    top = front;
    front = front->next;
    return top;
}

/**
   Returns true (1) if in current game tree state it is possible
   to move empty space left
*/
int canMoveLeft(struct GameState* thisState)
{
    int i;
    for (i = 0; i < rows; i++)
    {
        if (thisState->tilesPosition[i][0] == 0)
        {
            return 0;
        }
    }
    return 1;
}

/**
   Returns true (1) if in current game tree state it is possible
   to move empty space right
*/
int canMoveRight(struct GameState* thisState)
{
    int i;
    for (i = 0; i < rows; i++)
    {
        if (thisState->tilesPosition[i][rows - 1] == 0)
        {
            return 0;
        }
    }
    return 1;
}

/**
   Returns true (1) if in current game tree state it is possible
   to move empty space up
*/
int canMoveUp(struct GameState* thisState)
{
    int i;
    for (i = 0; i < rows; i++)
    {
        if (thisState->tilesPosition[0][i] == 0)
        {
            return 0;
        }
    }
    return 1;
}

/**
   Returns true (1) if in current game tree state it is possible
   to move empty space down
*/
int canMoveDown(struct GameState* thisState)
{
    int i;
    for (i = 0; i < rows; i++)
    {
        if (thisState->tilesPosition[rows - 1][i] == 0)
        {
            return 0;
        }
    }
    return 1;
}

/**
    Return state after moving empty space to left.
*/
void moveLeft(struct GameState *state)
{
    int i, j;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < rows; j++)
        {
            if (state->tilesPosition[i][j] == 0)
            {
                state->tilesPosition[i][j] = state->tilesPosition[i][j - 1];
                state->tilesPosition[i][j - 1] = 0;
                return;
            }
        }
    }
}

/**
    Return state after moving empty space to right.
*/
void moveRight(struct GameState *state)
{
    int i, j;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < rows; j++)
        {
            if (state->tilesPosition[i][j] == 0)
            {
                state->tilesPosition[i][j] = state->tilesPosition[i][j + 1];
                state->tilesPosition[i][j + 1] = 0;
                return;
            }
        }
    }
}

/**
    Return state after moving empty space to up.
*/
void moveUp(struct GameState *state)
{
    int i, j;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < rows; j++)
        {
            if (state->tilesPosition[i][j] == 0)
            {
                state->tilesPosition[i][j] = state->tilesPosition[i - 1][j];
                state->tilesPosition[i - 1][j] = 0;
                return;
            }
        }
    }
}

/**
    Return state after moving empty space to down.
*/
void moveDown(struct GameState *state)
{
    int i, j;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < rows; j++)
        {
            if (state->tilesPosition[i][j] == 0)
            {
                state->tilesPosition[i][j] = state->tilesPosition[i + 1][j];
                state->tilesPosition[i + 1][j] = 0;
                return;
            }
        }
    }
}

/**
    Return last move
*/
int getLastMove(struct GameState *state)
{
    if (state->prev == NULL)
    {
        return 0;
    }
    else
    {
        int i, j;
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < rows; j++)
            {
                if (state->tilesPosition[i][j] == 0)
                {
                    if (j > 0 && state->prev->tilesPosition[i][j - 1] == 0)
                    {
                        return MOVE_RIGHT;
                    }
                    if (i < (rows - 1) && state->prev->tilesPosition[i + 1][j] == 0)
                    {
                        return MOVE_UP;
                    }
                    if (j < (rows - 1) && state->prev->tilesPosition[i][j + 1] == 0)
                    {
                        return MOVE_LEFT;
                    }
                    if (i > 0 && state->prev->tilesPosition[i - 1][j] == 0)
                    {
                        return MOVE_DOWN;
                    }
                }
            }
        }
    }
    return 0;
}

/**
    Return tile which moved. 
*/
int getMovedTile(struct GameState *state)
{
    if (state->prev == NULL)
    {
        return 0;
    }
    else
    {
        int i, j;
        for (i = 0; i < rows; i++)
        {
            for (j = 0; j < rows; j++)
            {
                /* if current state has zero there, previous state has on these
                   coordinates tile which moved */
                if (state->tilesPosition[i][j] == 0)
                {
                    return state->prev->tilesPosition[i][j];
                }
            }
        }
    }
    return 0;
}

/**
    Returns number of rows in given string (according to format of parameter)
*/
int getRowsCount(char* initState)
{
    int rows = 0;
    int i;
    int length = strlen(initState);
    for (i = 0; i < length; i++)
    {
        if (initState[i] == ';')
        {
            /* we don't count semicolon on last position of parameter (if there is one) */
            if (i != strlen(initState) - 1)
            {
                rows++;    
            }
        }
        if (initState[i] != ';' && initState[i] != ' ' && isdigit(initState[i]) == 0)
        {
            return -1;
        }
    }
    
    return rows;
}

/**
    Prints two-dimensional array (only for checking if it is correct)
*/
void printMatrix(int** matrix)
{
    int i, j;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < rows; j++)
        {
            printf("%d ", matrix[i][j]);    
        }    
        printf("\n");
    }
}

/**
    Returns sum of manhattan distances between number and their right position
    (see documentation to find out how to solve manhattan distance)
*/
int getManhattanDistance(struct GameState state)
{
    int manhattan = 0;
    int number = 0;
    int row;
    int i, j;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < rows; j++)
        {
            number = state.tilesPosition[i][j];
            if (number == 0)
            {
                /* zero is on last position */
                number = rows * rows;
            }
            row = (number - 1) / rows;
            manhattan += abs(row - i);
            manhattan += abs((number - 1) % rows - j);
        }
    }
    
    return manhattan;
}

/**
    Returns 1 if this game state can be solved
    (see documentation for info how it is determined)
*/
int isSolvableState(struct GameState *state)
{
    int inversionCount = 0;
    int i, j, rowWithSpace;
    
    for (i = 0; i < rows * rows; i++)
    {
        if (state->tilesPosition[i / rows][i % rows] == 0)
        {
            rowWithSpace = i / rows;
            continue;
        }
        for (j = i + 1; j < rows * rows; j++)
        {
            if (state->tilesPosition[j / rows][j % rows] == 0)
            {
                continue;
            }
            if (state->tilesPosition[i / rows][i % rows] > state->tilesPosition[j / rows][j % rows])
            {
                inversionCount++;
            }
        }
    }
    
    if ((rows % EVEN_NUMBER == 1 && inversionCount % EVEN_NUMBER == 0) ||
        (rows % EVEN_NUMBER == 0 && (rows - rowWithSpace) % EVEN_NUMBER == 0 && inversionCount % EVEN_NUMBER == 1) ||
        (rows % EVEN_NUMBER == 0 && (rows - rowWithSpace) % EVEN_NUMBER == 1 && inversionCount % EVEN_NUMBER == 0))
    {
        return 1;
    }
    
    return 0;
}

/**
    Creates new game state (copies values from argument)
*/
struct GameState *createNewState(struct GameState *queue)
{
    struct GameState *s;
    int i, j;
    
    s = (struct GameState *) malloc(sizeof(struct GameState) + sizeof(int *) * sizeof(int *) * rows);
    if (s == NULL)
    {
        return NULL;    
    }
    
    s->distance = queue->distance + 1;
    
    s->tilesPosition = (int **)malloc(sizeof(int *) * sizeof(int *) * rows);
    if (s->tilesPosition == NULL)
    {
        return NULL;
    }
    for (i = 0; i < rows; i++)
    {
        s->tilesPosition[i] = (int *)malloc(sizeof(int *) * rows);
        if (s->tilesPosition[i] == NULL)
        {
            return NULL;
        }
        
        for (j = 0; j < rows; j++)
        {
            s->tilesPosition[i][j] = queue->tilesPosition[i][j];
        }
    }
    
    s->prev = (struct GameState *) malloc(sizeof(struct GameState *));
    if (s->prev == NULL)
    {
        return NULL;
    }
    s->prev = queue;
    s->next = (struct GameState *) malloc(sizeof(struct GameState *));
    if (s->next == NULL)
    {
        return NULL;
    }
    
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
    /* fill arays of moves and tiles, which moved - we'll have to go through them
       from the end */
    for (i = 0; goal != NULL; i++)
    {
        moves[i] = getLastMove(goal);
        movedTiles[i] = getMovedTile(goal);
        goal = goal->prev;
    }
    char *move;
    for (i = queue->distance - 1, j = 1; i >= 0; i--, j++)
    {
        /* moves are named after moving of space (empty tile), but we want to know, 
           which direction moved tile to swap with */
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
        printf("%d: [%d] %s\n", j, movedTiles[i], move);
    }
    printf("GOAL\n");
    system("PAUSE");
}

/**
    Searches for solution
*/
int solveFifteen()
{
    struct GameState *queue;
    int i;
    
    while (notEmptyPQ())
    {
        queue = getQueueTop();
        
        if (queue->manhattanDistance == 0)
        {
            printSolution(queue);
            
            for (i = 0; i < rows; i++)
            {
                free(queue->tilesPosition[i]);
            }
            free(queue->tilesPosition);
            free(queue->prev);
            queue->prev = NULL;
            
            /* deallocate memory */
            int pocet = 0;
            while (notEmptyPQ())
            {
                pocet++;
                free(queue);
                queue = NULL;
                queue = getQueueTop();
                for (i = 0; i < rows; i++)
                {
                    free(queue->tilesPosition[i]);
                }
                free(queue->tilesPosition);
                free(queue->prev);
                queue->prev = NULL;
            }
            printf("Ve fronte bylo %d\n", pocet);
            system("PAUSE");
            return 0;
        }
        else
        {
            int lastMove = getLastMove(queue);
            if (canMoveLeft(queue) && lastMove != MOVE_RIGHT)
            {
                struct GameState *s = createNewState(queue);
                if (s == NULL)
                {
                    return OUT_OF_MEMORY;    
                }
                moveLeft(s);
                s->manhattanDistance = getManhattanDistance(*s);
                insertPQ(s);
            }
            if (canMoveRight(queue) && lastMove != MOVE_LEFT)
            {
                struct GameState *s = createNewState(queue);
                if (s == NULL)
                {
                    return OUT_OF_MEMORY;    
                }
                moveRight(s);
                s->manhattanDistance = getManhattanDistance(*s);
                insertPQ(s);
            }
            if (canMoveUp(queue) && lastMove != MOVE_DOWN)
            {
                struct GameState *s = createNewState(queue);
                if (s == NULL)
                {
                    return OUT_OF_MEMORY;    
                }
                moveUp(s);
                s->manhattanDistance = getManhattanDistance(*s);
                insertPQ(s);
            }
            if (canMoveDown(queue) && lastMove != MOVE_UP)
            {
                struct GameState *s = createNewState(queue);
                if (s == NULL)
                {
                    return OUT_OF_MEMORY;    
                }
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
        
    /* copy argv[1] to variable initState */
    char initState[strlen(argv[1])];
    strcpy(initState, argv[1]);
    
    int i, j;
    int length = strlen(initState);
    struct GameState state;
    
    /* count rows */
    rows = getRowsCount(initState);
    
    if (rows == -1)
    {
        printf("ERR#2: Malformed input!");
        return MALFORMED_INPUT;
    }
    
    /* there is one less semicolon than rows */
    rows++;
    if (rows < MINIMUM_ROWS)
    {
        printf("ERR#3: Field too small!");
        return FIELD_TOO_SMALL;
    }
        
    /* allocation of memory for initial state of game */
    state.tilesPosition = (int **)malloc(rows * sizeof(int *));
    if (state.tilesPosition == NULL)
    {
        printf("ERR#5: Out of memory!");
        return OUT_OF_MEMORY;
    }
    for (i = 0; i < rows; i++)
    {
        state.tilesPosition[i] = (int *)malloc(rows * sizeof(int *));
        if (state.tilesPosition[i] == NULL)
        {
            printf("ERR#5: Out of memory!");
            return OUT_OF_MEMORY;
        }
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
            /* if count of columns doesn't match, input format isn't right */
            if (cols != prevCols && prevCols != 0)
            {
                printf("ERR#2: Malformed input!");
                return MALFORMED_INPUT;
            }
            state.tilesPosition[rows][cols] = number;
            usedNumbers[number] = 1;
            
            /* init for next row */
            rows++;
            prevCols = cols;
            cols = 0;
            number = 0;
            
            /* if next character after semicolon is space, jump after it */
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
    state.tilesPosition[rows][cols] = number;
    usedNumbers[number] = 1;
    
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
    
    /* init of first element in priority queue */
    state.manhattanDistance = getManhattanDistance(state);
    state.distance = 0;
    state.next = (struct GameState *) malloc(sizeof(struct GameState *));
    if (state.next == NULL)
    {
        printf("ERR#5: Out of memory!");
        return OUT_OF_MEMORY;
    }
    state.prev = (struct GameState *) malloc(sizeof(struct GameState *));
    if (state.prev == NULL)
    {
        printf("ERR#5: Out of memory!");
        return OUT_OF_MEMORY;
    }
    state.next = NULL;
    state.prev = NULL;
    
    /* insert first element to priority queue */
    insertPQ(&state);
    
    int solved = solveFifteen();
    
    if (solved == OUT_OF_MEMORY)
    {
        printf("ERR#5: Out of memory!");
        return OUT_OF_MEMORY;
    }
        
    /* system("PAUSE"); */
    return 0;
}
