#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

typedef struct {
	int x, y;
}coordinates;

//Struktura opisuj¹ca wektor koordynatów
//j-ta komórka wektora w (0 <= j < w.count) jest dostêpna jest jako w.ptr[j].
typedef struct {
	int allocated_size;			// rozmiar zaalokowanego bufora
	int count;					// liczba elementów w wektorze
	coordinates* ptr;           // wskaŸnik do pocz¹tku bufora
} vector_t;


// Inicjalizacja wektora *v. Pocz¹tkowo tablica bêdzie mia³a rozmiar 1.
void init_vector(vector_t* v) {
	v->count = 0;
	v->allocated_size = 1;
	v->ptr = (coordinates*)malloc(v->allocated_size * sizeof(coordinates));
}


// Realokacja wektora *v, tak aby bufor mia³ pojemnoœæ reallocate_size.
void reallocate(vector_t* v, int reallocate_size) {

	v->allocated_size = reallocate_size;
	coordinates* newPtr = (coordinates*)malloc(v->allocated_size * sizeof(coordinates));
	if (newPtr) {
		memcpy(newPtr, v->ptr, v->count * sizeof(coordinates));
		free(v->ptr);
		v->ptr = newPtr;
	}
}


// Umieszczenie wartoœci val jako nowego (koñcowego) elementu wektora *v.
void push_back(vector_t* v, coordinates val) {
	//Realokacja wymagana, gdy bufor nie jest w stanie pomieœciæ wiêcej elementów.
	if (v->count == v->allocated_size)
		reallocate(v, 2 * v->allocated_size);
	v->ptr[v->count] = val;
	v->count++;
}


// Pobranie i usuniêcie wartoœci koñcowego elementu wektora *v.
coordinates pop_back_v(vector_t* v) {
	v->count--;
	coordinates retv = v->ptr[v->count];

	//Realokacja realizowana, gdy rozmiar bufora jest ponad dwukrotnie
	//nadmiarowy w stosunku do liczby przechowywanych elementów.
	if (4 * v->count <= v->allocated_size)
		reallocate(v, v->allocated_size / 2);
	return retv;
}


