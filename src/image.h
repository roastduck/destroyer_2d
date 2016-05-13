#ifndef IMAGE_H_
#define IMAGE_H_

enum ImageName
{
    IMAGE_RED_CROSS = 0,

    IMAGE_NUM = 1
};

typedef const unsigned char (*pixel_t)[4];

#define BEGIN {
#define END { 0, 0, 0, 0 } };
#define X { R, G, B, A },
#define _ { 0x00, 0x00, 0x00, 0x00 },

/*
 * RED_CROSS
 */
#define R 0xFF
#define G 0x00
#define B 0x00
#define A 0xFF

static const int RED_CROSS_W = 16;
static const int RED_CROSS_H = 16;
static const unsigned char RED_CROSS[RED_CROSS_W * RED_CROSS_H + 1][4] =
BEGIN
_ _ X _ _ _ _ _ _ _ _ _ _ X _ _
_ X X X _ _ _ _ _ _ _ _ X X X _
X X X X X _ _ _ _ _ _ X X X X X
_ X X X X X _ _ _ _ X X X X X _
_ _ X X X X X _ _ X X X X X _ _
_ _ _ X X X X X X X X X X _ _ _
_ _ _ _ X X X X X X X X _ _ _ _
_ _ _ _ _ X X X X X X _ _ _ _ _
_ _ _ _ _ X X X X X X _ _ _ _ _
_ _ _ _ X X X X X X X X _ _ _ _
_ _ _ X X X X X X X X X X _ _ _
_ _ X X X X X _ _ X X X X X _ _
_ X X X X X _ _ _ _ X X X X X _
X X X X X _ _ _ _ _ _ X X X X X
_ X X X _ _ _ _ _ _ _ _ X X X _
_ _ X _ _ _ _ _ _ _ _ _ _ X _ _
END

#undef R
#undef G
#undef B
#undef A

#undef X
#undef _

const pixel_t IMAGES[IMAGE_NUM] = { RED_CROSS };
const int IMAGES_W[IMAGE_NUM] = { RED_CROSS_W };
const int IMAGES_H[IMAGE_NUM] = { RED_CROSS_H };

#endif // IMAGE_H_
