#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "stacks.h"

Stack **Stack_init(int size) {
    Stack **s = malloc(sizeof(Stack*) * size);

    for(int i = 0; i < size; i++) {
        s[i] = malloc(sizeof(Stack));
        s[i]->size = 0;
        s[i]->first = NULL;
        s[i]->type = NONE;
    }

    return s;
}

void Stack_pushCards(Stack *stack, Card *card) {
    Card *temp = NULL;

    if(!card)
        return;

    temp = stack->first;

    if(!temp) {
        stack->first = card;
    }
    else {
        while(temp->next)
            temp = temp->next;

        temp->isOnTop = false;
        temp->next = card;
        temp = NULL;
        free(temp);
    }
    

    if(card->next == NULL) {
        card->isOnTop = true;
        stack->size++;
        // printf("%s.\n", Card_printable(card));
    }
    else {
        temp = card;
        while(temp->next) {
            // printf("%s ", Card_printable(temp));
            stack->size++;
            temp = temp->next;
        }
        stack->size++;

        // printf("%s.\n", Card_printable(temp));

        temp->isOnTop = true;
        temp = NULL;
        free(temp);
    }
}

Card *Stack_popCards(Stack *stack, char *value) {
    Card *card = NULL, *anterior = NULL, *temp = NULL;

    if(!Stack_isEmpty(stack) && value) {
        card = stack->first;

        while(card) {
            if(!strcmp(card->value, value) && !card->isTurned)
                break;
        
            anterior = card;
            card = card->next;
        }

        if(card) {
            if(anterior) {
                if(anterior->isTurned && stack->type == TABLEAU)
                    anterior->isTurned = false;
                anterior->isOnTop = true;
                anterior->next = NULL;
                
                anterior = NULL;
                free(anterior);
            }
            else {
                stack->first = NULL;
            }

            temp = card;
            while(temp) {
                stack->size--;
                temp = temp->next;
            }

            temp = NULL;
            free(temp);
        }
    }

    return card;
}

void Stack_returnUnusedCard(Stack *stack, Card *card) {
    Card *temp = stack->first;
    while(temp) {
        if(!temp->next)
            break;
        temp = temp->next;
    }
    temp->isTurned = true;
    temp->next = card;
    stack->size++;
    temp = NULL;
    free(temp);
}

bool Stack_isEmpty(Stack *stack) {
    return stack->size == 0;
}

Stack **Stack_generateGame(Card **deck) {
    Stack **gameStacks = Stack_init(13);
    int initialPos, cnt = 0;

    // generate the tableau
    for(int i = 0; i < 7; i++) {
        gameStacks[i]->type = TABLEAU;
        initialPos = cnt;
        // printf("%s ", Card_printable(deck[cnt]));

        for(int j = 0; j < i; j++) {
            cnt++;
            deck[cnt - 1]->next = deck[cnt];
            // printf("-> %s ", Card_printable(deck[cnt-1]->next));
        }
        // printf(".\n");
        deck[cnt++]->isTurned = false;

        Stack_pushCards(gameStacks[i], deck[initialPos]);
    }

    // generate the empty foundations
    for(int i = FOUNDATION; i < FOUNDATION + 4; i++)
        gameStacks[i]->type = FOUNDATION;

    // generate the stock and stock_side
    initialPos = cnt;
    while(cnt < 51) {
        cnt++;
        deck[cnt - 1]->next = deck[cnt];
    }

    Stack_pushCards(gameStacks[STOCK], deck[initialPos]);
    gameStacks[STOCK]->type = STOCK;
    gameStacks[STOCK_SIDE]->type = STOCK_SIDE;

    return gameStacks;
}

Card *Stack_getPos(Stack *stack, int position) {
    if(position >= stack->size)
        return NULL; 

    Card *card = stack->first;

    for(int i = 0; i < position; i++)
        card = card->next;

    return card;
}

int  **Stack_print(Stack *stack);