#include "bios_info.h"
void cpu_brand_string(char* out_str) {
    uint32_t regs[4];
    char* p = out_str;

    for (int i = 0; i < 3; i++) {
        cpuid(0x80000002 + i, &regs[0], &regs[1], &regs[2], &regs[3]);
        for (int j = 0; j < 4; j++) {
            *(uint32_t*)p = regs[j];
            p += 4;
        }
    }
    out_str[48] = '\0';
}
void clear_screen() {
    uint32_t* framebuffer = (uint32_t*)0xFD000000; 
    uint32_t black_pixel = 0x00000000;  
    for (int y = 0; y < 1080; y++) {
        for (int x = 0; x < 1920; x++) {
            framebuffer[y * 1920 + x] = black_pixel;  
        }
    }
}
void HARDWAR_() {
    clear_screen();
    DISK_DISK_DISK_DISK();
    char brand[49];
    char full_line[64];
    int i = 0, j = 0;
    const char* label;
    char vendor[13];
    uint32_t eax, ebx, ecx, edx;
    int y = 40;
    cpuid(0, &eax, &ebx, &ecx, &edx);
    *(uint32_t*)&vendor[0]  = ebx;
    *(uint32_t*)&vendor[4]  = edx;
    *(uint32_t*)&vendor[8]  = ecx;
    vendor[12] = '\0';
    draw_text(10, y, "CPU Vendor: ", 0x0F, 0x00);
    draw_text(130, y, vendor, 0x0F, 0x00);
    y += 40;
    if (vendor[0] == 'G' && vendor[1] == 'e') {
        draw_text(10, y, "Vendor is: INTEL", 0x0B, 0x00);
    } else if (vendor[0] == 'A' && vendor[1] == 'u') {
        draw_text(10, y, "Vendor is: AMD", 0x0B, 0x00);
    } else {
        draw_text(10, y, "Vendor is: UNKNOWN", 0x0C, 0x00);
    }
   cpu_brand_string(brand);
    i = 0;
    label = "Brand: ";
    while (*label) full_line[i++] = *label++;
    j = 0;
    while (brand[j]) full_line[i++] = brand[j++];
    full_line[i] = '\0';

        BIOS_INFOSZ();
    draw_text(0, 2, full_line, 0x0F, 0x00);
}