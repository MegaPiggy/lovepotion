#pragma once

#include <cstdint>
#include "lcolor.h"

#if defined(__3DS__)
// clang-format off
/// \brief Convert 3DS texture coordinates to pixel index
/// \param width_ Texture width (must be multiple of 8)
/// \param x_ X coordinate
/// \param y_ Y coordinate
inline unsigned coordToIndex(unsigned const width_, unsigned const x_, unsigned const y_)
{
    static unsigned char const table[] =
    {
         0,  1,  4,  5, 16, 17, 20, 21,
         2,  3,  6,  7, 18, 19, 22, 23,
         8,  9, 12, 13, 24, 25, 28, 29,
        10, 11, 14, 15, 26, 27, 30, 31,
        32, 33, 36, 37, 48, 49, 52, 53,
        34, 35, 38, 39, 50, 51, 54, 55,
        40, 41, 44, 45, 56, 57, 60, 61,
        42, 43, 46, 47, 58, 59, 62, 63,
    };

    unsigned const tileX = x_ / 8;
    unsigned const tileY = y_ / 8;
    unsigned const subX  = x_ % 8;
    unsigned const subY  = y_ % 8;

    return ((width_ / 8) * tileY + tileX) * 64 + table[subY * 8 + subX];
}
// clang-format on
#endif
