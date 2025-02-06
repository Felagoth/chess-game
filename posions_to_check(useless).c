#include "types.h"

typedef struct coord_array
{
    coords coords[64];
    int size;
} coord_array;

coord_array positions_to_check(coords init_co, piece selected_piece)
{
    coord_array coords;
    coords.size = 0;
    if (selected_piece.name == 'P')
    {
        if (selected_piece.color == 'w')
        {
            coords.coords[0].x = init_co.x + 1;
            coords.coords[0].y = init_co.y;
            coords.size++;
            coords.coords[0].x = init_co.x + 2;
            coords.coords[0].y = init_co.y;
            coords.size++;
            coords.coords[0].x = init_co.x + 1;
            coords.coords[0].y = init_co.y + 1;
            coords.size++;
            coords.coords[1].x = init_co.x + 1;
            coords.coords[1].y = init_co.y - 1;
            coords.size++;
        }
        else
        {
            coords.coords[0].x = init_co.x - 1;
            coords.coords[0].y = init_co.y;
            coords.size++;
            coords.coords[0].x = init_co.x - 2;
            coords.coords[0].y = init_co.y;
            coords.size++;
            coords.coords[0].x = init_co.x - 1;
            coords.coords[0].y = init_co.y + 1;
            coords.size++;
            coords.coords[1].x = init_co.x - 1;
            coords.coords[1].y = init_co.y - 1;
            coords.size++;
        }
    }
    else if (selected_piece.name == 'N')
    {
        coords.coords[0].x = init_co.x + 2;
        coords.coords[0].y = init_co.y + 1;
        coords.size++;
        coords.coords[1].x = init_co.x + 2;
        coords.coords[1].y = init_co.y - 1;
        coords.size++;
        coords.coords[2].x = init_co.x - 2;
        coords.coords[2].y = init_co.y + 1;
        coords.size++;
        coords.coords[3].x = init_co.x - 2;
        coords.coords[3].y = init_co.y - 1;
        coords.size++;
        coords.coords[4].x = init_co.x + 1;
        coords.coords[4].y = init_co.y + 2;
        coords.size++;
        coords.coords[5].x = init_co.x + 1;
        coords.coords[5].y = init_co.y - 2;
        coords.size++;
        coords.coords[6].x = init_co.x - 1;
        coords.coords[6].y = init_co.y + 2;
        coords.size++;
        coords.coords[7].x = init_co.x - 1;
        coords.coords[7].y = init_co.y - 2;
        coords.size++;
    }
    else if (selected_piece.name == 'B' || 'Q')
    {
        for (int i = 1; i < 8; i++)
        {
            coords.coords[coords.size].x = init_co.x + i;
            coords.coords[coords.size].y = init_co.y + i;
            coords.size++;
            coords.coords[coords.size].x = init_co.x - i;
            coords.coords[coords.size].y = init_co.y - i;
            coords.size++;
            coords.coords[coords.size].x = init_co.x + i;
            coords.coords[coords.size].y = init_co.y - i;
            coords.size++;
            coords.coords[coords.size].x = init_co.x - i;
            coords.coords[coords.size].y = init_co.y + i;
            coords.size++;
        }
    }
    if (selected_piece.name == 'R' || 'Q')
    {
        for (int i = 1; i < 8; i++)
        {
            coords.coords[coords.size].x = init_co.x + i;
            coords.coords[coords.size].y = init_co.y;
            coords.size++;
            coords.coords[coords.size].x = init_co.x - i;
            coords.coords[coords.size].y = init_co.y;
            coords.size++;
            coords.coords[coords.size].x = init_co.x;
            coords.coords[coords.size].y = init_co.y + i;
            coords.size++;
            coords.coords[coords.size].x = init_co.x;
            coords.coords[coords.size].y = init_co.y - i;
            coords.size++;
        }
    }
    if (selected_piece.name == 'K')
    {
        coords.coords[0].x = init_co.x + 1;
        coords.coords[0].y = init_co.y;
        coords.size++;
        coords.coords[1].x = init_co.x - 1;
        coords.coords[1].y = init_co.y;
        coords.size++;
        coords.coords[2].x = init_co.x;
        coords.coords[2].y = init_co.y + 1;
        coords.size++;
        coords.coords[3].x = init_co.x;
        coords.coords[3].y = init_co.y - 1;
        coords.size++;
        coords.coords[4].x = init_co.x + 1;
        coords.coords[4].y = init_co.y + 1;
        coords.size++;
        coords.coords[5].x = init_co.x + 1;
        coords.coords[5].y = init_co.y - 1;
        coords.size++;
        coords.coords[6].x = init_co.x - 1;
        coords.coords[6].y = init_co.y + 1;
        coords.size++;
        coords.coords[7].x = init_co.x - 1;
        coords.coords[7].y = init_co.y - 1;
        coords.size++;
    }
    return coords;
}