#include <stdbool.h>
#include "card.h"

typedef enum {
    TABLEAU = 0, FOUNDATION = 7, STOCK = 11, STOCK_SIDE, NONE
} StackType;

typedef struct {
    int size;
    Card *first;
    Card *top;
    StackType type;
} Stack;

Stack **Stack_init(int size);
bool    Stack_isEmpty(Stack *stack);
bool    Stack_isEmpty(Stack *stack);
void    Stack_pushCards(Stack *stack, Card *card);
Card   *Stack_popCards(Stack *stack, int value, Suit suit);
void    Stack_returnUnusedCard(Stack *stack, Card *card);
