struct GameState
{
        int** tilesPosition;
        int manhattanDistance;
        int distance;
        struct GameState *next;
        struct GameState *prev;
} *front = NULL;

/**
    Inserts game state to prioiroty queue (depending on its manhattan distance)
*/
void insertPQ(struct GameState *state)
{
    if(front == NULL || (state->manhattanDistance + state->distance) < (front->manhattanDistance + front->distance))
    {
        state->next = front;
        front = state;
    }
    else
    {
        struct GameState *q;
        q = front;
        while(q->next != NULL && (q->next->manhattanDistance + q->next->distance) <= (state->manhattanDistance + state->distance))
        {
            q = q->next;
        }
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
    Returns top of priority queue (element with lowest manhattan distance)
*/
struct GameState *getQueueTop()
{
    struct GameState *top;
    top = front;
    front = front->next;
    return top;
}
