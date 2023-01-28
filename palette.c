#include <stdio.h>
#include <stdlib.h>
#include "palette.h"

void pal_SATURATED(unsigned char *pal)
{
    int n = 0;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++)
            {
                pal[n * 3] = 255 * i;
                pal[n * 3 + 1] = 255 * j;
                pal[n * 3 + 2] = 255 * k;
                n++;
            }
}

void pal_BLACKANDWHITE(unsigned char *pal)
{
    for (int i = 0; i < 2; i++)
    {
        pal[i * 3] = 255 * i;
        pal[i * 3 + 1] = 255 * i;
        pal[i * 3 + 2] = 255 * i;
    }
}

void pal_GRAY(unsigned char *pal)
{
    for (int i = 0; i < 6; i++)
    {
        pal[i * 3] = 51 * i;
        pal[i * 3 + 1] = 51 * i;
        pal[i * 3 + 2] = 51 * i;
    }
}

void pal_444(unsigned char *pal)
{
    int n = 0;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            for (int k = 0; k < 4; k++)
            {
                pal[n * 3] = 85 * i;
                pal[n * 3 + 1] = 85 * j;
                pal[n * 3 + 2] = 85 * k;
                n++;
            }
}
