#include <stdio.h>
#include <stdlib.h>
#include "stacks.h"

void run() {
    Card **deck = Card_generateDeck();

    // for(int i = 0; i < 52; i++) {
    //     printf("%s\n", Card_printable(deck[i]));
    // }

    Stack **s = Stack_generateGame(deck);

    for(int i = 0; i < 7; i++) {
        for(Card *temp = s[TABLEAU + i]->first; temp != NULL; temp = temp->next) {
            printf("%s ", Card_printable(temp));
        }
        printf("\n");
    }

    for(Card *temp = s[STOCK]->first; temp != NULL; temp = temp->next) {
        printf("%s ", Card_printable(temp));
    }
    printf("\n");

    for(Card *temp = s[STOCK_SIDE]->first; temp != NULL; temp = temp->next) {
        printf("%s ", Card_printable(temp));
    }
    printf("\n");

    for(int i = 0; i < 4; i++) {
        for(Card *temp = s[FOUNDATION + i]->first; temp != NULL; temp = temp->next) {
            printf("%s ", Card_printable(temp));
        }
        printf("\n");
    }
}

int main() {
    run();
    return 0;
}