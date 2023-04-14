#ifndef MAIN_UTILS_HPP
#define MAIN_UTILS_HPP

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// WARNING Program uses AVX instructions including AVX2. Check that your CPU supports it.
// WARNING _mm_load in fast version uses ONLY 16 byte aligned data, that's why I use resize versions of images.
// README  Slow version can work with all bmp pictures.

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <SFML/Graphics.hpp>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <immintrin.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "universalUtils.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Number of alpha blending calculation loops.

static const unsigned CalculationLoopCount = 10000;

/// @brief Horizontal offset of foreground.

static const unsigned Offset_x = 300;

/// @brief Vertical offset of foreground.

static const unsigned Offset_y = 200;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Value in shuffle mask that says to shuffle to put zero on current place.

static const char zeroValue = (char) 128;

/// @brief Mask for __mm_shuffle for alpha.

static const __m128i alphaShuffleMask = _mm_set_epi8(zeroValue, 14, zeroValue, 14, 
                                                     zeroValue, 14, zeroValue, 14,
                                                     zeroValue, 6,  zeroValue, 6,
                                                     zeroValue, 6,  zeroValue, 6);

/// @brief Mask for __mm_shuffle for sum.

static const __m128i sumShuffleMask   = _mm_set_epi8(zeroValue, zeroValue, zeroValue, zeroValue, 
                                                     zeroValue, zeroValue, zeroValue, zeroValue,
                                                     15,        13,        11,        9,   
                                                     7,         5,         3,         1);

/// @brief _m128 integer consisting of zero's.

static const __m128i zeroArray = _mm_set_epi8(0, 0, 0, 0, 
                                              0, 0, 0, 0, 
                                              0, 0, 0, 0, 
                                              0, 0, 0, 0);

/// @brief _m128 integer consisting of 255's.

static const __m128i ffArray = _mm_set1_epi16(255);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Contains info about image.

typedef struct image_t
{
    sf::Image *image = NULL;

    const unsigned *pixelsPointer = NULL;

    unsigned width  = 0;
    unsigned height = 0;
} image_t;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief  Constructor for image_t.
/// @param  filename Name of image.
/// @return image_t struct.

image_t *imageConstructor (const char *filename);

/// @brief Destructor for image_t.
/// @param Image Pointer to image_t.

void imageDestructor (image_t *Image);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief  Uses alpha blending to combine foreground and background.
/// @param  Background Pointer to background image_t struct.
/// @param  Foreground Pointer to foreground image_t struct.
/// @return Pointer to pixel array of result.

unsigned *alphaBlending (image_t *Background, 
                         image_t *Foreground);

// README Use -DSLOW in compilation for slow version.

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif
