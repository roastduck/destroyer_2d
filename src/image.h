/**
 * Images defined here are all in-source simple images
 * that will be generated to textures
 * Future developers can use a image loader to replace
 * this
 */

#ifndef IMAGE_H_
#define IMAGE_H_

void initImage();

enum ImageName
{
    IMAGE_RED_CROSS = 0,
    IMAGE_RED_ARROW = 1,
    IMAGE_LAUNCH = 2,

    IMAGE_NUM = 3
};

typedef const unsigned char (*pixel_t)[4];

#define BEGIN {
#define END { 0, 0, 0, 0 } }
#define X { R, G, B, A },
#define _ { r, g, b, a },

/*
 * RED_CROSS
 */
#define R 0xFF
#define G 0x00
#define B 0x00
#define A 0xFF

#define r 0xF0
#define g 0xEA
#define b 0xD6
#define a 0xFF

const int RED_CROSS_W = 16;
const int RED_CROSS_H = 16;
const unsigned char RED_CROSS[RED_CROSS_W * RED_CROSS_H + 1][4] =
BEGIN
X X X X X X X X X X X X X X X X
X _ _ _ _ _ _ _ _ _ _ _ _ _ _ X
X _ _ _ X _ _ _ _ _ _ X _ _ _ X
X _ _ X X X _ _ _ _ X X X _ _ X
X _ X X X X X _ _ X X X X X _ X
X _ _ X X X X X X X X X X _ _ X
X _ _ _ X X X X X X X X _ _ _ X
X _ _ _ _ X X X X X X _ _ _ _ X
X _ _ _ _ X X X X X X _ _ _ _ X
X _ _ _ X X X X X X X X _ _ _ X
X _ _ X X X X X X X X X X _ _ X
X _ X X X X X _ _ X X X X X _ X
X _ _ X X X _ _ _ _ X X X _ _ X
X _ _ _ X _ _ _ _ _ _ X _ _ _ X
X _ _ _ _ _ _ _ _ _ _ _ _ _ _ X
X X X X X X X X X X X X X X X X
END;

const int RED_ARROW_W = 16;
const int RED_ARROW_H = 16;
const unsigned char RED_ARROW[RED_ARROW_W * RED_ARROW_H + 1][4] =
BEGIN
X X X X X X X X X X X X X X X X
X X X X X X X X X X X X X X X X
X X _ _ _ _ _ _ _ _ _ _ _ _ X X
X X _ _ _ _ _ _ X _ _ _ _ _ X X
X X _ _ _ _ _ _ X X _ _ _ _ X X
X X _ _ _ _ _ _ X X X _ _ _ X X
X X _ _ _ _ _ _ X X X X _ _ X X
X X _ X X X X X X X X X X _ X X
X X _ X X X X X X X X X X _ X X
X X _ _ _ _ _ _ X X X X _ _ X X
X X _ _ _ _ _ _ X X X _ _ _ X X
X X _ _ _ _ _ _ X X _ _ _ _ X X
X X _ _ _ _ _ _ X _ _ _ _ _ X X
X X _ _ _ _ _ _ _ _ _ _ _ _ X X
X X X X X X X X X X X X X X X X
X X X X X X X X X X X X X X X X
END;

#undef R
#undef G
#undef B
#undef A

#undef r
#undef g
#undef b
#undef a

/*
 * LAUNCH
 */
#define R 0xFF
#define G 0x00
#define B 0x00
#define A 0xFF

#define r 0xFF
#define g 0xFF
#define b 0x66
#define a 0xFF

const int LAUNCH_W = 64;
const int LAUNCH_H = 16;
const unsigned char LAUNCH[LAUNCH_W * LAUNCH_H + 1][4] =
BEGIN
X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
X _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ X
X _ _ X X _ _ _ _ _ _ _ _ _ X X X _ _ _ _ _ X X _ _ _ _ X X _ _ X X _ _ _ _ _ X X _ _ _ _ X X X X X _ _ _ X X _ _ _ _ X X _ _ X
X _ _ X X _ _ _ _ _ _ _ _ X X X X X _ _ _ _ X X _ _ _ _ X X _ _ X X _ _ _ _ _ X X _ _ _ X X X X X X X _ _ X X _ _ _ _ X X _ _ X
X _ _ X X _ _ _ _ _ _ _ X X X _ X X X _ _ _ X X _ _ _ _ X X _ _ X X X _ _ _ _ X X _ _ X X _ _ _ _ X X _ _ X X _ _ _ _ X X _ _ X
X _ _ X X _ _ _ _ _ _ X X X _ _ _ X X X _ _ X X _ _ _ _ X X _ _ X X X X _ _ _ X X _ _ X X _ _ _ _ _ _ _ _ X X _ _ _ _ X X _ _ X
X _ _ X X _ _ _ _ _ _ X X _ _ _ _ _ X X _ _ X X _ _ _ _ X X _ _ X X X X X _ _ X X _ _ X X _ _ _ _ _ _ _ _ X X _ _ _ _ X X _ _ X
X _ _ X X _ _ _ _ _ _ X X _ _ _ _ _ X X _ _ X X _ _ _ _ X X _ _ X X _ X X _ _ X X _ _ X X _ _ _ _ _ _ _ _ X X X X X X X X _ _ X
X _ _ X X _ _ _ _ _ _ X X _ _ _ _ _ X X _ _ X X _ _ _ _ X X _ _ X X _ _ X X _ X X _ _ X X _ _ _ _ _ _ _ _ X X X X X X X X _ _ X
X _ _ X X _ _ _ _ _ _ X X X X X X X X X _ _ X X _ _ _ _ X X _ _ X X _ _ X X X X X _ _ X X _ _ _ _ _ _ _ _ X X _ _ _ _ X X _ _ X
X _ _ X X _ _ _ _ _ _ X X X X X X X X X _ _ X X _ _ _ _ X X _ _ X X _ _ _ X X X X _ _ X X _ _ _ _ _ _ _ _ X X _ _ _ _ X X _ _ X
X _ _ X X _ _ _ _ _ _ X X _ _ _ _ _ X X _ _ X X _ _ _ _ X X _ _ X X _ _ _ _ X X X _ _ X X _ _ _ _ X X _ _ X X _ _ _ _ X X _ _ X
X _ _ X X X X X X X _ X X _ _ _ _ _ X X _ _ X X X X X X X X _ _ X X _ _ _ _ _ X X _ _ _ X X X X X X X _ _ X X _ _ _ _ X X _ _ X
X _ _ X X X X X X X _ X X _ _ _ _ _ X X _ _ _ X X X X X X _ _ _ X X _ _ _ _ _ X X _ _ _ _ X X X X X _ _ _ X X _ _ _ _ X X _ _ X
X _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ X
X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X X
END;

#undef R
#undef G
#undef B
#undef A

#undef r
#undef g
#undef b
#undef a

/*
 * font of 'A'..'Z' ',' '.' ' '
 */
const int FONT_H = 16;
const int FONT_W = 13;

#define R 0x00
#define G 0x00
#define B 0x00
#define A 0xFF

#define r 0xFA
#define g 0xEB
#define b 0xD7
#define a 0xFF

const unsigned char FONT_BG_COLOR[4] = { r, g, b, a };
const unsigned char FONT[29][FONT_W * FONT_H + 1][4] = {
BEGIN
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ X X X _ _ _ _ _
_ _ _ _ X X X X X _ _ _ _
_ _ _ X X X _ X X X _ _ _
_ _ X X X _ _ _ X X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X X X X X X X X _ _
_ _ X X X X X X X X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
END,
BEGIN
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ X X X X X X _ _ _ _ _
_ _ X X X X X X X _ _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X X X X X X _ _ _ _
_ _ X X X X X X X _ _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X X X X X X X _ _ _
_ _ X X X X X X X _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
END,
BEGIN
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ X X X X X _ _ _ _
_ _ _ X X X X X X X _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ _ X X X X X X X _ _ _
_ _ _ _ X X X X X _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
END,
BEGIN
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ X X X X X X X _ _ _ _
_ _ X X X X X X X X _ _ _
_ _ X X _ _ _ _ _ X _ _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X _ _ _
_ _ X X X X X X X X _ _ _
_ _ X X X X X X X _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
END,
BEGIN
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ X X X X X X X X X _ _
_ _ X X X X X X X X X _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X X X X X X X X _ _
_ _ X X X X X X X X X _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X X X X X X X X _ _
_ _ X X X X X X X X X _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
END,
BEGIN
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ X X X X X X X X X _ _
_ _ X X X X X X X X X _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X X X X X X X X _ _
_ _ X X X X X X X X X _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
END,
BEGIN
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ X X X X X _ _ _ _
_ _ _ X X X X X X X _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X _ _ X X X X _ _ _
_ _ X X _ _ X X X X _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ _ X X X X X X X _ _ _
_ _ _ _ X X X X X X _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
END,
BEGIN
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X X X X X X X _ _ _
_ _ X X X X X X X X _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
END,
BEGIN
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ X X X X _ _ _ _ _
_ _ _ _ X X X X _ _ _ _ _
_ _ _ _ _ X X _ _ _ _ _ _
_ _ _ _ _ X X _ _ _ _ _ _
_ _ _ _ _ X X _ _ _ _ _ _
_ _ _ _ _ X X _ _ _ _ _ _
_ _ _ _ _ X X _ _ _ _ _ _
_ _ _ _ _ X X _ _ _ _ _ _
_ _ _ _ _ X X _ _ _ _ _ _
_ _ _ _ _ X X _ _ _ _ _ _
_ _ _ _ X X X X _ _ _ _ _
_ _ _ _ X X X X _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
END,
BEGIN
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ X X X X X X _ _ _ _
_ _ _ X X X X X X _ _ _ _
_ _ _ _ _ _ _ X X _ _ _ _
_ _ _ _ _ _ _ X X _ _ _ _
_ _ _ _ _ _ _ X X _ _ _ _
_ _ _ _ _ _ _ X X _ _ _ _
_ _ _ _ _ _ _ X X _ _ _ _
_ _ _ _ _ _ _ X X _ _ _ _
_ _ X X _ _ _ X X _ _ _ _
_ _ X X _ _ _ X X _ _ _ _
_ _ X X X X X X _ _ _ _ _
_ _ _ X X X X X _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
END,
BEGIN
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ X _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X _ _ _ X X _ _ _ _
_ _ X X _ _ X X _ _ _ _ _
_ _ X X _ X X _ _ _ _ _ _
_ _ X X X X _ _ _ _ _ _ _
_ _ X X X X X _ _ _ _ _ _
_ _ X X _ _ X X _ _ _ _ _
_ _ X X _ _ _ X X _ _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
END,
BEGIN
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X X X X X X X X _ _
_ _ X X X X X X X X X _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
END,
BEGIN
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X X _ _ _ X X X _ _
_ _ X X X _ _ _ X X X _ _
_ _ X X _ X _ X _ X X _ _
_ _ X X _ X X X _ X X _ _
_ _ X X _ _ X _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
END,
BEGIN
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X X _ _ _ _ X X _ _
_ _ X X X X _ _ _ X X _ _
_ _ X X X X X _ _ X X _ _
_ _ X X _ X X _ _ X X _ _
_ _ X X _ _ X X _ X X _ _
_ _ X X _ _ X X X X X _ _
_ _ X X _ _ _ X X X X _ _
_ _ X X _ _ _ _ X X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
END,
BEGIN
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ X X X X X _ _ _ _
_ _ _ X X X X X X X _ _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ _ X X X X X X X _ _ _
_ _ _ _ X X X X X _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
END,
BEGIN
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ X X X X X X X _ _ _ _
_ _ X X X X X X X X _ _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X X X X X X X _ _ _
_ _ X X X X X X X _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
END,
BEGIN
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ X X X X X _ _ _ _
_ _ _ X X X X X X X _ _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ X X _ X X _ _
_ _ X X _ _ X X X X X _ _
_ _ X X _ _ _ X X X _ _ _
_ _ _ X X X X X X X X _ _
_ _ _ _ X X X X _ X X _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
END,
BEGIN
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ X X X X X X X _ _ _ _
_ _ X X X X X X X X _ _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X X X X X X X _ _ _
_ _ X X X X X X X _ _ _ _
_ _ X X _ X X _ _ _ _ _ _
_ _ X X _ _ X X _ _ _ _ _
_ _ X X _ _ _ X X _ _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
END,
BEGIN
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ X X X X X X X _ _ _
_ _ X X X X X X X X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ _ X X X X X X _ _ _ _
_ _ _ _ X X X X X X _ _ _
_ _ _ _ _ _ _ _ _ X X _ _
_ _ _ _ _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X X X X X X X _ _ _
_ _ _ X X X X X X _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
END,
BEGIN
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ X X X X X X X X X _ _
_ _ X X X X X X X X X _ _
_ _ _ _ _ X X _ _ _ _ _ _
_ _ _ _ _ X X _ _ _ _ _ _
_ _ _ _ _ X X _ _ _ _ _ _
_ _ _ _ _ X X _ _ _ _ _ _
_ _ _ _ _ X X _ _ _ _ _ _
_ _ _ _ _ X X _ _ _ _ _ _
_ _ _ _ _ X X _ _ _ _ _ _
_ _ _ _ _ X X _ _ _ _ _ _
_ _ _ _ _ X X _ _ _ _ _ _
_ _ _ _ _ X X _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
END,
BEGIN
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X X X X X X X _ _ _
_ _ _ X X X X X X _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
END,
BEGIN
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ X X _ _ _ _ X X _ _ _
_ _ _ X X _ _ X X _ _ _ _
_ _ _ _ X X X X _ _ _ _ _
_ _ _ _ _ X X _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
END,
BEGIN
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ X X _ _ _ _ _ _ X X _ _
_ X X _ _ _ _ _ _ X X _ _
_ X X _ _ _ _ _ _ X X _ _
_ X X _ _ _ _ _ _ X X _ _
_ X X _ _ _ _ _ _ X X _ _
_ X X _ _ X X _ _ X X _ _
_ X X _ _ X X _ _ X X _ _
_ X X _ _ X X _ _ X X _ _
_ X X _ _ X X _ _ X X _ _
_ X X _ X X X X _ X X _ _
_ _ X X X X X X X X _ _ _
_ _ _ X X _ _ X X _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
END,
BEGIN
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ _ X X _ _ _ X X _ _ _
_ _ _ _ X X _ X X _ _ _ _
_ _ _ _ _ X X X _ _ _ _ _
_ _ _ _ _ X X X _ _ _ _ _
_ _ _ _ X X _ X X _ _ _ _
_ _ _ X X _ _ _ X X _ _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ X X _ _ _ _ _ X X _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
END,
BEGIN
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ X X _
_ _ X X _ _ _ _ _ _ X X _
_ _ _ X X _ _ _ _ X X _ _
_ _ _ X X _ _ _ _ X X _ _
_ _ _ _ X X _ _ X X _ _ _
_ _ _ _ X X X X X X _ _ _
_ _ _ _ _ X X X X _ _ _ _
_ _ _ _ _ _ X X _ _ _ _ _
_ _ _ _ _ _ X X _ _ _ _ _
_ _ _ _ _ _ X X _ _ _ _ _
_ _ _ _ _ _ X X _ _ _ _ _
_ _ _ _ _ _ X X _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
END,
BEGIN
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ X X X X X X X X X _ _
_ _ X X X X X X X X X _ _
_ _ _ _ _ _ _ _ _ _ X _ _
_ _ _ _ _ _ _ _ _ X X _ _
_ _ _ _ _ _ _ _ X X _ _ _
_ _ _ _ _ _ _ X X _ _ _ _
_ _ _ _ _ _ X X _ _ _ _ _
_ _ _ _ _ X X _ _ _ _ _ _
_ _ _ _ X X _ _ _ _ _ _ _
_ _ _ X X _ _ _ _ _ _ _ _
_ _ X X X X X X X X X _ _
_ _ X X X X X X X X X _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
END,
BEGIN
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ X X X _ _ _ _ _ _ _ _
_ _ X X X _ _ _ _ _ _ _ _
_ _ X X X _ _ _ _ _ _ _ _
_ _ _ _ X _ _ _ _ _ _ _ _
_ _ _ _ X _ _ _ _ _ _ _ _
_ _ X X _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
END,
BEGIN
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ X X X _ _ _ _ _ _ _ _
_ _ X X X _ _ _ _ _ _ _ _
_ _ X X X _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
END,
BEGIN
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
_ _ _ _ _ _ _ _ _ _ _ _ _
END
};

#undef R
#undef G
#undef B
#undef A

#undef r
#undef g
#undef b
#undef a

#undef X
#undef _

const pixel_t IMAGES[IMAGE_NUM] = { RED_CROSS, RED_ARROW, LAUNCH };
const int IMAGES_W[IMAGE_NUM] = { RED_CROSS_W, RED_ARROW_W, LAUNCH_W };
const int IMAGES_H[IMAGE_NUM] = { RED_CROSS_H, RED_ARROW_H, LAUNCH_H };

#endif // IMAGE_H_
