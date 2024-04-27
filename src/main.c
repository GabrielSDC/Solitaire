#include <stdio.h>
#include <stdlib.h>
#include "game.h"

void run() {
    Game_init();

    while(Game_isRunning()) {
        UI_printScreen();
        Game_input();
    }

    UI_printScreen();
    if(Game_isWon())
        UI_printWiningMessage();
    
    Game_freeStacks();
}

int main() {
    run();
    return 0;
}