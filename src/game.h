#include "ui.h"

void Game_init();
void Game_input();
bool Game_isWon();
void Game_moveCards(int origin_tb, char *card_value, int finish_tb);
void Game_moveToFoundation(int origin, char *card_value);
void Game_freeStacks();