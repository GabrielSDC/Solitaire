#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "game.h"

Stack **Game_stacks;
char error_message[100];

void Game_init() {
    Card **deck = Card_generateDeck();
    Game_stacks = Stack_generateGame(deck);
    UI_initScreen(Game_stacks);
    UI_printLogo();
}

void Game_input() {
    char move[10], value[10], to[20];

    printf("%s\n", error_message);
    strcpy(error_message, "");
    printf("from: ");
    scanf(" %s", move);

    if(!strcmp(move, "--help")) {
        // Game_helpMessage();
    }
    else if(!strcmp(move, "n")) {
        Game_moveCards(STOCK, NULL, STOCK_SIDE);
        UI_updateScreen(STOCK, STOCK_SIDE);
    }
    else if(!strcmp(move, "s")) {
        printf("to: ");
        scanf(" %s", to);

        if(atoi(to) > 0 && atoi(move) < 8) {
            Game_moveCards(STOCK_SIDE, NULL, atoi(to) - 1);
            UI_updateScreen(STOCK_SIDE, atoi(to) - 1);
        }
        else if(!strcmp(to, "f")) {
            StackType st = Game_stacks[STOCK_SIDE]->top->suit;
            Game_moveToFoundation(STOCK_SIDE, NULL);
            UI_updateScreen(STOCK_SIDE, FOUNDATION + st);
        }
        else
            strcpy(error_message, "Invalid input!1");
    }
    else if(atoi(move) > 0 && atoi(move) < 8) {
        printf("Card: ");
        scanf(" %s", value);
        printf("to: ");
        scanf(" %s", to);

        if(atoi(to) > 0 && atoi(to) < 8) {
            Game_moveCards(atoi(move) - 1, value, atoi(to) - 1);
            UI_updateScreen(atoi(move) - 1, atoi(to) - 1);
        }
        else if(!strcmp(to, "f")) {
            StackType st = Game_stacks[atoi(move) - 1]->top->suit;
            Game_moveToFoundation(atoi(move) - 1, value);
            UI_updateScreen(atoi(move) - 1, FOUNDATION + st);
        }
        else
            strcpy(error_message, "Invalid input!2");
    }
    else {
        strcpy(error_message, "Invalid input!3");
    }
}

static char *toupperstr(char *str) {
    if(str)
        for(int i = 0; i < strlen(str); i++)
            str[i] = toupper(str[i]);

    return str;
}

static bool isMovementValid(Card *base, Card *moving, StackType Stype) {
    if(!base && !moving || Stype != TABLEAU && Stype != FOUNDATION)
        return false;

    if(Stype == TABLEAU) {
        if((base->suit == (moving->suit + 1) % 4 || 
            base->suit == (moving->suit + 3) % 4) &&                         // se a cor de naipe é diferente
           (atoi(moving->value) + 1 == atoi(base->value) ||                  // se o valor de moving é base + 1
            !strcmp(moving->value, "A") && atoi(base->value) == 2 ||         // se moving é A e base é 2
            atoi(moving->value) == 10   && !strcmp(base->value, "J") ||      // se moving é 10 e base é J
            !strcmp(moving->value, "J") && !strcmp(base->value, "Q") ||      // se moving é J e base é Q
            !strcmp(moving->value, "Q") && !strcmp(base->value, "K"))) {     // se moving é Q e base é K
            return true;
        }
    }
    else {
        if(base->suit == moving->suit &&                                     // se os naipes são iguais
           (atoi(base->value) + 1 == atoi(moving->value) ||                  // se o valor de base é moving + 1
            !strcmp(base->value, "A") && atoi(moving->value) == 2 ||         // se base é A e moving é 2
            atoi(base->value) == 10   && !strcmp(moving->value, "J") ||      // se base é 10 e moving é J
            !strcmp(base->value, "J") && !strcmp(moving->value, "Q") ||      // se base é J e moving é Q
            !strcmp(base->value, "Q") && !strcmp(moving->value, "K"))) {     // se base é Q e moving é K
            return true;
        }
    }

    return false;
}

void Game_moveToFoundation(int origin, char *card_value) {
    Card *moving_card = Stack_popCards(Game_stacks[origin], toupperstr(card_value));

    if(moving_card) {
        if(Stack_isEmpty(Game_stacks[FOUNDATION + moving_card->suit])) {
            if(!strcmp(moving_card->value, "A")) {
                Stack_pushCards(Game_stacks[FOUNDATION + moving_card->suit], moving_card);
            }
            else {
                Stack_returnUnusedCard(Game_stacks[origin], moving_card);
                strcpy(error_message, "Invalid movement!1");
            }
        }
        else {
            Card *top = Game_stacks[FOUNDATION + moving_card->suit]->top;

            if(strcmp(top->value, "K")) {
                if(isMovementValid(top, moving_card, FOUNDATION)) {
                    Stack_pushCards(Game_stacks[FOUNDATION + moving_card->suit], moving_card);
                }
                else {
                    strcpy(error_message, "Invalid movement!2");
                    Stack_returnUnusedCard(Game_stacks[origin], moving_card);
                }
            }
            else {
                strcpy(error_message, "Invalid movement!3");
                Stack_returnUnusedCard(Game_stacks[origin], moving_card);
            }

            top = NULL;
            free(top);
        }
    }
    else {
        strcpy(error_message, "Invalid movement!4");
    }

    moving_card = NULL;
    free(moving_card);
}

void Game_moveCards(int origin_tb, char *card_value, int finish_tb) {
    Card *moving_card = NULL;

    if(origin_tb == STOCK && finish_tb == STOCK_SIDE) {
        moving_card = Stack_popCards(Game_stacks[STOCK], NULL);

        if(moving_card) {
            moving_card->isTurned = false;
            Stack_pushCards(Game_stacks[STOCK_SIDE], moving_card);
        }
        else {
            while(!Stack_isEmpty(Game_stacks[STOCK_SIDE]))
                Stack_pushCards(Game_stacks[STOCK], Stack_popCards(Game_stacks[STOCK_SIDE], NULL));
        }
    }
    else {
        if(origin_tb == STOCK)
            moving_card = Stack_popCards(Game_stacks[origin_tb], NULL);
        else
            moving_card = Stack_popCards(Game_stacks[origin_tb], toupperstr(card_value));

        if(moving_card) {
            if(Stack_isEmpty(Game_stacks[finish_tb])) {
                if(!strcmp(moving_card->value, "K")) {
                    Stack_pushCards(Game_stacks[finish_tb], moving_card);
                }
                else {
                    Stack_returnUnusedCard(Game_stacks[origin_tb], moving_card);
                }
            }
            else {
                Card *top = Game_stacks[finish_tb]->top;

                if(isMovementValid(top, moving_card, TABLEAU)) {
                    Stack_pushCards(Game_stacks[finish_tb], moving_card);
                }
                else {
                    strcpy(error_message, "Invalid movement!6");
                    Stack_returnUnusedCard(Game_stacks[origin_tb], moving_card);
                }

                top = NULL;
                free(top);
            }
        }
        else {
            strcpy(error_message, "Card not found!");
        }
    }

    moving_card = NULL;
    free(moving_card);
}

bool Game_isWon() {
    for(int i = FOUNDATION; i < FOUNDATION + 4; i++)
        if(Game_stacks[i]->size < 13)
            return false;

    return true;
}

static void freeCards(Card *c) {
    if(c) {
        freeCards(c->next);
        free(c);
    }
}

void Game_freeStacks() {
    for(int i = 0; i < NONE; i++) {
        freeCards(Game_stacks[i]->first);
        free(Game_stacks[i]);
    }
    free(Game_stacks);
}
