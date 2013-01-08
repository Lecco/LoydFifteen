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
    int i;
    int length = strlen(initState);
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
                // zero is on last position
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













