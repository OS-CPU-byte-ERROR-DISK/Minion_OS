typedef uint8_t  byte;
typedef uint16_t word;
typedef uint32_t dword;
typedef struct {
    byte Signature[4];
    uint32_t Length;
    byte unneeded1[40 - 8];
    uint32_t DSDT;
    byte unneeded2[48 - 44];
    uint32_t SMI_CMD;
    byte ACPI_ENABLE;
    byte ACPI_DISABLE;
    byte unneeded3[64 - 54];
    uint32_t PM1a_CNT_BLK;
    uint32_t PM1b_CNT_BLK;
    byte unneeded4[89 - 72];
    byte PM1_CNT_LEN;
} __attribute__((packed)) FADT;