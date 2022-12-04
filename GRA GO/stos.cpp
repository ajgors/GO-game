#include <stdio.h>
#include <stdlib.h>

typedef int stack_el_t;  // Stos przechowuje elementy typu in w polu value
// Warto��, kt�ra zwr�ci funkcja pop() w przypadku pr�by usuwania elementu
// z pustego stosu:
#define STACK_ERROR -1

// Typ struktury, opisuj�cy list� jednokierunkow� liczb ca�kowitych.
// G�owa listy ma nieokre�lon� warto�� pola "value".
// Lista jest wykorzystana do implementacji stosu.
// W naszej implementacji element na szczycie stosu, to ten
// wskazywany przez pole next nale��ce do "g�owy" listy.
typedef struct list {
    stack_el_t value;
    struct list* next;
} stack_t;

// Inicjalizacja stosu o "g�owie" head.
void init_stos(stack_t* head) {
    head->next = NULL;
}

// Operacja wstawia element z warto�ci� value na szczyt stosu.
void push(stack_t* l, stack_el_t value) {
    stack_t* x = (stack_t*)malloc(sizeof(stack_t));

    if (x) {
        x->value = value;
        x->next = l->next;
        l->next = x;
    }
}

// Usuwanie elementu ze szczytu stosu.
// Funkcja zwraca warto�� pola value usuni�tego elementu, lub
// warto�� STACK_ERROR w przypadku gdy stos jest pusty i nie by�o mo�liwe
// usuni�cie elementu. Uwaga: jest to implementacja b��dna je�li
// -1 jest dopuszczaln� warto�ci� dla pola value.
stack_el_t pop(stack_t* l) {
    if (l->next == NULL)
        return -1;   // Stos jest pusty.
    stack_el_t el = l->next->value;
    void* x = l->next;
    l->next = l->next->next;
    free(x);
    return el;
}

// Wypisuje zawarto�� stosu "h", zaczynaj�c od element�w na jego szczycie.
void print(stack_t* h) {
    h = h->next; // Pomijamy warto�� nieokre�lon� w g�owie.
    while (h != NULL) {
        printf("%d ", h->value);
        h = h->next;
    }
    printf("\n");
}


