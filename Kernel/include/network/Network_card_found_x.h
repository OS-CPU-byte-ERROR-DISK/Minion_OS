#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA    0xCFC

uint32_t pci_read_dword(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
    uint32_t address =
        (1U << 31) |
        ((uint32_t)bus << 16) |
        ((uint32_t)device << 11) |
        ((uint32_t)function << 8) |
        (offset & 0xFC);
    outl(PCI_CONFIG_ADDRESS, address);
    return inl(PCI_CONFIG_DATA);
}
// is the qemu network card
int find_realtek_8139(uint8_t *bus_out, uint8_t *dev_out, uint8_t *func_out) {
    for (uint8_t bus = 0; bus < 256; bus++) {
        for (uint8_t device = 0; device < 32; device++) {
            uint32_t data = pci_read_dword(bus, device, 0, 0);
            uint16_t vendor_id = data & 0xFFFF;
            uint16_t device_id = (data >> 16) & 0xFFFF;
            if (vendor_id == 0x10EC && device_id == 0x8139) {
                *bus_out = bus;
                *dev_out = device;
                *func_out = 0;
                return 1;
            }
            if (vendor_id == 0xFFFF)
                break;
        }
    }
    return 0;
}

uint32_t pci_read_io_base(uint8_t bus, uint8_t dev, uint8_t func) {
    uint32_t bar0 = pci_read_dword(bus, dev, func, 0x10);
    return bar0 & ~0x3;
}
void Network() {
    uint8_t bus, dev, func;
    int y = 20; 

    if (find_realtek_8139(&bus, &dev, &func)) {
        uint32_t io_base = pci_read_io_base(bus, dev, func);
        if (io_base != 0) {
            draw_text(320, y++, "Network card found", 0x09, 0x00);  // verde
        } 
    }
}