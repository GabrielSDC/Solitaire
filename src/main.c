#include <stdio.h>
#include <stdlib.h>
#include "card.h"

void run() {
    Card **deck = Card_generateDeck();

    for(int i = 0; i < 52; i++) {
        Card_print(deck[i]);
    }
}

int main() {
    run();
    return 0;
}