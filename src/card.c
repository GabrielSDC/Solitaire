#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "card.h"

static const char *cardVal[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
static const char *suits[] = {"♥", "♠", "♦", "♣"};

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

    c->value = malloc(sizeof(char*));
    strcpy(c->value, cardVal[value]);
    c->suit = suit;
    c->isTurned = true;
    c->isOnTop = false;
    c->next = NULL;

    return c;
}

void Card_turn(Card *card) {
    card->isTurned = true;
}

char *Card_printable(Card *card) {
    char *cardInfo = malloc(sizeof(char) * 10);

    strcat(cardInfo, card->value);
    strcat(cardInfo, suits[card->suit]);

    return cardInfo;
}

Card *Card_get(Card **deck, char *value) {
    for(int i = 0; i < 52; i++)
        if(!strcmp(deck[i]->value, value))
            return deck[i];

    return NULL;
}
