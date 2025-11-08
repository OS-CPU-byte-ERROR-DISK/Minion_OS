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

void CMDS() {
    CMOS_INCULOS();
    draw_text(10, 245, "bye bye CMOS XD", 0x04, 0x00);
    while (1) {
        MUSIC_X(); 
    }
}
