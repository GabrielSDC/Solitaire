#include <stdio.h>
#include <stdlib.h>
#include "game.h"

void run() {
    Game_init();

    do {
        system("clear");
        UI_printScreen();
        Game_input();
    }
    while(!Game_isWon());
}

int main() {
    run();
    return 0;
}