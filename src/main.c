#include <stdio.h>
#include <stdlib.h>
#include "card.h"

void run() {
    Card **deck = Card_generateDeck();

    for(int i = 0; i < 52; i++) {
        printf("%s\n", Card_printable(deck[i]));
    }

    Card c = Card_get("10", DIAMOND);
}

int main() {
    run();
    return 0;
}