#include "main.h"
typedef struct {
    int x, y;
}coordinates;

typedef struct {
    int allocated_size;
    int count;          
    coordinates* ptr;           
} vector_t;

void init_vector(vector_t* v);
void reallocate(vector_t * v, int reallocate_size);
void push_back(vector_t * v, coordinates val);
//coordinates pop_back_v(vector_t* v);

