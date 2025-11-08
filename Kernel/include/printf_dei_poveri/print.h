void DISEGNAMENTO_DEL_FONT(int x, int y, unsigned char *font_char, uint8_t color_fg, uint8_t color_bg) {
    for (int row = 0; row < 7; row++) {
        unsigned char bits = font_char[row];
        for (int col = 0; col < 5; col++) {
            uint8_t pixel_color = (bits & (1 << (4 - col))) ? color_fg : color_bg;
            SET_DRAW_PIXEL(x + col * 2, y + row * 2, pixel_color);
            SET_DRAW_PIXEL(x + col * 2 + 1, y + row * 2, pixel_color);
            SET_DRAW_PIXEL(x + col * 2, y + row * 2 + 1, pixel_color);
            SET_DRAW_PIXEL(x + col * 2 + 1, y + row * 2 + 1, pixel_color);
        }
    }
}
void draw_text(int x, int y, const char *text, uint8_t color_fg, uint8_t color_bg) {
    while (*text) {
        char c = *text++;

        if (c >= 'a' && c <= 'z') {
            DISEGNAMENTO_DEL_FONT(x, y, font_lowercase[c - 'a'], color_fg, color_bg);
            x += 12;
        }
        else if (c >= 'A' && c <= 'Z') {
            DISEGNAMENTO_DEL_FONT(x, y, font_uppercase[c - 'A'], color_fg, color_bg);
            x += 12;
        }
        else if (c >= '0' && c <= '9') {
            DISEGNAMENTO_DEL_FONT(x, y, font_digits[c - '0'], color_fg, color_bg);
            x += 12;
        }
        else if (c == ' ') {
            x += 12;
        }
        else {
            int found = -1;
            for (int i = 0; i < sizeof(special_chars); i++) {
                if (c == special_chars[i]) {
                    found = i;
                    break;
                }
            }

            if (found != -1) {
                DISEGNAMENTO_DEL_FONT(x, y, font_symbols[found], color_fg, color_bg);
                x += 12;
            } else {
                x += 12;
            }
        }
    }
}