#include "ports.h"
#define COLOR_CURSOR 0x00  // nero
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define CURSOR_WIDTH 24
#define CURSOR_HEIGHT 39
#define COLOR_BG 4         // Grigio scuro
#define KEYBOARD_DATA    0x60
#define STATUS_PORT      0x64
#define COMMAND_PORT     0x64
#define OUTPUT_BUFFER_FULL 0x01
#define INPUT_BUFFER_FULL  0x02
#define MOUSE_DATA_BIT     0x20 
uint8_t cursor_background[CURSOR_WIDTH * CURSOR_HEIGHT];
unsigned char *VIDEO_MEMORY_PTR = (unsigned char *)0xFD000000;

const uint16_t mouse_cursor_bitmap[16] = {
    0b10000000, 
    0b11000000, 
    0b11100000, 
    0b11110000, 
    0b11111000, 
    0b11111100,
    0b11111110, 
    0b11111111, 
    0b11111111, 
    0b11111111, 
    0b11111111, 
    0b11111111, 
    0b11000011, 
    0b00000011, 
    0b00000001, 
    0b000111100,
    0000000,
    0b0011111100,
    000000,
    0b0111111110000000,
    0b1111111111000000,
    0b1111111111000000,
    0b1111111111000000,
    0b1111111111000000,
    0b0111111110000000,
    0b0111111110000000,
    0b0011111100000000,
    0b0001111000000000,
    0b0000110000000000,
    0b0000010000000000,
    0b0000010000000000,
    0b0000000000000000,
    0b0000000000000000,
};

void save_cursor_background(int x, int y) {
    for (int row = 0; row < CURSOR_HEIGHT; row++) {
        for (int col = 0; col < CURSOR_WIDTH; col++) {
            int px = x + col;
            int py = y + row;
            if (px >= 0 && px < SCREEN_WIDTH && py >= 0 && py < SCREEN_HEIGHT)
                cursor_background[row * CURSOR_WIDTH + col] = VIDEO_MEMORY_PTR[py * SCREEN_WIDTH + px];
            else
                cursor_background[row * CURSOR_WIDTH + col] = COLOR_BG;
        }
    }
}

void restore_cursor_background(int x, int y) {
    for (int row = 0; row < CURSOR_HEIGHT; row++) {
        for (int col = 0; col < CURSOR_WIDTH; col++) {
            int px = x + col;
            int py = y + row;
            if (px >= 0 && px < SCREEN_WIDTH && py >= 0 && py < SCREEN_HEIGHT)
                VIDEO_MEMORY_PTR[py * SCREEN_WIDTH + px] = cursor_background[row * CURSOR_WIDTH + col];
        }
    }
}

void draw_mouse_cursor(int x, int y) {
    for (int row = 0; row < 16; row++) {
        uint16_t bits = mouse_cursor_bitmap[row];
        for (int col = 0; col < 10; col++) {
            if ((bits >> (9 - col)) & 1) {
                int px = x + col;
                int py = y + row;
                if (px >= 0 && px < SCREEN_WIDTH && py >= 0 && py < SCREEN_HEIGHT)
                    VIDEO_MEMORY_PTR[py * SCREEN_WIDTH + px] = COLOR_CURSOR;
            }
        }
    }
}

void wait_mouse_input_buffer_empty() {
    while (inb(STATUS_PORT) & INPUT_BUFFER_FULL);
}

void wait_mouse_output_buffer_full() {
 
    while (!(inb(STATUS_PORT) & OUTPUT_BUFFER_FULL));
    
    if (!(inb(STATUS_PORT) & MOUSE_DATA_BIT)) {
        inb(KEYBOARD_DATA);
        return wait_mouse_output_buffer_full(); 
    }
}

void mouse_write(uint8_t val) {
    wait_mouse_input_buffer_empty();
    outb(COMMAND_PORT, 0xD4);  
    wait_mouse_input_buffer_empty();
    outb(KEYBOARD_DATA, val);  
}

uint8_t mouse_read() {
    wait_mouse_output_buffer_full();
    return inb(KEYBOARD_DATA);
}

void enable_mouse() {
    wait_mouse_input_buffer_empty();
    outb(COMMAND_PORT, 0xA8);  
    mouse_write(0xF4);        
}
