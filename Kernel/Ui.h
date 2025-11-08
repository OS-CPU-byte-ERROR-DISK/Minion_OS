#include "include/drivers/ports.h"
#include "include/font.h"
#include "include/drivers/mouse.h"
#include "include/image_data.h"
#include "include/clock.h"
#include "include/shutdown/shutdown.h"
#define GREEN__ 0x0A
#define CHAR_HEIGHT 16
#define STATUSBAR_HEIGHT 20
#define STATUSBAR_COLOR COLOR_DOCK
#define CHAR_WIDTH  5
#define SCALE       2
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define COLOR_GREEN 0x0A
#define COLOR_RED 0x04
#define COLOR_DOCK       0
#define COLOR_ACCENT     4
#define COLOR_BTN_BG     7
#define COLOR_BTN_BORDER 0
#define DOCK_WIDTH 60
#define ICON_WIDTH  80
#define ICON_HEIGHT 70
#define POWER_BTN_W 40
#define POWER_BTN_H 25
#define POWER_BTN_X (SCREEN_WIDTH - POWER_BTN_W - 25)
#define POWER_BTN_Y 0
#define WHITE_COLOR 15
int icon_x, icon_y, icon_w, icon_h;
int reboot_icon_x, reboot_icon_y;
int shutdown_window_x = (SCREEN_WIDTH - 250) / 2;
int shutdown_window_y = (SCREEN_HEIGHT - 100) / 2;
int shutdown_window_visible = 0;
int dragging_shutdown_window = 0;
int shutdown_drag_offset_x = 0;
int shutdown_drag_offset_y = 0;
int img_w = 320;   // roba della foto
int img_h = 200;   // altezza della foto   
int confirm_shutdown_visible = 0;
int window_x = 100;
int window_y = 100;
int window_w = 200;
int window_h = 80;
int window_radius = 8;
int window_visible = 0;
int dragging_window = 0;
int drag_offset_x = 0;
int drag_offset_y = 0;
int shutdown_win_x = (SCREEN_WIDTH - 250) / 2;
int shutdown_win_y = (SCREEN_HEIGHT - 100) / 2;
int shutdown_win_w = 250;
int shutdown_win_h = 100;
int icon_x = (SCREEN_WIDTH - ICON_WIDTH) / 2;
int icon_y = (SCREEN_HEIGHT - ICON_HEIGHT) / 2;

size_t strlen(const char *str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

void SET_DRAW_PIXEL(int x, int y, uint8_t color) {
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT)
        VIDEO_MEMORY_PTR[y * SCREEN_WIDTH + x] = color;
}
void ROBO_STRANO(int cx, int cy, int r, int quadrant, uint8_t color) {
    for (int dy = 0; dy <= r; dy++) {
        for (int dx = 0; dx <= r; dx++) {
            if (dx*dx + dy*dy <= r*r) {
                int px = cx, py = cy;
                switch(quadrant) {
                    case 1: px -= dx; py -= dy; break;
                    case 2: px += dx; py -= dy; break;
                    case 3: px -= dx; py += dy; break;
                    case 4: px += dx; py += dy; break;
                }
                SET_DRAW_PIXEL(px, py, color);
            }
        }
    }
}
void rounded_rect(int x, int y, int w, int h, int radius, uint8_t color) {
    draw_rect(x + radius, y, w - 2*radius, h, color);
    draw_rect(x, y + radius, radius, h - 2*radius, color);
    draw_rect(x + w - radius, y + radius, radius, h - 2*radius, color);
    ROBO_STRANO(x + radius, y + radius, radius, 1, color);
    ROBO_STRANO(x + w - radius - 1, y + radius, radius, 2, color);
    ROBO_STRANO(x + radius, y + h - radius - 1, radius, 3, color);
    ROBO_STRANO(x + w - radius - 1, y + h - radius - 1, radius, 4, color);
}
void disegnamento_icone_strane(int x, int y, int w, int h, const char *label, uint8_t color_bg, uint8_t color_border) {
    draw_rect(x, y, w, h, color_bg);
    draw_rect_border(x, y, w, h, color_border);
    int text_x = x + (w - 4 * 12) / 2;
    int text_y = y + (h - 14) / 2;
    draw_text(text_x, text_y, label, WHITE_COLOR, color_bg);
}

void draw_rect(int x, int y, int w, int h, uint8_t color) {
    for (int py = y; py < y + h; py++) {
        for (int px = x; px < x + w; px++) {
            if (px >= 0 && px < SCREEN_WIDTH && py >= 0 && py < SCREEN_HEIGHT)
                VIDEO_MEMORY_PTR[py * SCREEN_WIDTH + px] = color;
  }
    }
}
void draw_rect_border(int x, int y, int w, int h, uint8_t color) {
    for (int px = x; px < x + w; px++) {
        if (y >= 0 && y < SCREEN_HEIGHT)
            VIDEO_MEMORY_PTR[y * SCREEN_WIDTH + px] = color;
        if ((y + h - 1) >= 0 && (y + h - 1) < SCREEN_HEIGHT)
            VIDEO_MEMORY_PTR[(y + h - 1) * SCREEN_WIDTH + px] = color;
    }
    for (int py = y; py < y + h; py++) {
        if (x >= 0 && x < SCREEN_WIDTH)
            VIDEO_MEMORY_PTR[py * SCREEN_WIDTH + x] = color;
        if ((x + w - 1) >= 0 && (x + w - 1) < SCREEN_WIDTH)
            VIDEO_MEMORY_PTR[py * SCREEN_WIDTH + (x + w - 1)] = color;
    }
}
void BARRA_UBUNTU_BUGGATA() {
    draw_rect(0, 0, DOCK_WIDTH, SCREEN_HEIGHT, COLOR_DOCK);
}
void icone_della_BARRA_UBUNTU_BUGGATA(int y_offset, uint8_t color) {
    draw_rect(10, y_offset, 40, 40, color);
    draw_rect_border(10, y_offset, 40, 40, COLOR_BTN_BORDER);
}
void SFONDO_(const uint8_t *img_data, int img_w, int img_h, int screen_w, int screen_h) {
    for (int y = 0; y < screen_h; y++) {
        for (int x = 0; x < screen_w; x++) {
            int src_x = x * img_w / screen_w;
            int src_y = y * img_h / screen_h;
            VIDEO_MEMORY_PTR[y * screen_w + x] = img_data[src_y * img_w + src_x];
        }
    }
}
void SHUTDOW_MESSAGEBOX() {
    rounded_rect(shutdown_win_x, shutdown_win_y, shutdown_win_w, shutdown_win_h, 8, COLOR_BTN_BG);
    draw_rect_border(shutdown_win_x, shutdown_win_y, shutdown_win_w, shutdown_win_h, COLOR_BTN_BORDER);
    draw_rect(shutdown_win_x, shutdown_win_y, shutdown_win_w, 20, 0);
    const char *msg = "shutdown?";
    int text_x = shutdown_win_x + (shutdown_win_w - 18 * strlen(msg)) / 2;
    int text_y = shutdown_win_y + 20;
    draw_text(text_x, text_y, msg, WHITE_COLOR, 0);
    draw_rect(shutdown_win_x + 40, shutdown_win_y + 60, 60, 20, COLOR_RED);
    draw_rect_border(shutdown_win_x + 40, shutdown_win_y + 60, 60, 20, COLOR_BTN_BORDER);
    draw_text(shutdown_win_x + 58, shutdown_win_y + 64, "YES", WHITE_COLOR, COLOR_RED);
    draw_rect(shutdown_win_x + 150, shutdown_win_y + 60, 60, 20, COLOR_GREEN);
    draw_rect_border(shutdown_win_x + 150, shutdown_win_y + 60, 60, 20, COLOR_BTN_BORDER);
    draw_text(shutdown_win_x + 160, shutdown_win_y + 64, "NO", WHITE_COLOR, COLOR_GREEN);
}
void open_window() {
    if (!window_visible) return;
    window_w = 360;
    window_h = 240;
    rounded_rect(window_x, window_y, window_w, window_h, window_radius, COLOR_BTN_BG);
    draw_rect_border(window_x, window_y, window_w, window_h, COLOR_BTN_BORDER);
    draw_rect(window_x, window_y, window_w, 20, COLOR_ACCENT);
    int btn_size = 16;
    int btn_x = window_x + window_w - btn_size - 4;  
    int btn_y = window_y + 2;  
    draw_rect(btn_x, btn_y, btn_size, btn_size, COLOR_ACCENT);
    int char_width = 8;
    int char_height = 14;
    int text_x = btn_x + (btn_size - char_width) / 2;
    int text_y = btn_y + (btn_size - char_height) / 2;
    draw_text(text_x, text_y, "x", WHITE_COLOR, COLOR_ACCENT);
    char *line1 = "Small OS";
    char *line2 = "made by";
    char *line3 = "MalwareLab150";
    char *line4 = "using VESA VBE 800x600";
    int line1_x = window_x + (window_w - strlen(line1) * char_width) / 2;
    int line2_x = window_x + (window_w - strlen(line2) * char_width) / 2;
    int line3_x = window_x + (window_w - strlen(line3) * char_width) / 2;
    int line4_x = window_x + (window_w - strlen(line4) * char_width) / 2;
    int total_text_height = 4 * char_height;
    int start_y = window_y + (window_h - total_text_height) / 2;
    int line1_y = start_y;
    int line2_y = start_y + char_height;
    int line3_y = start_y + 2 * char_height;
    int line4_y = start_y + 3 * char_height;
    draw_text(line1_x, line1_y, line1, GREEN__, COLOR_BTN_BG);
    draw_text(line2_x, line2_y, line2, GREEN__, COLOR_BTN_BG);
    draw_text(line3_x, line3_y, line3, GREEN__, COLOR_BTN_BG);
    draw_text(line4_x, line4_y, line4, GREEN__, COLOR_BTN_BG);
}
void BARRA_SOPRA() {
    draw_rect(0, 0, SCREEN_WIDTH, STATUSBAR_HEIGHT, STATUSBAR_COLOR);
    draw_rect(POWER_BTN_X, POWER_BTN_Y, POWER_BTN_W, POWER_BTN_H, COLOR_ACCENT);
    draw_rect_border(POWER_BTN_X, POWER_BTN_Y, POWER_BTN_W, POWER_BTN_H, COLOR_BTN_BORDER);
    draw_text(POWER_BTN_X + 2, POWER_BTN_Y + (POWER_BTN_H - CHAR_HEIGHT) / 2, "SHT", WHITE_COLOR, COLOR_ACCENT);
}
int STARTER_UI() {
    int doom_icon_x = reboot_icon_x + icon_w + 2; // 20px di spazio a destra di reboot
    int doom_icon_y = reboot_icon_y; // Stessa altezza di reboot
    int doom_icon_w = ICON_WIDTH;
    int doom_icon_h = ICON_HEIGHT;
    int prev_window_x = window_x;
    int prev_window_y = window_y;
    int prev_shutdown_x = shutdown_window_x;
    int prev_shutdown_y = shutdown_window_y;
    int mouse_cycle = 0;
    int mouse_x = SCREEN_WIDTH / 2;
    int mouse_y = SCREEN_HEIGHT / 2;
    icon_x = (SCREEN_WIDTH - ICON_WIDTH) / 2;
    icon_y = (SCREEN_HEIGHT - ICON_HEIGHT) / 2;
    icon_w = ICON_WIDTH;
    icon_h = ICON_HEIGHT;
    reboot_icon_x = icon_x;
    reboot_icon_y = icon_y + icon_h + 30;
    clear_screen();
    SFONDO_(image_data, 320, 200, SCREEN_WIDTH, SCREEN_HEIGHT);
    BARRA_SOPRA();
    BARRA_UBUNTU_BUGGATA();
    icone_della_BARRA_UBUNTU_BUGGATA(20, COLOR_ACCENT);
    icone_della_BARRA_UBUNTU_BUGGATA(80, COLOR_BTN_BG);
    icone_della_BARRA_UBUNTU_BUGGATA(140, COLOR_BTN_BG);
    disegnamento_icone_strane(icon_x, icon_y, icon_w, icon_h, "INFO", COLOR_BTN_BG, COLOR_BTN_BORDER);
    disegnamento_icone_strane(reboot_icon_x, reboot_icon_y, icon_w, icon_h, "REBOOT", COLOR_BTN_BG, COLOR_BTN_BORDER);
    disegnamento_icone_strane(doom_icon_x, doom_icon_y, doom_icon_w, doom_icon_h, "dxl", COLOR_BTN_BG, COLOR_BTN_BORDER);
    enable_mouse();
    save_cursor_background(mouse_x, mouse_y);
    draw_mouse_cursor(mouse_x, mouse_y);
    uint8_t packet[3];
    uint8_t prev_left = 0;
    while (1) {
        
        uint8_t data = mouse_read();

        switch (mouse_cycle) {
            case 0:
                if (!(data & 0x08)) break;
                packet[0] = data;
                mouse_cycle = 1;
                break;
            case 1:
                packet[1] = data;
                mouse_cycle = 2;
                break;
            case 2:
                packet[2] = data;
                mouse_cycle = 0;
                uint8_t status = packet[0];
                int8_t dx = (int8_t)packet[1];
                int8_t dy = (int8_t)packet[2];
                uint8_t left = status & 0x01;
                restore_cursor_background(mouse_x, mouse_y);
                mouse_x += dx;
                mouse_y -= dy;
                if (mouse_x < 0) mouse_x = 0;
                if (mouse_x > SCREEN_WIDTH - CURSOR_WIDTH) mouse_x = SCREEN_WIDTH - CURSOR_WIDTH;
                if (mouse_y < 0) mouse_y = 0;
                if (mouse_y > SCREEN_HEIGHT - CURSOR_HEIGHT) mouse_y = SCREEN_HEIGHT - CURSOR_HEIGHT;
                save_cursor_background(mouse_x, mouse_y);
                draw_mouse_cursor(mouse_x, mouse_y);
                BARRA_SOPRA();
                int time_width = 8 * CHAR_WIDTH * SCALE;
                int time_x = (SCREEN_WIDTH - time_width) / 2;
                uint8_t hh, mm, ss, day, month;
                uint16_t year;
                get_rtc_datetime(&hh, &mm, &ss, &day, &month, &year);
                draw_time(time_x, 10, hh, mm, ss);
                draw_date(80, 10, day, month, year);
                BARRA_UBUNTU_BUGGATA();
                icone_della_BARRA_UBUNTU_BUGGATA(20, COLOR_ACCENT);
                icone_della_BARRA_UBUNTU_BUGGATA(80, COLOR_BTN_BG);
                icone_della_BARRA_UBUNTU_BUGGATA(140, COLOR_BTN_BG);
                disegnamento_icone_strane(icon_x, icon_y, icon_w, icon_h, "INFO", COLOR_BTN_BG, COLOR_BTN_BORDER);
                if (dragging_window) {
                    for (int y = 0; y < window_h; y++) {
                        for (int x = 0; x < window_w; x++) {
                            int sx = prev_window_x + x;
                            int sy = prev_window_y + y;
                            if (sx >= 0 && sx < SCREEN_WIDTH && sy >= 0 && sy < SCREEN_HEIGHT) {
                                int img_x = sx * 320 / SCREEN_WIDTH;
                                int img_y = sy * 200 / SCREEN_HEIGHT;
                                VIDEO_MEMORY_PTR[sy * SCREEN_WIDTH + sx] = image_data[img_y * 320 + img_x];
                            }
                        }
                    }
                }
                if (left && !prev_left &&
                    mouse_x >= doom_icon_x && mouse_x <= doom_icon_x + doom_icon_w &&
                    mouse_y >= doom_icon_y && mouse_y <= doom_icon_y + doom_icon_h) {
                    DOOM_();
                }
                if (left && !prev_left &&
  
                    mouse_x >= reboot_icon_x && mouse_x <= reboot_icon_x + icon_w &&
  
                    mouse_y >= reboot_icon_y && mouse_y <= reboot_icon_y + icon_h) {

                        REBOOT(); 

                    }
                if (left && !prev_left) {
                    if (window_visible &&
                        mouse_x >= window_x + window_w - 20 && mouse_x <= window_x + window_w - 4 &&
                        mouse_y >= window_y + 4 && mouse_y <= window_y + 20) {
                        SFONDO_(image_data, img_w, img_h, SCREEN_WIDTH, SCREEN_HEIGHT);
                        disegnamento_icone_strane(icon_x, icon_y, icon_w, icon_h, "INFO", COLOR_BTN_BG, COLOR_BTN_BORDER);
                        disegnamento_icone_strane(reboot_icon_x, reboot_icon_y, icon_w, icon_h, "REBOOT", COLOR_BTN_BG, COLOR_BG);
                        disegnamento_icone_strane(doom_icon_x, doom_icon_y, doom_icon_w, doom_icon_h, "dxl", COLOR_BTN_BG, COLOR_BTN_BORDER);
                        window_visible = 0;
                        dragging_window = 0;
                    }

                    if (window_visible &&
                        mouse_x >= window_x && mouse_x <= window_x + window_w &&
                        mouse_y >= window_y && mouse_y <= window_y + 20) {
                        dragging_window = 1;
                        drag_offset_x = mouse_x - window_x;
                        drag_offset_y = mouse_y - window_y;
                    }

                    if (!window_visible &&
                        mouse_x >= icon_x && mouse_x <= icon_x + icon_w &&
                        mouse_y >= icon_y && mouse_y <= icon_y + icon_h) {
                        window_visible = 1;
                    }
                }

                if (left && !prev_left &&
                    mouse_x >= POWER_BTN_X && mouse_x <= POWER_BTN_X + POWER_BTN_W &&
                    mouse_y >= POWER_BTN_Y && mouse_y <= POWER_BTN_Y + POWER_BTN_H) {
                    shutdown_window_visible = 1;
                }

                if (shutdown_window_visible) {
                    SHUTDOW_MESSAGEBOX();

                    int shutdown_x = shutdown_window_x;
                    int shutdown_y = shutdown_window_y;

                    if (left && !prev_left &&
                        mouse_x >= shutdown_x + 40 && mouse_x <= shutdown_x + 100 &&
                        mouse_y >= shutdown_y + 60 && mouse_y <= shutdown_y + 80) {
                        shutdown();
                    }

                    if (left && !prev_left &&
                        mouse_x >= shutdown_x + 150 && mouse_x <= shutdown_x + 210 &&
                        mouse_y >= shutdown_y + 60 && mouse_y <= shutdown_y + 80) {

                        SFONDO_(image_data, img_w, img_h, SCREEN_WIDTH, SCREEN_HEIGHT);
                       
                        disegnamento_icone_strane(reboot_icon_x, reboot_icon_y, icon_w, icon_h, "REBOOT", COLOR_BTN_BG, COLOR_BG);
                        disegnamento_icone_strane(icon_x, icon_y, icon_w, icon_h, "INFO", COLOR_BTN_BG, COLOR_BTN_BORDER);
                        disegnamento_icone_strane(doom_icon_x, doom_icon_y, doom_icon_w, doom_icon_h, "dxl", COLOR_BTN_BG, COLOR_BTN_BORDER);


                        shutdown_window_visible = 0; 
                    }

                    if (shutdown_window_visible && mouse_x >= shutdown_x && mouse_x <= shutdown_x + 250 && mouse_y >= shutdown_y && mouse_y <= shutdown_y + 100) {
                        dragging_shutdown_window = 1;
                        shutdown_drag_offset_x = mouse_x - shutdown_x;
                        shutdown_drag_offset_y = mouse_y - shutdown_y;
                    }

                    if (dragging_shutdown_window) {
                        shutdown_window_x = mouse_x - shutdown_drag_offset_x;
                        shutdown_window_y = mouse_y - shutdown_drag_offset_y;

                        if (shutdown_window_x < 0) shutdown_window_x = 0;
                        if (shutdown_window_y < 0) shutdown_window_y = 0;
                        if (shutdown_window_x + 250 > SCREEN_WIDTH) shutdown_window_x = SCREEN_WIDTH - 250;
                        if (shutdown_window_y + 100 > SCREEN_HEIGHT) shutdown_window_y = SCREEN_HEIGHT - 100;
                    }

                    if (!left) {
                        dragging_shutdown_window = 0;
                    }
                }

                if (!left) {
                    dragging_window = 0;
                }

                if (dragging_window) {
                    prev_window_x = window_x;
                    prev_window_y = window_y;

                    window_x = mouse_x - drag_offset_x;
                    window_y = mouse_y - drag_offset_y;

                    if (window_x < 0) window_x = 0;
                    if (window_y < 0) window_y = 0;
                    if (window_x + window_w > SCREEN_WIDTH) window_x = SCREEN_WIDTH - window_w;
                    if (window_y + window_h > SCREEN_HEIGHT) window_y = SCREEN_HEIGHT - window_h;
                } else {
                    prev_window_x = window_x;
                    prev_window_y = window_y;
                }

                if (window_visible) {
                    open_window();
                }

                prev_left = left;
                break;
        }
    }

    return 0;
}
