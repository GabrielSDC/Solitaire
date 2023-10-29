#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "card.h"
#include "stacks.h"

Stack *Stack_init() {
    Stack *s = malloc(sizeof(Stack));

    s->size = 0;
    s->first = NULL;
    s->type = NONE;

    return s;
}

void Stack_pushCards(Stack *stack, Card *card) {
    Card *temp = stack->first;

    if(!temp) {
        card->isOnTop = true;
        card->isTurned = false;
        stack->first = card;
        return;
    }

    while(!temp->isOnTop)
        temp = temp->next;

    temp->isOnTop = false;
    temp->next = card;
    stack->size++;
    free(temp);

    if(card->next == NULL)
        card->isOnTop = true;
    else {
        temp = card;
        while(temp->next != NULL) {
            stack->size++;
            temp = temp->next;
        }

        temp->isOnTop = true;
        free(temp);
    }
}

Card *Stack_popCards(Stack *stack, char *value) {
    Card *card = NULL, *anterior = NULL;

    if(!Stack_isEmpty(stack) && value) {
        card = stack->first;

        while(card) {
            if(!strcmp(card->value, value))
                break;
        
            anterior = card;
            card = card->next;
        }

        if(card) {
            if(anterior) {
                if(anterior->isTurned)
                    anterior->isTurned = false;
                anterior->isOnTop = true;
                anterior->next = NULL;
            }

            while(card) {
                stack->size--;
                card = card->next;
            }
        }
    }

    return card;
}

bool Stack_isEmpty(Stack *stack) {
    return stack->size == 0;
}

Card **Stack_generateGame(Card **deck) {
    Stack gameStacks = malloc(sizeof(Stack) * 12);
    Card initialCard = NULL;
    int cnt = 0;

    // generate the tableau
    for(int i = 0; i < 7; i++) {
        gameStacks[i]->type = TABLEAU;
        initialCard = deck[cnt];

        for(int j = 0; j < i + 1; j++, cnt++) {
            if(j == i - 1) {
                deck[cnt]->isTurned = false;
            }
            else {
                deck[cnt]->next = deck[cnt + 1];
            }
        }

        Stack_pushCards(gameStacks[i], initialCard);
    }

    free(initialCard);

    // generate the empty foundations
    for(int i = 7; i < 10; i++)
        gameStack[i]->type = FOUNDATION;

    // generate the stock and stock_side
    initialCard = deck[cnt];
    while(cnt < 51)
        deck[cnt]->next = deck[++cnt];

    Stack_pushCards(gameStack[10], initialcard);
    free(initialCard);

    gameStack[10]->type = STOCK;
    gameSatck[11]->type = STOCK_SIDE;

    return stackGame;
}

int  **Stack_print(Stack *stack);
