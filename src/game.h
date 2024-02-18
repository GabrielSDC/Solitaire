#include "ui.h"

void Game_init();
void Game_input();
bool Game_isWon();
void Game_moveCards(int origin_tb, int card_value, int finish_tb);
void Game_moveToFoundation(int origin, int card_value);
void Game_freeStacks();