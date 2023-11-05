#include <stdio.h>
#include <stdlib.h>
#include "game.h"

void run() {
    Game_init();

    while(!Game_isWon()) {
        Game_printTableau();
        Game_input();
    }
}

int main() {
    run();
    return 0;
}