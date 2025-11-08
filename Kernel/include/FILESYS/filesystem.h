#include "fls_define.h"
//save on ram
void fs_save() {
    __builtin_memcpy(ram_disk_sector, (uint8_t*)filesystem, sizeof(filesystem));
    ram_disk_initialized = true;
}
//loads from the ram.
void fs_load() {
    if (!ram_disk_initialized) return;
    __builtin_memcpy((uint8_t*)filesystem, ram_disk_sector, sizeof(filesystem));
}
int find_file_by_name(const char* name) {
    for (int f = 0; f < MAX_FILES; f++) {
        if (filesystem[f].used) {
            bool match = true;
            for (int i = 0; i < MAX_FILENAME_LEN; i++) {
                if (filesystem[f].name[i] != name[i]) {
                    match = false; break;
                }
                if (name[i] == 0) break;
            }
            if (match) return f;
        }
    }
    return -1;
}

void delete_file_by_name(const char* name) {
    int idx = find_file_by_name(name);
    if (idx >= 0) {
        filesystem[idx].used = false;
        for (int i = 0; i < MAX_FILENAME_LEN; i++) filesystem[idx].name[i] = 0;
        filesystem[idx].size = 0;
        for (int i = 0; i < MAX_FILE_SIZE; i++) filesystem[idx].content[i] = 0;
    }
}

void create_empty_file(const char* name) {
    for (int f = 0; f < MAX_FILES; f++) {
        if (!filesystem[f].used) {
            filesystem[f].used = true;
            for (int i = 0; i < MAX_FILENAME_LEN; i++) filesystem[f].name[i] = 0;
            int i = 0;
            while (name[i] && i < MAX_FILENAME_LEN - 1) {
                filesystem[f].name[i] = name[i];
                i++;
            }
            filesystem[f].size = 0;
            for (int j = 0; j < MAX_FILE_SIZE; j++) filesystem[f].content[j] = 0;
            cmos_tiex(&filesystem[f].sec, &filesystem[f].min, &filesystem[f].hour,
                      &filesystem[f].day, &filesystem[f].month, &filesystem[f].year);
            return;
        }
    }
}
// Legge solo scancode della tastiera (scarta dati mouse)
uint8_t read_keyboard_scancode() {
    uint8_t status;
    do { 
        status = inb(0x64);
        if (status & 0x01) {
            if (!(status & 0x20)) {  // Dati dalla tastiera
                return inb(0x60);
            } else {
                // Scarta dati mouse
                inb(0x60);
            }
        }
    } while (1);
}

uint8_t read_mouse_data() {
    uint8_t status;
    do { 
        status = inb(0x64);
        if (status & 0x01) {
            if (status & 0x20) {  
                return inb(0x60);
            } else {
                inb(0x60);
            }
        }
    } while (1);
}

char sc_ascii(uint8_t sc) {
    static const char map[128] = {
        0,27,'1','2','3','4','5','6','7','8','9','0','-','=','\b',
        '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',0,
        'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\',
        'z','x','c','v','b','n','m',',','.','/',0,'*',0,' ',0
    };
    if (sc < 128) return map[sc];
    return 0;
}

int read_line(char* buf, int max_len) {
    int pos = 0;
    int cx = 24; 
    int cy = 44;

    while (1) {
        uint8_t sc = read_keyboard_scancode();  
        if (sc & 0x80) continue;

        if (sc == 0x01) { // ESC
            buf[0] = 0;
            return -1;
        }

        char c = sc_ascii(sc);

        if (sc == 0x1C) { // enter
            buf[pos] = 0;
            return pos;
        } 
        else if (sc == 0x0E) { 
            if (pos > 0) {
                pos--;
                buf[pos] = 0; 
                cx -= 8;
                draw_char(cx, cy, ' ', 0); 
            }
        }
        else if (c >= 32 && pos < max_len - 1) {
            buf[pos++] = c;
            draw_char(cx, cy, c, 15);
            cx += 8;
        }
    }
}

void file_editor(int file_idx) {
    clear_screen(0);
    print(8, 8, "Notepad - ESC=exit, DEL=save", 15);
    int pos = filesystem[file_idx].size;
    int cx = 8, cy = 24;
    for (int i = 0; i < pos; i++) {
        char c = filesystem[file_idx].content[i];
        draw_char(cx, cy, c, 14);
        cx += 8; if (cx >= 640) { cx = 8; cy += 8; }
    }
    while (1) {
        uint8_t sc = read_keyboard_scancode();  
        if (sc & 0x80) continue;
        if (sc == 0x01) break; // ESC
        if (sc == 0x53) {
            filesystem[file_idx].size = pos;
            fs_save();
            clear_screen(0);
            print(8, 8, "File saved.", 15);
            return;
        }
        if (sc == 0x0E) {
            if (pos > 0) {
                pos--; cx -= 8;
                if (cx < 8) { cy -= 8; cx = 632; }
                draw_char(cx, cy, ' ', 0);
            }
            continue;
        }
        char c = sc_ascii(sc);
        if (c >= 32 && pos < MAX_FILE_SIZE) {
            filesystem[file_idx].content[pos++] = c;
            draw_char(cx, cy, c, 14);
            cx += 8; if (cx >= 640) { cx = 8; cy += 8; }
        }
    }
}
// Shell
void CMD_() {
    clear_screen(0);
    print(8, 8, "Shell: 'mrd' = new file or open a file, 'dr454' = delete, ESC = exit , ls = list", 15);
    char input[64];
    while (1) {
        print(8, 60, "> ", 15);
        int len = read_line(input, 64);
        if (len < 0) return;
        if (len == 0) continue;

        // create new file - open a file
        if (len == 3 && input[0] == 'm' && input[1] == 'r' && input[2] == 'd') {
            clear_screen(0); print(8, 8, "Enter filename:", 15);
            char fname[MAX_FILENAME_LEN];
            int n = read_line(fname, MAX_FILENAME_LEN);
            if (n <= 0) continue;
            create_empty_file(fname);
            fs_save();
            int idx = find_file_by_name(fname);
            if (idx >= 0) file_editor(idx);
        }

        // list files
        else if (len == 2 && input[0] == 'l' && input[1] == 's') {
            clear_screen(0);
            print(8, 8, "File list:", 15);
            int y = 24;

            char printed_names[MAX_FILES][MAX_FILENAME_LEN];
            int printed_count = 0;

            for (int i = 0; i < MAX_FILES; i++) {
                if (!filesystem[i].used) continue;
                bool already_printed = false;
                for (int p = 0; p < printed_count; p++) {
                    bool match = true;
                    for (int c = 0; c < MAX_FILENAME_LEN; c++) {
                        if (printed_names[p][c] != filesystem[i].name[c]) {
                            match = false;
                            break;
                        }
                        if (filesystem[i].name[c] == 0) break;
                    }
                    if (match) {
                        already_printed = true;
                        break;
                    }
                }

                if (already_printed) continue;

                for (int c = 0; c < MAX_FILENAME_LEN; c++)
                    printed_names[printed_count][c] = filesystem[i].name[c];
                printed_count++;

                char line[64];
                int pos = 0;

                for (int j = 0; j < MAX_FILENAME_LEN && filesystem[i].name[j]; j++)
                    line[pos++] = filesystem[i].name[j];

                line[pos++] = ' ';
                line[pos++] = '(';

                int size = filesystem[i].size;
                if (size == 0) {
                    line[pos++] = '0';
                } else {
                    char digits[6];
                    int digit_count = 0;
                    while (size > 0 && digit_count < 6) {
                        digits[digit_count++] = '0' + (size % 10);
                        size /= 10;
                    }
                    for (int d = digit_count - 1; d >= 0; d--)
                        line[pos++] = digits[d];
                }

                line[pos++] = ' ';
                line[pos++] = 'b';
                line[pos++] = 'y';
                line[pos++] = 't';
                line[pos++] = 'e';
                line[pos++] = 's';
                line[pos++] = ')';
                line[pos++] = ' ';

                line[pos++] = (filesystem[i].day / 10) + '0';
                line[pos++] = (filesystem[i].day % 10) + '0';
                line[pos++] = '/';
                line[pos++] = (filesystem[i].month / 10) + '0';
                line[pos++] = (filesystem[i].month % 10) + '0';
                line[pos++] = '/';
                line[pos++] = (filesystem[i].year / 10) + '0';
                line[pos++] = (filesystem[i].year % 10) + '0';
                line[pos++] = ' ';
                line[pos++] = (filesystem[i].hour / 10) + '0';
                line[pos++] = (filesystem[i].hour % 10) + '0';
                line[pos++] = ':';
                line[pos++] = (filesystem[i].min / 10) + '0';
                line[pos++] = (filesystem[i].min % 10) + '0';
                line[pos++] = ':';
                line[pos++] = (filesystem[i].sec / 10) + '0';
                line[pos++] = (filesystem[i].sec % 10) + '0';

                line[pos] = 0;
                print(8, y, line, 14);
                y += 8;
            }
        }
        // delete file
        else if (len == 5 && input[0] == 'd' && input[1] == 'r' &&
                 input[2] == '4' && input[3] == '5' && input[4] == '4') {
            clear_screen(0); print(8, 8, "Enter filename to delete:", 15);
            char fname[MAX_FILENAME_LEN];
            int n = read_line(fname, MAX_FILENAME_LEN);
            if (n <= 0) continue;
            int idx = find_file_by_name(fname);
            if (idx >= 0) {
                delete_file_by_name(fname);
                fs_save();
                clear_screen(0); print(8, 8, "File deleted.", 15);
            } else {
                clear_screen(0); print(8, 8, "File not found.", 15);
            }
        }
    }
}

// Entry point
int FILESYS() {
    fs_load();
    CMD_();
    return 0;
}

