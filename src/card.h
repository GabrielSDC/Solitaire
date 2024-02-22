#include <stdbool.h>

typedef enum {
    HEARTS = 0, SPADES, DIAMONDS, CLUBS,
} Suit;

typedef struct Card Card; 

struct Card {
    int value;
    Suit suit;
    bool isTurned;
    bool isOnTop;
    Card *next;
};

Card **Card_generateDeck();
Card  *Card_init(int value, Suit suit);
void   Card_turn(Card *card);
Card *Card_get(Card **deck, int value, Suit s);