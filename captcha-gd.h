#ifndef GD_CAPTCHA_H
#define GD_CAPTCHA_H

#include <stdio.h>

// WxH, pixels.
#define CAPTCHA_WIDTH 180
#define CAPTCHA_HEIGHT 60

// Margins to make captcha more readable.
#define CAPTCHA_MARGIN_X (CAPTCHA_WIDTH * 0.05)
#define CAPTCHA_MARGIN_Y (CAPTCHA_HEIGHT * 0.25)

// Will be striked through with X random lines, each X pixels thick.
#define CAPTCHA_STRIKE_WIDTH 2
#define CAPTCHA_STRIKE_LINES 3

// Maximum angle to turn a single char (randomly).
#define CAPTCHA_CHAR_ANGLE (M_PI/8)

void render(const char *captcha_text, FILE *handle, char *fontname);

#endif
