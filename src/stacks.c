#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stacks.h"

Stack **Stack_init(int size) {
    Stack **s = malloc(sizeof(Stack*) * size);

    for(int i = 0; i < size; i++) {
        s[i] = malloc(sizeof(Stack));
        s[i]->size = 0;
        s[i]->first = NULL;
        s[i]->top = NULL;
        s[i]->type = NONE;
    }

    return s;
}

void Stack_pushCards(Stack *stack, Card *card) {
    Card *temp = NULL;

    if(!card)
        return;

    temp = stack->top;

    if(!temp) {
        stack->first = card;
    }
    else {
        temp->isOnTop = false;
        temp->next = card;
        temp = NULL;
        free(temp);
    }

    stack->size++;

    if(card->next == NULL) {
        card->isOnTop = true;
        stack->top = card;
    }
    else {
        temp = card;
        while(temp->next) {
            stack->size++;
            temp = temp->next;
        }

        temp->isOnTop = true;
        stack->top = temp;
        temp = NULL;
        free(temp);
    }
}

Card *Stack_popCards(Stack *stack, char *value) {
    Card *card = NULL, *anterior = NULL, *temp = NULL;

    if(!stack)
        return NULL;

    if(!Stack_isEmpty(stack)) {
        card = stack->first;

        if(value == NULL) {
            while(card->next) {
                anterior = card;
                card = card->next;
            }
            
            if(anterior) {
                anterior->isOnTop = true;
                anterior->next = NULL;
                stack->top = anterior;
            }
            else {
                stack->first = NULL;
                stack->top = NULL;
            }
            
            anterior = NULL;
            free(anterior);

            card->next = NULL;
            card->isOnTop = false;
            stack->size--;
            
            return card;
        }

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
                stack->top = anterior;
                
                anterior = NULL;
                free(anterior);
                
                temp = card;
                while(temp) {
                    stack->size--;
                    temp = temp->next;
                }

                temp = NULL;
                free(temp);
            }
            else {
                stack->first = NULL;
                stack->top = NULL;
                stack->size = 0;
            }

            temp = NULL;
            free(temp);
        }
    }

    return card;
}

// a consertar: quando uma carta é retornada, ela vira a anterior independente de ela já estar desvirada 
// antes da tentativa de movimentação acontecer
void Stack_returnUnusedCard(Stack *stack, Card *card) {
    if(!stack || !card)
        return;

    if(!Stack_isEmpty(stack)) {
        stack->top->isTurned = true;
        stack->top->isOnTop = false;
        stack->top->next = card;    
    }
    else {
        stack->first = card;
    }

    Card *temp = card;
    while(temp->next) {
        stack->size++;
        temp = temp->next;
    }

    stack->size++;
    stack->top = temp;

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

int  **Stack_print(Stack *stack);