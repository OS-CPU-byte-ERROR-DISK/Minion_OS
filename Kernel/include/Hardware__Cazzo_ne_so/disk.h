#define ATA_PRIMARY_BASE 0x1F0
#define ATA_SECONDARY_BASE 0x170
int HDD_FOUND(uint16_t base_port, int slave);
int HDD_SCAN(uint16_t base_port, int slave, uint16_t* buffer);
void PRINTAMENTO_DELLE_INFO(uint16_t* id_data, int row);

void scan_ata_devices() {
    uint16_t id_data[256];
    int row = 320;
    uint16_t ports[2] = {ATA_PRIMARY_BASE, ATA_SECONDARY_BASE};

    for (int ch = 0; ch < 2; ch++) {
        for (int sl = 0; sl < 2; sl++) {
            if (HDD_FOUND(ports[ch], sl)) {
                if (HDD_SCAN(ports[ch], sl, id_data)) {
                    PRINTAMENTO_DELLE_INFO(id_data, row++);
                } else {
                    draw_text(0, row++, "ATA device NOT FOUND (or is not working)", 0x4E, 0);
                }
                return;
            }
        }
    }    
      draw_text(0, row, "No ATA devices found", 0x07, 0);
}

int HDD_FOUND(uint16_t base_port, int slave) {
    uint8_t dev = slave ? 0xB0 : 0xA0;
    outb(base_port + 6, dev);
    for (volatile int i = 0; i < 10000; i++);
    uint8_t status = inb(base_port + 7);
    return (status != 0 && status != 0xFF);
}

int HDD_SCAN(uint16_t base_port, int slave, uint16_t* buffer) {
    const int TIMEOUT = 100000;
    uint8_t dev = slave ? 0xB0 : 0xA0;

    outb(base_port + 6, dev);
    for (volatile int i = 0; i < 10000; i++);

    outb(base_port + 7, 0xEC);

    int t = TIMEOUT;
    uint8_t status;
    do {
        status = inb(base_port + 7);
    } while ((status == 0 || status == 0xFF) && t--);
    if (t <= 0 || status == 0 || status == 0xFF) {
        return 0;
    }

    t = TIMEOUT;
    while (!(inb(base_port + 7) & 0x08) && t--);
    if (t <= 0) {
        return 0;
    }

    for (int i = 0; i < 256; i++) {
        buffer[i] = inw(base_port);
    }

    return 1;
}

void PRINTAMENTO_DELLE_INFO(uint16_t* id_data, int row) {
    char model[41];
    for (int i = 0; i < 40; i += 2) {
        model[i]   = (id_data[27 + i/2] >> 8) & 0xFF;
        model[i+1] = id_data[27 + i/2] & 0xFF;
    }
    model[40] = 0;
    for (int i = 39; i >= 0; i--) {
        if (model[i] == ' ' || model[i] == 0) model[i] = 0;
        else break;
    }
    char msg[80];
    int pos = 0;
    const char* prefix = "ATA IDE Device: ";
    for (; prefix[pos]; pos++) msg[pos] = prefix[pos];
    for (int i = 0; model[i]; i++) msg[pos++] = model[i];
    msg[pos] = 0;
   draw_text(0, row, msg, 0x2F, 0);
}

void DISK_DISK_DISK_DISK() {
    clear_screen(0x00);
   scan_ata_devices(); 
}
