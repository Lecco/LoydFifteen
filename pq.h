/**
    Inserts game state to priority queue (depending on its manhattan distance)
*/
void insertPQ(struct GameState *state)
{
    // if priority queue is empty or if this new state has lower (distance + manhattan distance), 
    // place it on top of priority queue
    if(front == NULL || (state->manhattanDistance + state->distance) < (front->manhattanDistance + front->distance))
    {
        state->next = front;
        front = state;
    }
    else
    {
        struct GameState *q;
        q = front;
        // walk through priority queue until we reach  element with too high sum of manhattan distance and distance
        while(q->next != NULL && (q->next->manhattanDistance + q->next->distance) <= (state->manhattanDistance + state->distance))
        {
            q = q->next;
        }
        // lets place this state before state with bigger (distance + manhattan distance)
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
