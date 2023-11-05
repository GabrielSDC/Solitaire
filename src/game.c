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
}

void Game_input() {
    char move[10], value[10], to[20];

    printf("%s\n", error_message);
    strcpy(error_message, "");
    printf("Move: ");
    scanf(" %s", move);

    if(!strcmp(move, "--help")) {
        // Game_helpMessage();
    }
    else if(!strcmp(move, "s")) {
        // Game_updateStock();
    }
    else if(atoi(move) > 0){
        scanf(" %s%s", value, to);
        Game_moveCards(atoi(move) - 1, value, atoi(to) - 1);
    }
    else {
        strcpy(error_message, "Invalid input!");
    }

}

static char *toupperstr(char *str) {
    for(int i = 0; i < strlen(str); i++) {
        str[i] = toupper(str[i]);
    }

    return str;
}

void Game_moveCards(int origin_tb, char card_value[], int finish_tb) {
    Card *moving_card = Stack_popCards(Game_stacks[origin_tb], toupperstr(card_value));

    if(moving_card) {
        if(Game_stacks[finish_tb]->size == 0) {
            if(!strcmp(moving_card->value, "K"))
                Stack_pushCards(Game_stacks[finish_tb], moving_card);
            else {
                strcpy(error_message, "Invalid movement!");
                Stack_returnUnusedCard(Game_stacks[origin_tb], moving_card);
            }
        }
        else {
            Card *temp = Game_stacks[finish_tb]->first;

            while(!temp->isOnTop)
                temp = temp->next;
            
            if(temp->suit == (moving_card->suit + 1) % 4 ||
               temp->suit == (moving_card->suit + 3) % 4)
                Stack_pushCards(Game_stacks[finish_tb], moving_card);
            else {
                strcpy(error_message, "Invalid movement!");
                Stack_returnUnusedCard(Game_stacks[origin_tb], moving_card);
            }
        }
    }
    else {
        strcpy(error_message, "Card not found!");
    }
}

void Game_printTableau() {
    for(int i = 0; i < 7; i++) {
        printf("%d: ", i + 1);
        for(Card *temp = Game_stacks[TABLEAU + i]->first; temp != NULL; temp = temp->next) {
            printf("%s ", Card_printable(temp));
        }
        printf("%d\n", Game_stacks[TABLEAU + i]->size);
    }
}

void print_stack(Stack *stack) {
    for(Card *temp = stack->first; temp != NULL; temp = temp->next) {
        printf("%s\n", Card_printable(temp));

        if(temp->isOnTop) {
            if(temp->suit == CLUBS || temp->suit == SPADES) {
                printf("\e[0;30m%s\e[0m\n", UNTURNED_CARD_M);
                printf("\e[0;30m%s\e[0m\n", UNTURNED_CARD_B);
            }
            else {
                printf("\e[0;30m%s\e[0m\n", UNTURNED_CARD_B);
                printf("\e[0;30m%s\e[0m\n", UNTURNED_CARD_M);
            }
        }
    }
}

bool Game_isWon() {
    for(int i = FOUNDATION; i < FOUNDATION + 4; i++)
        if(Game_stacks[i]->size < 13)
            return false;

    return true;
}