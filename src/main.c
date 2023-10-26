#include <stdio.h>
#include <stdlib.h>
#include "card.h"

void run() {
    Card **deck = Card_generateDeck();

    for(int i = 0; i < 54; i++) {
        Card_print(deck[i]);
    }

    printf("1\n");
}

int main() {
    run();
    return 0;
}