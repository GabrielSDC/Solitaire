#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "game.h"

Game *game = NULL;

void Game_init() {
    game = malloc(sizeof(Game));
    
    game->score = 0;
    strcpy(game->error_message, "");
    game->is_playing = true;
    game->last_move = NULL;

    Card **deck = Card_generateDeck();
    game->stacks = Game_startStacks(deck);

    UI_initScreen(game->stacks);
    UI_printLogo();
}

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

    printf("%s\n", game->error_message);
    strcpy(game->error_message, "");
    printf("from: ");
    scanf(" %s", move);

    if(!strcmp(move, "exit")) {
        game->is_playing = false;
    }
    else if(!strcmp(move, "solve")) {
        Game_solve();
    }
    else if(!strcmp(move, "undo")) {
        Game_undoMovement();
    }
    else if(!strcmp(move, "--help")) {
        // Game_helpMessage();
    }
    else if(!strcmp(move, "n")) {
        Game_moveCards(STOCK, -1, STOCK_SIDE);
    }
    else if(!strcmp(move, "s")) {
        printf("to: ");
        scanf(" %s", to);

        if(atoi(to) > 0 && atoi(move) < 8) {
            Game_moveCards(STOCK_SIDE, -1, atoi(to) - 1);
        }
        else if(!strcmp(to, "f")) {
            Game_moveToFoundation(STOCK_SIDE, -1);
        }
        else
            strcpy(game->error_message, "Invalid input!");
    }
    else if(atoi(move) > 0 && atoi(move) < 8) {
        printf("Card: ");
        scanf(" %s", value);
        printf("to: ");
        scanf(" %s", to);

        if(atoi(to) > 0 && atoi(to) < 8) {
            Game_moveCards(atoi(move) - 1, getCardValue(value), atoi(to) - 1);
        }
        else if(!strcmp(to, "f")) {
            Game_moveToFoundation(atoi(move) - 1, getCardValue(value));
        }
        else
            strcpy(game->error_message, "Invalid input!");
    }
    else {
        strcpy(game->error_message, "Invalid input!");
    }
}

static bool isMovementValid(Card *base, Card *moving, StackType Stype) {
    if(!base && !moving || Stype != TABLEAU && Stype != FOUNDATION)
        return false;

    if(Stype == TABLEAU) {
        return (base->suit == (moving->suit + 1) % 4 || 
                base->suit == (moving->suit + 3) % 4) &&
                moving->value == base->value - 1;
    } 

    if(Stype == FOUNDATION) {
        return base->suit == moving->suit && base->value == moving->value - 1;
    }

    return false;
}

static void newMove(int origin, int finish, Card *moving_card) {
    Move *new_move = malloc(sizeof(Move));
    
    new_move->origin_stack    = origin;
    new_move->finish_stack    = finish;
    new_move->turned_new_card = Card_turn(game->stacks[origin]->top);
    new_move->previous        = game->last_move;
    new_move->card_value      = moving_card ? moving_card->value : -1;
    
    game->last_move = new_move;
    Stack_pushCards(game->stacks[finish], moving_card);
    UI_updateScreen(origin, finish);
}

Errors Game_moveToFoundation(int origin, int card_value) {
    Card *moving_card = Stack_popCards(game->stacks[origin], card_value);

    if(!moving_card) {
        strcpy(game->error_message, "Card not found!");
        return CARD_NOT_FOUND;
    }
    
    int suited_foundation = FOUNDATION + moving_card->suit;
    if(Stack_isEmpty(game->stacks[suited_foundation])) {
        if(moving_card->value == 0) {
            newMove(origin, suited_foundation, moving_card);
            moving_card = NULL;
            return NO_ERROR;
        }
    }
    else {
        Card *top = game->stacks[suited_foundation]->top;
        if(isMovementValid(top, moving_card, FOUNDATION)) {
            newMove(origin, suited_foundation, moving_card);
            top = NULL, moving_card = NULL;
            return NO_ERROR;
        }
    }

    strcpy(game->error_message, "Invalid movement!");
    Stack_returnUnusedCard(game->stacks[origin], moving_card);
    
    moving_card = NULL;
    return INVALID_MOVEMENT;
}

Errors Game_moveCards(int origin_tb, int card_value, int finish_tb) {
    Card *moving_card = NULL;

    if(origin_tb == STOCK && finish_tb == STOCK_SIDE) {
        moving_card = Stack_popCards(game->stacks[STOCK], -1);

        if(moving_card) {
            newMove(STOCK, STOCK_SIDE, moving_card);
        }
        else {
            while(!Stack_isEmpty(game->stacks[STOCK_SIDE])) {
                moving_card = Stack_popCards(game->stacks[STOCK_SIDE], -1);
                Stack_pushCards(game->stacks[STOCK], moving_card);
            }
            newMove(STOCK_SIDE, STOCK, NULL);
        }
        moving_card = NULL;
        return NO_ERROR;
    }
    else {
        moving_card = Stack_popCards(game->stacks[origin_tb], card_value);

        if(!moving_card) {
            strcpy(game->error_message, "Card not found!");
            return CARD_NOT_FOUND;
        }

        if(Stack_isEmpty(game->stacks[finish_tb])) {
            if(moving_card->value == 12) {
                newMove(origin_tb, finish_tb, moving_card);
                moving_card = NULL;
                return NO_ERROR;
            }
        }
        else {
            Card *top = game->stacks[finish_tb]->top;
            if(isMovementValid(top, moving_card, TABLEAU)) {
                newMove(origin_tb, finish_tb, moving_card);
                top = NULL, moving_card = NULL;
                return NO_ERROR;
            }
        }
    }
    
    strcpy(game->error_message, "Invalid movement!");
    Stack_returnUnusedCard(game->stacks[origin_tb], moving_card);

    moving_card = NULL;
    return INVALID_MOVEMENT;
}

void Game_undoMovement() {
    Move *last = game->last_move;
    
    if(!last) return;

    Card *moving_card = NULL;
    int origin = last->origin_stack;
    int finish = last->finish_stack;
    int card_value = last->card_value;

    if(card_value == -1) {
        while(!Stack_isEmpty(game->stacks[STOCK])) {
            moving_card = Stack_popCards(game->stacks[STOCK], -1);
            Stack_pushCards(game->stacks[STOCK_SIDE], moving_card);
        }
    }
    else {
        if(!Stack_isEmpty(game->stacks[origin]) && last->turned_new_card) {
            game->stacks[origin]->top->isTurned = true;
        }
        moving_card = Stack_popCards(game->stacks[finish], card_value);
        Stack_pushCards(game->stacks[origin], moving_card);
    }

    game->last_move = last->previous;
    last->previous = NULL;
    free(last);
    last = NULL, moving_card = NULL;

    UI_updateScreen(origin, finish);
}

bool Game_isWon() {
    if(!game->is_playing)
        return true;

    for(int i = FOUNDATION; i < FOUNDATION + 4; i++)
        if(game->stacks[i]->size < 13)
            return false;

    return true;
}

static void freeCards(Card *c) {
    if(c) {
        freeCards(c->next);
        free(c);
        c = NULL;
    }
}

static void freeMoves(Move *m) {
    if(m) {
        freeMoves(m->previous);
        free(m);
        m = NULL;
    }
}

void Game_freeStacks() {
    for(int i = 0; i < NONE; i++) {
        freeCards(game->stacks[i]->first);
        free(game->stacks[i]);
        game->stacks[i] = NULL;
    }
    free(game->stacks);
    game->stacks = NULL;

    freeMoves(game->last_move);
    game->last_move = NULL;

    free(game);
    game = NULL;
}

static bool isGameSolved() {
    for(int i = 0; i < 7; i++) {
        if(Stack_isEmpty(game->stacks[i])) continue;
        for(Card *aux = game->stacks[i]->first; aux->next; aux = aux->next) {
            if(aux->isTurned || 
               aux->value != aux->next->value + 1 ||
               aux->suit != (aux->next->suit + 1) % 4 && 
               aux->suit != (aux->next->suit + 3) % 4) {
                aux = NULL;
                return false;
            }
        }
    }

    return true;
}

void Game_solve() {
    if(!isGameSolved()) return;
    while(!Game_isWon()) {
        for(int i = 0; i < 7; i++) {
            if(!Stack_isEmpty(game->stacks[i])) {
                Game_moveToFoundation(i, -1);
            }
        }
        Game_moveToFoundation(STOCK, -1);
        Game_moveToFoundation(STOCK_SIDE, -1);
        Game_moveCards(STOCK, -1, STOCK_SIDE);
    }
    
    for(int i = 0; i < NONE; i++) {
        UI_updateScreen(i, i);
    }
}

static void personalizarDeck(Stack **gameStacks, Card **deck) {
    Card *aux = NULL;
    for(int i = 12; i >= 0; i--) {
        for(Suit j = HEARTS; j <= CLUBS; j++) {
            if(i % 2)
                aux = Card_get(deck, i, j);
            else
                aux = Card_get(deck, i, (j + 1) % 4);

            Card_turn(aux);
            Stack_pushCards(gameStacks[j], aux);
        }
    }
    aux = NULL;
}