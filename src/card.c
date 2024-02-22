#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "card.h"

static void randDeck(Card **deck) {
    Card *temp;
    int rPos;

    srand(time(NULL));

    for(int i = 0; i < 52; i++) {
        rPos = rand() % 52;

        temp = deck[i];
        deck[i] = deck[rPos];
        deck[rPos] = temp;
    }
}

Card **Card_generateDeck() {
    Card **deck = malloc(sizeof(Card*) * 54);
    
    for(Suit s = HEARTS; s <= CLUBS; s++) {
        for(int i = 0; i < 13; i++) {
            deck[s * 13 + i] = Card_init(i, s);
        }
    }

    randDeck(deck);

    return deck;
}

Card *Card_init(int value, Suit suit) {
    Card *c = malloc(sizeof(Card));

    // c->value    = malloc(sizeof(char*));
    c->value    = value;
    c->suit     = suit;
    c->isTurned = true;
    c->isOnTop  = false;
    c->next     = NULL;

    return c;
}

void Card_turn(Card *card) {
    if(card)
        card->isTurned = false;
}

Card *Card_get(Card **deck, int value, Suit s) {
    for(int i = 0; i < 52; i++)
        if(deck[i]->value == value && deck[i]->suit == s)
            return deck[i];
    return NULL;
}
