#include <stdio.h>
#include "game.h"

void print_stack(Stack *stack) {
    for(Card *temp = stack->first; temp != NULL; temp = temp->next) {
        printf("%s\n", Card_printable(temp));

        if(temp->isOnTop) {
            if(temp->suit == CLUBS || temp->suit == SPADES) {
                printf("\e[0;30m%s\e[0m\n", TURNED_CARD_M);
                printf("\e[0;30m%s\e[0m\n", TURNED_CARD_B);
            }
            else {
                printf("\e[0;30m%s\e[0m\n", TURNED_CARD_B);
                printf("\e[0;30m%s\e[0m\n", TURNED_CARD_M);
            }
        }
    }
}