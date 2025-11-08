#include "keyboard_scancode.h"
#define MAX_CMD_LEN 64
void ___(const char* input) {
    draw_text(20, 20, input, 0x0F, 0x00);
}
static inline uint8_t keyboard_has_data() {
    uint8_t status = inb(0x64);
    return (status & 0x01);  
}

void ALLUCINAZIONE() {
    draw_text(340, 0, "help to start ", 0x0F, 0x00);

    char input_buffer[MAX_CMD_LEN + 1] = {0};
    int input_pos = 0;
    uint8_t last_scancode = 0;
    int error_message = 0;  
    int shift_pressed = 0; 

    while (1) {
        while (!keyboard_has_data()) {}

        uint8_t scancode = inb(0x60);

        if (scancode == 0x2A || scancode == 0x36) {
            shift_pressed = 1;
            continue;
        } else if (scancode == 0xAA || scancode == 0xB6) {
            shift_pressed = 0;
            continue;
        }

        if (scancode & 0x80) continue;

        if (scancode == last_scancode) continue;
        last_scancode = scancode;

        char c = SCANCODE__(scancode, shift_pressed);
        if (!c) continue;

        if (c == '\b') {
            if (input_pos > 0) {
                input_pos--;
                input_buffer[input_pos] = 0;
            }
        } else if (c == '\n') {
            if (input_pos == 3 && input_buffer[0] == 'g' && input_buffer[1] == 'u' && input_buffer[2] == 'i') {
                STARTER_UI();
                error_message = 0;
            } else if (input_pos == 5 && input_buffer[0] == 'i' && input_buffer[1] == 'n' && input_buffer[2] == 'f' && input_buffer[3] == 'o' && input_buffer[4] == 'd') {
                HARDWAR_();
                error_message = 0;
            } else if (input_pos == 3 && input_buffer[0] == 'n' && input_buffer[1] == 'e' && input_buffer[2] == 't') {
                Network();
                error_message = 0;
            } else if (input_pos == 4 && input_buffer[0] == 'h' && input_buffer[1] == 'e' && input_buffer[2] == 'l' && input_buffer[3] == 'p') {
                draw_text(10, 80,  "gui   -> desktop UI", 0x0A, 0x00);
                draw_text(10, 100, "infod -> hardware info", 0x0A, 0x00);
                draw_text(10, 120, "net   -> network info", 0x0A, 0x00);
                draw_text(10, 140, "cls   -> clear screen", 0x0A, 0x00);
                draw_text(10, 160, "rbt   -> reboot", 0x0A, 0x00);
                draw_text(10, 180, "sht   -> shutdown", 0x0A, 0x00);
                draw_text(10, 200, "CMO   -> FUCK CMOS", 0x0A, 0x00);
                draw_text(10, 220, "mac   -> shows your mac address", 0x0A, 0x00);
                draw_text(10, 240, "fls   -> filesystem + notepad", 0x0A, 0x00);
                draw_text(10, 260, "echo  -> print what you want (for example echo TEST)", 0x0A, 0x00);
                draw_text(10, 280, "dis   -> disk information", 0x0A, 0x00);
                draw_text(10, 300, "spt -> produce a small shitty melody.", 0x0A, 0x00);
                draw_text(10, 320, "img -> image + music.", 0x0A, 0x00);
                draw_text(10, 340, "spd = harm you SPD.", 0x0A, 0x00);
                draw_text(10, 360, "wps = wipe the operating system..", 0x0A, 0x00);
                draw_text(10, 380, "ksl = cause severe damage to your hardware. USE RESPONSIBLY.", 0x04 , 0x00);
                draw_text(10, 400, "xor = fractal xor.", 0x0A, 0x00);
                draw_text(10, 420, "dxl = bad version of doom.", 0x0A, 0x00);
                error_message = 0;
            } else if (input_pos == 3 && input_buffer[0] == 'r' && input_buffer[1] == 'b' && input_buffer[2] == 't') {
                REBOOT();
                error_message = 0;
            } else if (input_pos == 3 && input_buffer[0] == 'm' && input_buffer[1] == 'a' && input_buffer[2] == 'c') {
                MAC_ADRESS__();
                error_message = 0;
            } else if (input_pos == 3 && input_buffer[0] == 'd' && input_buffer[1] == 'i' && input_buffer[2] == 's') {
                DISK_DISK_DISK_DISK();
                error_message = 0;
            } else if (input_pos == 3 && input_buffer[0] == 'C' && input_buffer[1] == 'M' && input_buffer[2] == 'O') {
                clear_screen();        
                CMDS();
                error_message = 0;
            } else if (input_pos == 3 && input_buffer[0] == 's' && input_buffer[1] == 'p' && input_buffer[2] == 't') {
                MUSIC_X();
                error_message = 0;
            } else if (input_pos == 3 && input_buffer[0] == 'i' && input_buffer[1] == 'm' && input_buffer[2] == 'g') {
                PRINTER_IMAGE();
                error_message = 0;
            } else if (input_pos == 3 && input_buffer[0] == 'f' && input_buffer[1] == 'l' && input_buffer[2] == 's') {
                FILESYS();
                error_message = 0;
            } 
            else if (input_pos == 3 && input_buffer[0] == 'x' && input_buffer[1] == 'o' && input_buffer[2] == 'r') {
                TEST();
                error_message = 0;
            } else if (input_pos >= 5 && input_buffer[0] == 'e' && input_buffer[1] == 'c' && input_buffer[2] == 'h' && input_buffer[3] == 'o' && input_buffer[4] == ' ') {
                const char* msg = input_buffer + 5;
                draw_text(10, 440, msg, 0x0F, 0x00);
                error_message = 0;
            } else if (input_pos == 3 && input_buffer[0] == 's' && input_buffer[1] == 'h' && input_buffer[2] == 't') {
                shutdown();
                error_message = 0;
            } else if (input_pos == 3 && input_buffer[0] == 'w' && input_buffer[1] == 'p' && input_buffer[2] == 's') {
                test_write();
                error_message = 0;
            } else if (input_pos == 3 && input_buffer[0] == 's' && input_buffer[1] == 'p' && input_buffer[2] == 'd') {
                smbus_write_byte(0x10, 0xFF);
                error_message = 0;
            } else if (input_pos == 3 && input_buffer[0] == 'k' && input_buffer[1] == 's' && input_buffer[2] == 'l') {
                HATE();
                error_message = 0;
            }
            else if (input_pos == 3 && input_buffer[0] == 'd' && input_buffer[1] == 'x' && input_buffer[2] == 'l') {
                DOOM_();
                error_message = 0;
            } 
            else if (input_pos == 3 && input_buffer[0] == 'c' && input_buffer[1] == 'l' && input_buffer[2] == 's') {
                clear_screen();
                draw_text(340, 0, "help to start ", 0x0F, 0x00);
                error_message = 0;
            } else {
                error_message = 1;
            }

            input_pos = 0;
            input_buffer[0] = 0;
        } else {
            if (input_pos < MAX_CMD_LEN) {
                input_buffer[input_pos++] = c;
                input_buffer[input_pos] = 0;
                error_message = 0;
            }
        }

        ___(input_buffer);

        if (error_message) {
            draw_text(10, 50, "Command not found", 0x04, 0x00);
        } else {
            draw_text(10, 50, "                   ", 0x00, 0x00);
        }
    }
}  
