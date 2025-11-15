#include "struct.h"
#include "font2.h"
#define VIDEO____ ((volatile uint8_t*)0xA0000)
uint32_t pci_read32(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address = (1 << 31) | (bus << 16) | (slot << 11) |
                       (func << 8) | (offset & 0xFC);
    outl(PCI_CONFIG_ADDRESS, address);
    return inl(PCI_CONFIG_DATA);
}

void draw_char(int x, int y, char c, uint8_t color) {
    for (int row = 0; row < 8; row++) {
        uint8_t line = font[(uint8_t)c][row];
        for (int col = 0; col < 8; col++) {
            if (line & (1 << (7 - col))) {
                VIDEO____[(y + row) * 800 + (x + col)] = color;
                VIDEO____[(y + row) * 800 + (x + col + 1)] = color;  // doppio pixel
            }
        }
    }
}
void print(int x, int y, const char* str, uint8_t color) {
    while (*str) {
        draw_char(x, y, *str++, color);
        x += 8;
    }
}
void print_mac(int x, int y, uint8_t mac[6]) {
    const char* hex = "0123456789ABCDEF";
    char buf[4];
    for (int i = 0; i < 6; i++) {
        buf[0] = hex[(mac[i] >> 4) & 0xF];
        buf[1] = hex[mac[i] & 0xF];
        buf[2] = (i < 5) ? ':' : '\0';
        buf[3] = '\0';
        print(x + i * 20, y, buf, 14);
    }
}

void read_mac_rtl8139(uint32_t io_base, uint8_t* mac) {
    for (int i = 0; i < 6; i++)
        mac[i] = inb(io_base + i);
}

void read_mac_pcnet(uint32_t io_base, uint8_t* mac) {
    for (int i = 0; i < 6; i++)
        mac[i] = inb(io_base + i);
}

void read_mac_e1000(uint32_t mmio_base, uint8_t* mac) {
    volatile uint32_t* mmio = (uint32_t*)mmio_base;
    uint32_t ral = mmio[0x5400 >> 2];
    uint32_t rah = mmio[0x5404 >> 2];
    mac[0] = ral & 0xFF;
    mac[1] = (ral >> 8) & 0xFF;
    mac[2] = (ral >> 16) & 0xFF;
    mac[3] = (ral >> 24) & 0xFF;
    mac[4] = rah & 0xFF;
    mac[5] = (rah >> 8) & 0xFF;
}

void detect_nics() {
    uint8_t mac[6];
    char buf[64];

    for (uint8_t bus = 0; bus < 256; bus++) {
        for (uint8_t slot = 0; slot < 32; slot++) {
            uint32_t id = pci_read32(bus, slot, 0, 0x00);
            uint16_t vendor = id & 0xFFFF;
            uint16_t device = (id >> 16) & 0xFFFF;

            for (int i = 0; drivers[i].name; i++) {
                if (drivers[i].vendor == vendor && drivers[i].device == device) {
                    uint32_t bar = pci_read32(bus, slot, 0, 0x10);
                    uint32_t base = (drivers[i].type == IO) ? (bar & ~0x7) : (bar & ~0xF);
                    drivers[i].read_mac(base, mac);
                    print(10, 10 + i * 20, "NIC:", 11);
                    print(50, 10 + i * 20, drivers[i].name, 15);
                    print_mac(150, 10 + i * 20, mac);
                }
            }
        }
    }
}

void MAC_ADRESS__() {
    clear_screen();
    detect_nics();
    while (1);
}
