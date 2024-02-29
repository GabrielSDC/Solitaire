#include "ui.h"

typedef struct {
    int origin_stack;
    int finish_stack;
    bool turned_new_card;
} Move;

typedef struct {
    int score;
    char error_message[100];
    bool is_playing;
    Move *previous_move;
    Stack **stacks;
} Game;

void    Game_init();
void    Game_input();
bool    Game_isWon();
void    Game_moveCards(int origin_tb, int card_value, int finish_tb);
bool    Game_moveToFoundation(int origin, int card_value);
void    Game_freeStacks();
void    Game_solve();
Stack **Game_startStacks(Card **deck);