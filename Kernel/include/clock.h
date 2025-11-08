uint8_t cmos_read(uint8_t reg) {
    outb(0x70, reg);
    return inb(0x71);
}
uint8_t bcd_to_bin(uint8_t val) {
    return ((val >> 4) * 10) 
    + (val & 0x0F);
}
void PRINT_TIMER(int x, int y, char c, uint8_t fg, uint8_t bg) {
    if (c >= '0' && c <= '9') {
        DISEGNAMENTO_DEL_FONT(x, y, font_digits[c - '0'], fg, bg);
    } else if (c == ':') {
        DISEGNAMENTO_DEL_FONT(x, y, font_digits[10], fg, bg);
    } else if (c >= 'A' && c <= 'C') {
        DISEGNAMENTO_DEL_FONT(x, y, font_uppercase[c - 'A'], fg, bg);
    } else if (c >= 'a' && c <= 'c') {
        DISEGNAMENTO_DEL_FONT(x, y, font_lowercase[c - 'a'], fg, bg);
    } else {
        static const unsigned char space[7] = {0,0,0,0,0,0,0};
        DISEGNAMENTO_DEL_FONT(x, y, space, fg, bg);
    }
}
void draw_time(int x, int y, int hh, int mm, int ss) {
    PRINT_TIMER(x + 0 * 12, y, '0' + (hh / 10), 15, 0);
    PRINT_TIMER(x + 1 * 12, y, '0' + (hh % 10), 15, 0);
    PRINT_TIMER(x + 2 * 12, y, ':', 15, 0);
    PRINT_TIMER(x + 3 * 12, y, '0' + (mm / 10), 15, 0);
    PRINT_TIMER(x + 4 * 12, y, '0' + (mm % 10), 15, 0);
    PRINT_TIMER(x + 5 * 12, y, ':', 15, 0);
    PRINT_TIMER(x + 6 * 12, y, '0' + (ss / 10), 15, 0);
    PRINT_TIMER(x + 7 * 12, y, '0' + (ss % 10), 15, 0);
}
void get_rtc_datetime(uint8_t *hours, uint8_t *minutes, uint8_t *seconds,
                      uint8_t *day, uint8_t *month, uint16_t *year) {
    *seconds = bcd_to_bin(cmos_read(0x00));
    *minutes = bcd_to_bin(cmos_read(0x02));
    *hours   = bcd_to_bin(cmos_read(0x04));
    *day     = bcd_to_bin(cmos_read(0x07));
    *month   = bcd_to_bin(cmos_read(0x08));
    *year    = 2000 + bcd_to_bin(cmos_read(0x09));
}
void draw_date(int x, int y, int dd, int mm, int yyyy) {
    PRINT_TIMER(x + 0 * 12, y, '0' + (dd / 10), 15, 0);
    PRINT_TIMER(x + 1 * 12, y, '0' + (dd % 10), 15, 0);
    PRINT_TIMER(x + 2 * 12, y, '/', 15, 0);
    PRINT_TIMER(x + 3 * 12, y, '0' + (mm / 10), 15, 0);
    PRINT_TIMER(x + 4 * 12, y, '0' + (mm % 10), 15, 0);
    PRINT_TIMER(x + 5 * 12, y, '/', 15, 0);
    PRINT_TIMER(x + 6 * 12, y, '0' + ((yyyy / 1000) % 10), 15, 0);
    PRINT_TIMER(x + 7 * 12, y, '0' + ((yyyy / 100) % 10), 15, 0);
    PRINT_TIMER(x + 8 * 12, y, '0' + ((yyyy / 10) % 10), 15, 0);
    PRINT_TIMER(x + 9 * 12, y, '0' + (yyyy % 10), 15, 0);
}
