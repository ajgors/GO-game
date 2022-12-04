#pragma once
typedef int stack_el_t; 

#define STACK_ERROR -1

typedef struct list {
    stack_el_t value;
    struct list* next;
} stack_t;


void init_stos(stack_t* head);
void push(stack_t* l, stack_el_t value);
stack_el_t pop(stack_t* l);
void print(stack_t* h);


