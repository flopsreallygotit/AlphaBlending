#include "mainUtils.hpp"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

image_t *imageConstructor (const char *filename)
{
    image_t *Image = (image_t *) calloc (1, sizeof(*Image));

    Image->image = new sf::Image;

    CHECKERROR(Image->image->loadFromFile(filename) &&
               "Can't load image from file.", 
               NULL);

    Image->pixelsPointer = (const unsigned *) Image->image->getPixelsPtr();

    Image->width  = Image->image->getSize().x;
    Image->height = Image->image->getSize().y;

    return Image;
}

void imageDestructor (image_t *Image)
{
    CHECKWARNING(Image != NULL &&
                 "Can't destroy nullptr.");

    Image->width  = 0;
    Image->height = 0;

    delete Image->image;
    Image->image = NULL;

    Image->pixelsPointer = NULL;

    free(Image);
    Image = NULL;
    
    return;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#ifdef SLOW

unsigned *alphaBlending (image_t *Background, 
                         image_t *Foreground)
{
    unsigned *result = (unsigned *) calloc (Background->width * 
                                            Background->height, 
                                            sizeof(*result));

    memcpy(result, 
           Background->pixelsPointer, 
           4 * Background->width * Background->height);

    for (unsigned loop = 0; loop < CalculationLoopCount; loop++)
    {
        for (unsigned y = 0; y < Foreground->height; y++)
        {
            for (unsigned x = 0; x < Foreground->width; x++)
            {
                unsigned foregroundColor = Foreground->pixelsPointer[x + y * Foreground->width];
                unsigned backgroundColor = Background->pixelsPointer[x + Offset_x + 
                                                                    (y + Offset_y) * Background->width];
                unsigned foregroundAlpha = foregroundColor >> 24;
                unsigned newColor = backgroundColor & 0xFF000000;

                for (int idx = 0; idx < 24; idx += 8)
                {
                    unsigned foregroundChannel = ((0xFF << idx) & foregroundColor) >> idx;
                    unsigned backgroundChannel = ((0xFF << idx) & backgroundColor) >> idx;

                    newColor += ((foregroundChannel * foregroundAlpha + 
                                  backgroundChannel * (255 - foregroundAlpha)) / 255) << idx;
                }

                result[x + Offset_x + 
                      (y + Offset_y) * Background->width] = newColor;
            }
        }
    }

    return result;
}

#else

unsigned *alphaBlending (image_t *Background, 
                         image_t *Foreground)
{
    unsigned *result = (unsigned *) calloc (Background->width * 
                                            Background->height, 
                                            sizeof(*result));

    memcpy(result, Background->pixelsPointer, 
           4 * Background->width * Background->height);

    for (unsigned loop = 0; loop < CalculationLoopCount; loop++)
    {
        for (unsigned y = 0; y < Foreground->height; y++)
        {
            for (unsigned x = 0; x < Foreground->width; x += 4)
            {   
                __m128i foregroundColor = _mm_load_si128((const __m128i *) 
                                          &Foreground->pixelsPointer[x + y * Foreground->width]);
                __m128i backgroundColor = _mm_load_si128((const __m128i *) 
                                          &Background->pixelsPointer[x + Offset_x + 
                                                                    (y + Offset_y) * Background->width]);

                __m128i upperForegroundColor = (__m128i) _mm_movehl_ps((__m128) zeroArray, 
                                                                       (__m128) foregroundColor);
                __m128i upperBackgroundColor = (__m128i) _mm_movehl_ps((__m128) zeroArray, 
                                                                       (__m128) backgroundColor);

                foregroundColor      = _mm_cvtepu8_epi16(foregroundColor);
                upperForegroundColor = _mm_cvtepu8_epi16(upperForegroundColor);

                backgroundColor      = _mm_cvtepu8_epi16(backgroundColor);
                upperBackgroundColor = _mm_cvtepu8_epi16(upperBackgroundColor);

                __m128i lowerForegroundAlpha = _mm_shuffle_epi8(foregroundColor,      
                                                                alphaShuffleMask);                           
                __m128i upperForegroundAlpha = _mm_shuffle_epi8(upperForegroundColor, 
                                                                alphaShuffleMask);

                foregroundColor      = _mm_mullo_epi16(foregroundColor, 
                                                       lowerForegroundAlpha);  
                upperForegroundColor = _mm_mullo_epi16(upperForegroundColor, 
                                                       upperForegroundAlpha);

                backgroundColor      = _mm_mullo_epi16(backgroundColor, 
                                                       _mm_sub_epi16 (ffArray, lowerForegroundAlpha));                     
                upperBackgroundColor = _mm_mullo_epi16(upperBackgroundColor, 
                                                       _mm_sub_epi16 (ffArray, upperForegroundAlpha));

                __m128i lowerSum = _mm_add_epi16(foregroundColor, 
                                                 backgroundColor);                                  
                __m128i upperSum = _mm_add_epi16(upperForegroundColor,
                                                 upperBackgroundColor);

                lowerSum = _mm_shuffle_epi8(lowerSum, sumShuffleMask);                                
                upperSum = _mm_shuffle_epi8(upperSum, sumShuffleMask);

                __m128i newColor = _mm_set1_epi8((char) 255);
                newColor = (__m128i) _mm_movelh_ps((__m128) lowerSum, 
                                                   (__m128) upperSum);

                 _mm_store_si128((__m128i *) &result[x + Offset_x + (y + Offset_y) * Background->width], newColor);
            }
        }
    }

    return result;
}

#endif

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
