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

/**
   Returns true (1) if in current game tree state it is possible
   to move empty space left
*/
int canMoveLeft(struct GameState* thisState)
{
    int i;
    int length = sizeof(thisState->tilesPosition);
    for (i = 0; i < length; i++)
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
    int length = sizeof(thisState->tilesPosition);
    for (i = 0; i < length; i++)
    {
        if (thisState->tilesPosition[i][length - 1] == 0)
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
    int length = sizeof(thisState->tilesPosition);
    for (i = 0; i < length; i++)
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
    int length = sizeof(thisState->tilesPosition);
    for (i = 0; i < length; i++)
    {
        if (thisState->tilesPosition[length - 1][i] == 0)
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
    int length = sizeof(state->tilesPosition);
    for (i = 0; i < length; i++)
    {
        for (j = 0; j < length; j++)
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
    int length = sizeof(state->tilesPosition);
    for (i = 0; i < length; i++)
    {
        for (j = 0; j < length; j++)
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
    int length = sizeof(state->tilesPosition);
    for (i = 0; i < length; i++)
    {
        for (j = 0; j < length; j++)
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
    int length = sizeof(state->tilesPosition);
    for (i = 0; i < length; i++)
    {
        for (j = 0; j < length; j++)
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
        int length = sizeof(state->tilesPosition);
        for (i = 0; i < length; i++)
        {
            for (j = 0; j < length; j++)
            {
                if (state->tilesPosition[i][j] == 0)
                {
                    if (j > 0 && state->prev->tilesPosition[i][j - 1] == 0)
                    {
                        return MOVE_RIGHT;
                    }
                    if (i < (length - 1) && state->prev->tilesPosition[i + 1][j] == 0)
                    {
                        return MOVE_UP;
                    }
                    if (j < (length - 1) && state->prev->tilesPosition[i][j + 1] == 0)
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
        int length = sizeof(state->tilesPosition);
        for (i = 0; i < length; i++)
        {
            for (j = 0; j < length; j++)
            {
                // if current state has zero there, previous state has on these
                // coordinates tile which moved
                if (state->tilesPosition[i][j] == 0)
                {
                    return state->prev->tilesPosition[i][j];
                }
            }
        }
    }
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
            // we don't count semicolon on last position of parameter (if there is one)
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
    (see documentation to find out how to solve manhattan distance)
*/
int getManhattanDistance(struct GameState state)
{
    int manhattan = 0;
    int number = 0;
    int row;
    int i, j;
    int length = sizeof(state.tilesPosition);
    for (i = 0; i < length; i++)
    {
        for (j = 0; j < length; j++)
        {
            number = state.tilesPosition[i][j];
            if (number == 0)
            {
                // zero is on last position
                number = length * length;
            }
            row = (number - 1) / length;
            manhattan += abs(row - i);
            manhattan += abs((number - 1) % length - j);
        }
    }
    
    return manhattan;
}
