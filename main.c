#include <stdio.h>
#include <stdlib.h>

#define POSITION_ZERO 16
#define MISSING_PARAMETER 1
#define MALFORMED_INPUT 2


/***************** DATA STRUCTURES ***************************/
struct GameState
{
        int** tilesPosition;
        int manhattanDistance;
        int distance;
        struct GameState *next;
} *queueTop = NULL;

/**************** FUNCTION PROTOTYPES ********************/

// functions of GameState structure
int canMoveLeft(struct GameState* thisState);
int canMoveRight(struct GameState* thisState);
int canMoveUp(struct GameState* thisState);
int canMoveDown(struct GameState* thisState);

// function working with parameter from command line
int getRowsCount(char* initState);
void printMatrix(int** matrix);

// PQ = priority queue
int getManhattanDistance(struct GameState state);
void insertPQ(struct GameState *state);
void printPQ();
int notEmptyPQ();


/********************** FUNCTION DEFINITIONS *****************/
/**
   Returns true (1) if in current game tree state it is possible
   to move empty space left
*/
int canMoveLeft(struct GameState* thisState)
{
     return 1;
}

/**
   Returns true (1) if in current game tree state it is possible
   to move empty space right
*/
int canMoveRight(struct GameState* thisState)
{
    return 1;
}

/**
   Returns true (1) if in current game tree state it is possible
   to move empty space up
*/
int canMoveUp(struct GameState* thisState)
{
    return 1;
}

/**
   Returns true (1) if in current game tree state it is possible
   to move empty space down
*/
int canMoveDown(struct GameState* thisState)
{
    return 1;
}

/**
    Returns number of rows in given string (according to format of parameter)
*/
int getRowsCount(char* initState)
{
    int rows = 0;
    int length = strlen(initState);
    int i;
    for (i = 0; i < length; i++)
    {
        if (initState[i] == ';')
        {
            // we don't count semicolon on last position of parameter
            if (i != strlen(initState) - 1)
            {
                rows++;    
            }
        }    
    }
    
    return rows;
}

/**
    Prints two dimesnional array
*/
void printMatrix(int** matrix)
{
    int length = sizeof(matrix);
    int i, j;
    for (i = 0; i < length; i++)
    {
        for (j = 0; j < length; j++)
        {
            printf("%d ", matrix[i][j]);    
        }    
        printf("\n");
    }
}

/**
    Returns sum of manhattan distances between number and their right position
*/
int getManhattanDistance(struct GameState state)
{
    int manhattan = 0;
    int number = 0;
    int row;
    int i, j, k, l;
    int length = sizeof(state.tilesPosition); 
    for (i = 0; i < length; i++)
    {
        for (j = 0; j < length; j++)
        {
            number = state.tilesPosition[i][j];
            if (number == 0)
            {
                // zero is on sixteenth position
                number = POSITION_ZERO;
            }
            row = (number - 1) / length;
            manhattan += abs(row - i);
            manhattan += abs((number - 1) % length - j);
        }
    }
    
    return manhattan;
}

/**
    Inserts game state to prioiroty queue (depending on its manhattan distance)
*/
void insertPQ(struct GameState *state)
{
    if (queueTop == NULL)
    {
        queueTop = state;
        queueTop->next = NULL;
    }
    else
    {
        struct GameState *tmp = queueTop;
        
        while (tmp->next != NULL && tmp->next->manhattanDistance > state->manhattanDistance);
        
        state->next = tmp->next;
        tmp->next = state;
        tmp->next->next = NULL;
        
        
    }
}

/**
    Print priority queue from beginning in format: Position. ManhattanDistance
*/
void printPQ()
{
    struct GameState *tmp;
    tmp = queueTop;
    int i;
    for (i = 1; tmp != NULL; i++)
    {
        printf("%d. %d\n", i, tmp->manhattanDistance);  
        tmp = tmp->next;  
    }
}

/**
    Returns true (1) if priority queue isn't empty
*/
int notEmptyPQ()
{
    return (queueTop != NULL);    
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
    state.distance = 0;
    
    // count rows
    int rows = getRowsCount(initState);
    
    // there is one less semicolon than rows
    rows++;
    
    // allocation of memory for initial state of game
    state.tilesPosition = (int *) malloc(rows * sizeof(int *));
    for (i = 0; i < rows; i++)
    {
        state.tilesPosition[i] = malloc(rows * sizeof(int *));   
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
    state.distance = 0;
    state.manhattanDistance = getManhattanDistance(state);
    
    // insert first element to priority queue
    insertPQ(&state);
    
    /*
    while (notEmptyPQ())
    {
            
    }*/
    
    printPQ();
    
    /*
    http://voho.cz/wiki/informatika/algoritmus/graf/a-star/
    http://stackoverflow.com/questions/9917366/priority-queue-in-c
    http://www.indiastudychannel.com/resources/13012-C-Program-priority-queue-using-linked-list.aspx
    */
    
    printf("%d\n", getManhattanDistance(state));
    system("PAUSE");
    return 0;
}
