#include <stdio.h>
#include <stdlib.h>
#include "game.h"

void run() {
    Game_init();

    do {
        UI_printScreen();
        Game_input();
    }
    while(!Game_isWon());

    UI_printScreen();
    Game_freeStacks();
}

int main() {
    run();
    return 0;
}