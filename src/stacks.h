#include <stdbool.h>
#include "card.h"

typedef struct {
    int size;
    Card first;
} Stack;

Stack *Stack_init();
void   Stack_pushCards(Stack *stack, Card *card);
Card  *Stack_popCards(Stack *stack, char *value);
Card **Stack_generateGame(Card **deck);
bool   Stack_isEmpty(Stack *stack);
int  **Stack_print(Stack *stack);