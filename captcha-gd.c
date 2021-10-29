#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <gd.h>

#include "captcha-gd.h"

int main(int argc, char **argv)
{
    char *font_name = argc > 2 ? argv[2] : NULL;
    const char *captcha_text = argc > 1 ? argv[1] : NULL;
    int good = 1;

    if (captcha_text == NULL) {
        puts("You must specify input text as first argument!");
        good = 0;
    }
    if (font_name == NULL) {
        puts("You must specify the filesystem path to a truetype font as second argument!");
        good = 0;
    }
    if (!good) {
        return 1;
    }

    srand(time(NULL) - 3);

    render(captcha_text, stdout, font_name);

    return 0;
}

/*
 * Generate a random number in range [0..range].
 */
static int range_random(int range)
{
    if (range) {
        return rand() / (RAND_MAX / range);
    }
    else {
        return 0;
    }
}

void render(const char *captcha_text, FILE *handle, char *fontname)
{
    gdImagePtr captcha_image = NULL;
    int background_color, font_color, font_size, char_step, i;
    int brect[8]; // Bounds rect (set by gdImageStringFT).

    char current_char[2] = {0};
    char *font_error = NULL;

    char_step = (CAPTCHA_WIDTH - (CAPTCHA_MARGIN_X * 2)) / strlen(captcha_text);
    font_size = CAPTCHA_HEIGHT - 2 * CAPTCHA_MARGIN_Y;

    captcha_image = gdImageCreate(CAPTCHA_WIDTH, CAPTCHA_HEIGHT);

    // First allocated color (white) is set as a background.
    background_color = gdImageColorAllocate(captcha_image, 255, 255, 255);

    // R, G, B font color (black).
    font_color = gdImageColorAllocate(captcha_image,
                                      rand() % 201,
                                      rand() % 201,
                                      rand() % 201); // generate a random color, used by text
    for (i = 0; *captcha_text; ++captcha_text, ++i) {
        current_char[0] = *captcha_text;
        font_error = gdImageStringFT(captcha_image, brect, font_color, fontname,
                                     font_size, CAPTCHA_CHAR_ANGLE * (rand() / (double) RAND_MAX * 2 - 1),
                                     CAPTCHA_MARGIN_X + (i * char_step), CAPTCHA_HEIGHT - CAPTCHA_MARGIN_Y,
                                     current_char);
        if (font_error) {
            fprintf(stderr, "Failed to render char '%c': %s\n", *captcha_text, font_error);
            if (strcmp(font_error, "Could not find/open font") == 0 || strcmp(font_error, "Could not read font") == 0) {
                exit(2);
            }
        }

    }

    gdImageSetThickness(captcha_image, CAPTCHA_STRIKE_WIDTH);
    for (i = 0; i < CAPTCHA_STRIKE_LINES; ++i) {
        gdImageLine(captcha_image,
                    range_random(CAPTCHA_MARGIN_X),
                    range_random(CAPTCHA_HEIGHT),
                    range_random(CAPTCHA_MARGIN_X) + CAPTCHA_WIDTH - CAPTCHA_MARGIN_X,
                    range_random(CAPTCHA_HEIGHT),
                    i % 2 ? background_color : font_color);
    }
    for (i = 0; i < (CAPTCHA_WIDTH * CAPTCHA_HEIGHT) / 3; i++) {
        int random_x = rand() % CAPTCHA_WIDTH;
        int random_y = rand() % CAPTCHA_HEIGHT;
        gdImageSetPixel(captcha_image,
                        random_x,
                        random_y,
                        gdImageColorAllocate(captcha_image,
                                             rand() % 201,
                                             rand() % 201,
                                             rand() % 201)); // generate another random color
    }
    gdImagePng(captcha_image, handle);
    gdImageDestroy(captcha_image);
}
