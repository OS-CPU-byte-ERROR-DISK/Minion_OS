#define IDT_ENTRIES 256  
// avvia la IDT + Inutile dire che usa il file .asm (controlla il file IDT.asm )



/// NON è COMPLETO. NON è COMPLETO.
extern void idt_flush(uint32_t);
extern void rtc_isr();
struct idt_entry {
    uint16_t base_low;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct idt_entry idt[IDT_ENTRIES];
struct idt_ptr idtp;

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

extern void rtc_ind(uint32_t idtp_addr);

void IDT_STARTER_POINT() {
    idtp.limit = (sizeof(struct idt_entry) * IDT_ENTRIES) - 1;
    idtp.base = (uint32_t)&idt;

    for (int i = 0; i < IDT_ENTRIES; i++) {
        idt_set_gate(i, 0, 0, 0);
    }

    rtc_ind((uint32_t)&idtp); 
}

void init_idt_irq() {
    idt_set_gate(0x28, (uint32_t)rtc_ind, 0x08, 0x8E); 
}
