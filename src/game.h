#include "ui.h"

typedef enum {CARD_NOT_FOUND, INVALID_MOVEMENT, NO_ERROR} Errors;

typedef struct Move Move;

struct Move {
    Card *card;
    int origin_stack;
    int finish_stack;
    int score;
    bool turned_new_card;
    Move *previous;
};

typedef struct {
    int score;
    char error_message[100];
    bool is_playing;
    Move *last_move;
    Stack **stacks;
} Game;

void    Game_init();
void    Game_input();
bool    Game_isWon();
Errors  Game_moveCards(int origin_tb, int card_value, int finish_tb);
Errors  Game_moveToFoundation(int origin, int card_value);
void    Game_undoMovement();
void    Game_freeStacks();
void    Game_solve();
Stack **Game_startStacks(Card **deck);