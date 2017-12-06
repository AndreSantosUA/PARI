/**
 *       @file  pcolor.c
 *      @brief  File to set printf() colors
 *
 * Not much on detailed description about pcolor
 *
 *     @author  Vitor Santos, vitor@ua.pt
 *
 *   @internal
 *     Created  Oct-2016
 *     Company  University of Aveiro
 *   Copyright  Copyright (c) 2016, Vitor Santos
 *
 * =====================================================================
 */

#include <stdio.h>
#include "pcolor.h"

/**
 * @brief  Set the color of text that follows upon the next printf.
 *
 * @param  attr Type of attribute (RESET, BRIGHT, BLINK, etc...)
 * @param  fg Color of foreground
 * @param  bg Color of background
 * @return Nothing
 */
void TextColor(int attr, int fg, int bg)
{
        printf("%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
}

/**
 * @brief  printf of an horizontal separator red line
 *
 * @return Nothing
 */
void PrintRedLine(void)
{
        int n;
        TextColor(RESET, RED, WHITE);   //completar a função
        for(n=0; n< 80; n++)
                printf("=");
        printf("\n");
}

/**
 * @brief  Reset color text
 *
 * @return Nothing
 */
void ResetTextColors(void)
{
        TextColor(RESET, WHITE, GRAY); //completar a função
}

/**
 * @brief  Make somesort of highlight text
 *
 * @return Nothing
 */
void HighLightText(void)
{
        TextColor(RESET, BLUE, YELLOW); //completar a função
}
