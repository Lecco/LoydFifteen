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
    int i, j;
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