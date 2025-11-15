void KEY_RBT() {
    uint8_t good = 0x02;
    while (good & 0x02)
        good = inl(0x64); 
    outb(0x64, 0xFE); 
    while (1) {
        __asm__ volatile ("hlt");
    }
}

void REBOOT() {
    KEY_RBT();
    while (1) {
    }
}