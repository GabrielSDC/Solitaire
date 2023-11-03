#include <stdbool.h>

#define UNTURNED_CARD "\e[44;37m┌────┐\e[0m"
#define TURNED_CARD_M "\e[47m│    │\e[0m"
#define TURNED_CARD_B "\e[47m└────┘\e[0m"

typedef enum {
    HEARTS = 0, SPADES, DIAMONDS, CLUBS,
} Suit;

typedef struct Card Card; 

struct Card {
    char *value;
    Suit suit;
    bool isTurned;
    bool isOnTop;
    Card *next;
};

Card **Card_generateDeck();
Card  *Card_init(int value, Suit suit);
void   Card_turn(Card *card);
Card  *Card_get(Card **deck, char *value);
char  *Card_printable(Card *card);