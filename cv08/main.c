#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct vector
{
    char *elements;
    int size, capacity;
} vector_t;

vector_t newVector();
void push( vector_t *vect, char element);
void increment( vector_t *vect, int position);
void insert( vector_t *vect, char element, int position);
char get( vector_t *vect, int position);

int main()
{
    vector_t grid = newVector();
    vector_t mines = newVector();
    char pole;
    int width = 0, current_width = 0;
    int height = 0;
    printf( "Zadejte hraci plochu:\n" );
    while(scanf("%c", &pole) == 1)
    {
        switch (pole)
        {
            case '.':
                insert( &grid, 0, current_width + width * height );
                current_width++;
                break;
            case '*':
                insert( &grid, 9, current_width + width * height );
                push( &mines, current_width + width * height );
                current_width++;
                break;        
            case '\n':
                if((width == current_width) == (width == 0))
                {
                    printf("Nespravny vstup.\n");
                    return 0;
                }
                width = current_width;
                current_width = 0;
                height++;
                break;        
            default:
                printf("Nespravny vstup.\n");
                return 0;
        }
    }
    if(current_width != 0 || height == 0)
    {
        printf("Nespravny vstup.\n");
        return 0;
    }

    for(int mine = 0; mine < mines.size; mine++)
    {
        int mine_w = mines.elements[mine] % width;
        int mine_h = mines.elements[mine] / width;
//        printf("%d %d\n", mine_w, mine_h);
        for(int dw = -1; dw <= 1; dw++)
        {
            if( (mine_w + dw + width) % width != mine_w + dw ) continue;
            for(int dh = -1; dh <= 1; dh++)
            {
                if( dw == dh && dh == 0) continue;
                if( (mine_h + dh + height) % height != mine_h + dh ) continue;
                increment( &grid, ( mine_w + dw ) + ( mine_h + dh ) * width );
            }
        }
    }

    printf( "Vyplnena hraci plocha:\n" );
    for(int h = 0; h < height; h++)
    {
        for(int w = 0; w < width; w++)
        {
            char tile = get( &grid, w + h * width );
            if(!tile) printf( "." );
            else if(tile > 8) printf( "*" );
            else printf( "%d", tile );
        }
        printf( "\n" );
    }
    return 0;
}

vector_t newVector()
{
    vector_t vect = {(char *)malloc(sizeof(char)), 0, 1};
    return vect;
}

void insert( vector_t *vect, char element, int position)
{
    while( position >= vect->capacity )
    {
        vect->capacity <<= 1;
        vect->elements = (char *)realloc( vect->elements, vect->capacity * sizeof(char) );
    }
    vect->elements[position] = element;
}
void increment( vector_t *vect, int position)
{
    assert( vect->capacity > position);
    vect->elements[position]++;
}

void push( vector_t *vect, char element)
{
    insert( vect, element, vect->size );
    vect->size++;
}

char get( vector_t *vect, int position)
{
    assert( position < vect->capacity );
    return vect->elements[position];
}