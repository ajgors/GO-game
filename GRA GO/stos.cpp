#include <stdio.h>
#include <stdlib.h>

typedef int stack_el_t;  // Stos przechowuje elementy typu in w polu value
// Wartoœæ, która zwróci funkcja pop() w przypadku próby usuwania elementu
// z pustego stosu:
#define STACK_ERROR -1

// Typ struktury, opisuj¹cy listê jednokierunkow¹ liczb ca³kowitych.
// G³owa listy ma nieokreœlon¹ wartoœæ pola "value".
// Lista jest wykorzystana do implementacji stosu.
// W naszej implementacji element na szczycie stosu, to ten
// wskazywany przez pole next nale¿¹ce do "g³owy" listy.
typedef struct list {
    stack_el_t value;
    struct list* next;
} stack_t;

// Inicjalizacja stosu o "g³owie" head.
void init_stos(stack_t* head) {
    head->next = NULL;
}

// Operacja wstawia element z wartoœci¹ value na szczyt stosu.
void push(stack_t* l, stack_el_t value) {
    stack_t* x = (stack_t*)malloc(sizeof(stack_t));

    if (x) {
        x->value = value;
        x->next = l->next;
        l->next = x;
    }
}

// Usuwanie elementu ze szczytu stosu.
// Funkcja zwraca wartoœæ pola value usuniêtego elementu, lub
// wartoœæ STACK_ERROR w przypadku gdy stos jest pusty i nie by³o mo¿liwe
// usuniêcie elementu. Uwaga: jest to implementacja b³êdna jeœli
// -1 jest dopuszczaln¹ wartoœci¹ dla pola value.
stack_el_t pop(stack_t* l) {
    if (l->next == NULL)
        return -1;   // Stos jest pusty.
    stack_el_t el = l->next->value;
    void* x = l->next;
    l->next = l->next->next;
    free(x);
    return el;
}

// Wypisuje zawartoœæ stosu "h", zaczynaj¹c od elementów na jego szczycie.
void print(stack_t* h) {
    h = h->next; // Pomijamy wartoœæ nieokreœlon¹ w g³owie.
    while (h != NULL) {
        printf("%d ", h->value);
        h = h->next;
    }
    printf("\n");
}


