#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "game.h"
#define GIVE_UP 1

Stack **Game_stacks;
char error_message[100];
int flag = 0;

Stack **Game_startStacks(Card **deck) {
    Stack **gameStacks = Stack_init(13);
    int initialPos, cnt = 0;

    // generate the tableau
    for(int i = 0; i < 7; i++) {
        gameStacks[i]->type = TABLEAU;
        initialPos = cnt;

        for(int j = 0; j < i; j++) {
            cnt++;
            deck[cnt - 1]->next = deck[cnt];
        }
        deck[cnt++]->isTurned = false;

        Stack_pushCards(gameStacks[i], deck[initialPos]);
    }

    // generate the empty foundations
    for(int i = FOUNDATION; i < FOUNDATION + 4; i++)
        gameStacks[i]->type = FOUNDATION;

    // generate the stock and stock_side
    initialPos = cnt;
    while(cnt < 51) {
        deck[cnt]->isTurned = false;
        cnt++;
        deck[cnt]->isTurned = false;
        deck[cnt - 1]->next = deck[cnt];
    }

    Stack_pushCards(gameStacks[STOCK], deck[initialPos]);
    gameStacks[STOCK]->type = STOCK;
    gameStacks[STOCK_SIDE]->type = STOCK_SIDE;

    return gameStacks;
}

void Game_init() {
    Card **deck = Card_generateDeck();
    Game_stacks = Game_startStacks(deck);
    UI_initScreen(Game_stacks);
    UI_printLogo();
}

static int getCardValue(char *value) {
    int num = atoi(value); 
    if(num > 0 && num < 11)
        return num - 1;

    char letter = toupper(value[0]);
    if(letter == 'A') return 0;
    if(letter == 'J') return 10;
    if(letter == 'Q') return 11; 
    if(letter == 'K') return 12;

    return -10;
}

void Game_input() {
    char move[10], value[10], to[20];

    printf("%s\n", error_message);
    strcpy(error_message, "");
    printf("from: ");
    scanf(" %s", move);

    if(!strcmp(move, "exit")) {
        flag = GIVE_UP;
    }
    if(!strcmp(move, "solve")) {
        Game_solve();
    }
    else if(!strcmp(move, "--help")) {
        // Game_helpMessage();
    }
    else if(!strcmp(move, "n")) {
        Game_moveCards(STOCK, -1, STOCK_SIDE);
        UI_updateScreen(STOCK, STOCK_SIDE);
    }
    else if(!strcmp(move, "s")) {
        printf("to: ");
        scanf(" %s", to);

        if(atoi(to) > 0 && atoi(move) < 8) {
            Game_moveCards(STOCK_SIDE, -1, atoi(to) - 1);
            UI_updateScreen(STOCK_SIDE, atoi(to) - 1);
        }
        else if(!strcmp(to, "f")) {
            StackType st = Game_stacks[STOCK_SIDE]->top->suit;
            Game_moveToFoundation(STOCK_SIDE, -1);
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
            Game_moveCards(atoi(move) - 1, getCardValue(value), atoi(to) - 1);
            UI_updateScreen(atoi(move) - 1, atoi(to) - 1);
        }
        else if(!strcmp(to, "f")) {
            StackType st = Game_stacks[atoi(move) - 1]->top->suit;
            Game_moveToFoundation(atoi(move) - 1, getCardValue(value));
            UI_updateScreen(atoi(move) - 1, FOUNDATION + st);
        }
        else
            strcpy(error_message, "Invalid input!2");
    }
    else {
        strcpy(error_message, "Invalid input!3");
    }
}

static bool isMovementValid(Card *base, Card *moving, StackType Stype) {
    if(!base && !moving || Stype != TABLEAU && Stype != FOUNDATION)
        return false;

    if(Stype == TABLEAU) {
        if((base->suit == (moving->suit + 1) % 4 || 
            base->suit == (moving->suit + 3) % 4) &&
            moving->value == base->value - 1) { 
            return true;
        }
    }
    else {
        if(base->suit == moving->suit && base->value == moving->value - 1) { // se os naipes são iguais
            return true;
        }
    }

    return false;
}

void Game_moveToFoundation(int origin, int card_value) {
    Card *moving_card = Stack_popCards(Game_stacks[origin], card_value);

    if(moving_card) {
        if(Stack_isEmpty(Game_stacks[FOUNDATION + moving_card->suit])) {
            // if(!strcmp(moving_card->value, "A")) {
            if(moving_card->value == 0) {
                Card_turn(Game_stacks[origin]->top);
                Stack_pushCards(Game_stacks[FOUNDATION + moving_card->suit], moving_card);
            }
            else {
                Stack_returnUnusedCard(Game_stacks[origin], moving_card);
                strcpy(error_message, "Invalid movement!1");
            }
        }
        else {
            Card *top = Game_stacks[FOUNDATION + moving_card->suit]->top;

            // if(strcmp(top->value, "K")) {
            if(top->value != 12) {
                if(isMovementValid(top, moving_card, FOUNDATION)) {
                    Card_turn(Game_stacks[origin]->top);
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

void Game_moveCards(int origin_tb, int card_value, int finish_tb) {
    Card *moving_card = NULL;

    if(origin_tb == STOCK && finish_tb == STOCK_SIDE) {
        moving_card = Stack_popCards(Game_stacks[STOCK], -1);

        if(moving_card) {
            moving_card->isTurned = false;
            Stack_pushCards(Game_stacks[STOCK_SIDE], moving_card);
        }
        else {
            while(!Stack_isEmpty(Game_stacks[STOCK_SIDE]))
                Stack_pushCards(Game_stacks[STOCK], Stack_popCards(Game_stacks[STOCK_SIDE], -1));
        }
    }
    else {
        moving_card = Stack_popCards(Game_stacks[origin_tb], card_value);

        if(moving_card) {
            if(Stack_isEmpty(Game_stacks[finish_tb])) {
                // if(!strcmp(moving_card->value, "K")) {
                if(moving_card->value == 12) {
                    Card_turn(Game_stacks[origin_tb]->top);
                    Stack_pushCards(Game_stacks[finish_tb], moving_card);
                }
                else {
                    Stack_returnUnusedCard(Game_stacks[origin_tb], moving_card);
                }
            }
            else {
                Card *top = Game_stacks[finish_tb]->top;

                if(isMovementValid(top, moving_card, TABLEAU)) {
                    Card_turn(Game_stacks[origin_tb]->top);
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
    if(flag == GIVE_UP)
        return true;

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

static bool isGameSolved() {
    for(int i = 0; i < 7; i++) {
        if(Stack_isEmpty(Game_stacks[i])) continue;
        for(Card *aux = Game_stacks[i]->first; aux->next != NULL; aux = aux->next) {
            if(aux->isTurned || aux->value != aux->next->value - 1)
                return false;
        }
    }

    return true;
}

void Game_solve() {
    if(!isGameSolved()) return;
    while(!Game_isWon()) {
        for(int i = 0; i < 7; i++) {
            if(!Stack_isEmpty(Game_stacks[i]))
                Game_moveToFoundation(i, -1);
        }
    }
}