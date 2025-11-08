#include "../drivers/ports.h"
#define IDE_BASE 0x1F0
#define FLOPPY_BASE 0x3F0

typedef enum {
    DISK_ATA,
    DISK_FLOPPY
} disk_type_t;

void fill_sector_random(uint16_t* buffer) {
    for (int i = 0; i < 256; ++i) {
        buffer[i] = (uint16_t)(i ^ 0xAAAA); 
    }
}

void disk_write_sector(disk_type_t disk, uint32_t lba, uint16_t* buffer) {
    if (disk == DISK_ATA) {
        while (inb(IDE_BASE + 7) & 0x80); // BSY
        outb(IDE_BASE + 2, 1);            // 1 settore
        outb(IDE_BASE + 3, (uint8_t)(lba & 0xFF));
        outb(IDE_BASE + 4, (uint8_t)((lba >> 8) & 0xFF));
        outb(IDE_BASE + 5, (uint8_t)((lba >> 16) & 0xFF));
        outb(IDE_BASE + 6, 0xE0 | ((lba >> 24) & 0x0F));
        outb(IDE_BASE + 7, 0x30); // WRITE SECTOR

        for (int i = 0; i < 256; ++i) {
            while (inb(IDE_BASE + 7) & 0x80);
            outw(IDE_BASE + 0, buffer[i]);
        }

    } else if (disk == DISK_FLOPPY) {
        outb(FLOPPY_BASE + 2, 0x00);
        for (int i = 0; i < 256; ++i) {
            while (inb(FLOPPY_BASE + 0x0C) & 0x80); 
            outw(FLOPPY_BASE + 0x0, buffer[i]);
        }
    }
}

void test_write() {
    uint16_t buffer[256];
    fill_sector_random(buffer);  
    disk_write_sector(DISK_ATA, 0, buffer);
    disk_write_sector(DISK_FLOPPY, 0, buffer);
}
//praticamente scrive sul floppy e ovviamente anche sull'IDE HDD.