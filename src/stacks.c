#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "card.h"
#include "stacks.h"

Stack *Stack_init() {
    Stack *s = malloc(sizeof(Stack));

    s->size = 0;
    s->first = NULL;

    return s;
}

void Stack_pushCards(Stack *stack, Card *card) {
    Card *temp = stack->first;

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

Card **Stack_generateGame(Card **deck);

int  **Stack_print(Stack *stack);
