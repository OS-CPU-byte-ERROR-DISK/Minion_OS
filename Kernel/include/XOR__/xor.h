void Disegna_lo_xor(int x_offset, int y_offset) {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            uint8_t color = (x + x_offset) ^ (y + y_offset);
            SET_DRAW_PIXEL(x, y, color);
        }
    }
}
int TEST() {
    int x_off = 222;
    int y_off = 49;

    while (1) {
        Disegna_lo_xor(x_off, y_off);
        x_off++;
        y_off++;  
    }
}