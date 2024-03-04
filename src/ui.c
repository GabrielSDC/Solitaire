#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui.h"

Stack **stacks = NULL;

char *screen[60][7];

int  longest_stack = 7;

const char *cardValues[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

const char *suits[] = {"♥", "♠", "♦", "♣"};

char *BLANK_SPACE =    "      ";

char *EMPTY_FOUND[] = {"┌A───┐",
                       "│    │",
                       "│    │",
                       "└───A┘"};

char *EMPTY_TABLE[] = {"┌K───┐",
                       "│    │",
                       "│    │",
                       "└───K┘"};

char *EMPTY_STOCK[] = {"┌────┐",
                       "│    │",
                       "│    │",
                       "└────┘"};

char *CARD_BLACK[] =  {"\e[47;30m│    │\e[0m", 
                       "\e[47;30m│    │\e[0m",
                       "\e[47;30m└────┘\e[0m"};

char *CARD_RED[] =    {"\e[47;31m│    │\e[0m", 
                       "\e[47;31m│    │\e[0m",
                       "\e[47;31m└────┘\e[0m"};

char *TURNED_CARD[] = {"\e[44;37m┌────┐\e[0m",
                       "\e[44;37m│    │\e[0m",
                       "\e[44;37m│    │\e[0m",
                       "\e[44;37m└────┘\e[0m"};

char *TITLE[] = {
		"♢ ♥ ♤ ♠ ♧ ♦ ♡ ♣ ♢ ♥ ♤ ♠ ♧ ♦ ♡ ♣ ♢ ♥ ♤ ♠ ♧ ♦ ♡ ♣ ♢ ♥ ♤ ♠ ♧ ♦ ♡ ♣ ♢ ♥ ♤ ♠ ♧ ♦ ♡ ♣ ♢ ♥ ♤ ♠ ♧ ♦ ♡ ♣ ♢ ♥ ♤ ♠ ♧ ♦ ♡ ♣ ♢ ♥ ♤\n",
		"    ██████████                  ██████      ████        ████                      ████",
		"  ████      ████                  ████                  ████",
		"  ████              ████████      ████    ██████    ████████████    ████████    ██████    ████  ████    ████████",
		"    ██████████    ████    ████    ████      ████        ████              ████    ████    ██████      ████    ████",
		"            ████  ████    ████    ████      ████        ████        ██████████    ████    ████        ████████████",
		"  ████      ████  ████    ████    ████      ████        ████      ████    ████    ████    ████        ████",
		"    ██████████      ████████    ████████  ████████      ████        ██████████  ████████  ████          ██████████\n",
		"♢ ♥ ♤ ♠ ♧ ♦ ♡ ♣ ♢ ♥ ♤ ♠ ♧ ♦ ♡ ♣ ♢ ♥ ♤ ♠ ♧ ♦ ♡ ♣ ♢ ♥ ♤ ♠ ♧ ♦ ♡ ♣ ♢ ♥ ♤ ♠ ♧ ♦ ♡ ♣ ♢ ♥ ♤ ♠ ♧ ♦ ♡ ♣ ♢ ♥ ♤ ♠ ♧ ♦ ♡ ♣ ♢ ♥ ♤"
	}; 

void UI_initScreen(Stack **s) {
    stacks = s;

    for(int i = 0; i < 4; i++) {
        screen[i][0] = TURNED_CARD[i];
    }

    for(int i = 0; i < 4; i++) {
        screen[i][1] = EMPTY_STOCK[i];
    }

    for(int i = 0; i < 4; i++) {
        screen[i][2] = BLANK_SPACE;
    }

    for(int i = 3; i < 7; i++) {
        for(int j = 0; j < 4; j++) {
            screen[j][i] = EMPTY_FOUND[j];
        }
    }

    for(int i = 0; i < 7; i++) {
        int pos = 4;

        if(Stack_isEmpty(s[i])) {
            for(int j = 0; j < 4; j++)
             screen[pos++][i] = EMPTY_TABLE[j];
        }
        else {
            for(Card *temp = s[i]->first; temp->next; temp = temp->next) {
                screen[pos++][i] = TURNED_CARD[0];
            }

            screen[pos++][i] = UI_getCard(s[i]->top);
            
            if(s[i]->top->suit == HEARTS || s[i]->top->suit == DIAMONDS) {
                for(int j = 0; j < 3; j++) {
                    screen[pos++][i] = CARD_RED[j];
                }
            }
            else {
                for(int j = 0; j < 3; j++) {
                    screen[pos++][i] = CARD_BLACK[j];
                }
            }
        }

        while(pos < 60) {
            screen[pos++][i] = BLANK_SPACE;
        }
    }
}

void UI_updateScreen(int origin_tb, int finish_tb) {
    if(origin_tb == STOCK || finish_tb == STOCK) {
        if(!Stack_isEmpty(stacks[STOCK])) {
            for(int i = 0; i < 4; i++) {
                screen[i][0] = TURNED_CARD[i];
            }
        }
        else {
            for(int i = 0; i < 4; i++) {
                screen[i][0] = EMPTY_STOCK[i];
            }
        }
    }
    
    if(origin_tb == STOCK_SIDE || finish_tb == STOCK_SIDE) {
        if(!Stack_isEmpty(stacks[STOCK_SIDE])) {
            screen[0][1] = UI_getCard(stacks[STOCK_SIDE]->top);
            if(stacks[STOCK_SIDE]->top->suit == HEARTS || stacks[STOCK_SIDE]->top->suit == DIAMONDS) {
                for(int i = 0; i < 3; i++) {
                    screen[i + 1][1] = CARD_RED[i];
                }
            }
            else {
                for(int i = 0; i < 3; i++) {
                    screen[i + 1][1] = CARD_BLACK[i];
                }
            }
        }
        else {
            for(int i = 0; i < 4; i++) {
                screen[i][1] = EMPTY_STOCK[i];
            }
        }
    }

    if(stacks[origin_tb]->type == FOUNDATION) {
        int f = origin_tb - 4;
        
        if(!Stack_isEmpty(stacks[origin_tb])) {
            screen[0][f] = UI_getCard(stacks[origin_tb]->top);
            if(stacks[origin_tb]->top->suit == HEARTS || stacks[origin_tb]->top->suit == DIAMONDS) {
                for(int i = 0; i < 3; i++) {
                    screen[i + 1][f] = CARD_RED[i];
                }
            }
            else {
                for(int i = 0; i < 3; i++) {
                    screen[i + 1][f] = CARD_BLACK[i];
                }
            }
        }
        else {
            for(int i = 0; i < 4; i++) {
                screen[i][f] = EMPTY_FOUND[i];
            }
        }
    }

    if(stacks[finish_tb]->type == FOUNDATION) {
        int f = finish_tb - 4;

        if(!Stack_isEmpty(stacks[finish_tb])) {
            screen[0][f] = UI_getCard(stacks[finish_tb]->top);
            if(stacks[finish_tb]->top->suit == HEARTS || stacks[finish_tb]->top->suit == DIAMONDS) {
                for(int i = 0; i < 3; i++) {
                    screen[i + 1][f] = CARD_RED[i];
                }
            }
            else {
                for(int i = 0; i < 3; i++) {
                    screen[i + 1][f] = CARD_BLACK[i];
                }
            }
        }
        else {
            for(int i = 0; i < 4; i++) {
                screen[i][f] = EMPTY_FOUND[i];
            }
        }
    }

    if(stacks[origin_tb]->type == TABLEAU) { 
        int pos = 4;

        if(!Stack_isEmpty(stacks[origin_tb])) {
            for(Card *temp = stacks[origin_tb]->first; !temp->isOnTop; temp = temp->next, pos++) {
                screen[pos][origin_tb] = UI_getCard(temp);
            }
            screen[pos][origin_tb] = UI_getCard(stacks[origin_tb]->top);
            pos++;

            if(stacks[origin_tb]->top->suit == HEARTS || stacks[origin_tb]->top->suit == DIAMONDS) {
                for(int i = 0; i < 3; i++, pos++) {
                    screen[pos][origin_tb] = CARD_RED[i];
                }
            }
            else {
                for(int i = 0; i < 3; i++, pos++) {
                    screen[pos][origin_tb] = CARD_BLACK[i];
                }
            }

            if(stacks[origin_tb]->size > longest_stack)
                longest_stack = stacks[origin_tb]->size;
        }
        else {
            for(int i = 0; i < 4; i++, pos++) {
                screen[pos][origin_tb] = EMPTY_TABLE[i];
            }
        }

        while(pos < 60) {
            screen[pos][origin_tb] = BLANK_SPACE;
            pos++;
        }
    }

    if(stacks[finish_tb]->type == TABLEAU) { 
        int pos = 4;

        if(!Stack_isEmpty(stacks[finish_tb])) {
            for(Card *temp = stacks[finish_tb]->first; !temp->isOnTop; temp = temp->next) {
                screen[pos++][finish_tb] = UI_getCard(temp);
            }
            screen[pos][finish_tb] = UI_getCard(stacks[finish_tb]->top);
            pos++;

            if(stacks[finish_tb]->top->suit == HEARTS || stacks[finish_tb]->top->suit == DIAMONDS) {
                for(int i = 0; i < 3; i++, pos++) {
                    screen[pos][finish_tb] = CARD_RED[i];
                }
            }
            else {
                for(int i = 0; i < 3; i++, pos++) {
                    screen[pos][finish_tb] = CARD_BLACK[i];
                }
            }

            if(stacks[finish_tb]->size > longest_stack)
                longest_stack = stacks[finish_tb]->size;
        }
        else {
            for(int i = 0; i < 4; i++, pos++) {
                screen[pos][finish_tb] = EMPTY_TABLE[i];
            }
        }

        while(pos < 60) {
            screen[pos][finish_tb] = BLANK_SPACE;
            pos++;
        }
    }
}

void UI_printLogo() {
    system("clear");
    for(int i = 0; i < 9; i++) {
        printf("%s\n", TITLE[i]);
    }

    printf("\n\n\t\t\t\t\t press ENTER to start\n");
    scanf("%*c");
}

void UI_printScreen() {
    system("clear");

    for(int i = 0; i < longest_stack + 8; i++) {
        for(int j = 0; j < 7; j++) {
            printf("%s ", screen[i][j]);
        }
        printf("%s", i == 3 ? "\n\n":"\n");
    }
}

char *UI_getCard(Card *card) {
    if(card->isTurned)
        return TURNED_CARD[0];

    char *cardInfo = malloc(sizeof(char) * 30);

    strcat(cardInfo, "\e[47");
    
    if(card->suit % 2)
        strcat(cardInfo, ";30m┌");
    else
        strcat(cardInfo, ";31m┌");
    
    strcat(cardInfo, cardValues[card->value]);
    strcat(cardInfo, suits[card->suit]);
    
    if(card->value != 9)
        strcat(cardInfo, "─");
    
    strcat(cardInfo, "─┐\e[0m");

    return cardInfo;
}
