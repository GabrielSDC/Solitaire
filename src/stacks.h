#include <stdbool.h>
#include "card.h"

typedef enum {
    TABLEAU = 0, FOUNDATION = 7, STOCK = 11, STOCK_SIDE, NONE
} StackType;

typedef struct {
    int size;
    Card *first;
    StackType type;
} Stack;

Stack **Stack_init(int size);
void    Stack_pushCards(Stack *stack, Card *card);
Card   *Stack_popCards(Stack *stack, char *value);
Stack **Stack_generateGame(Card **deck);
bool    Stack_isEmpty(Stack *stack);
int   **Stack_print(Stack *stack);
Card   *Stack_getPos(Stack *stack, int position);
