// driver made by MalwareLab150. 
// yes im stupid :xd
//define + struct
#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA    0xCFC
extern void read_mac_rtl8139(uint32_t io_base, uint8_t* mac);
extern void read_mac_pcnet(uint32_t io_base, uint8_t* mac);
extern void read_mac_e1000(uint32_t mmio_base, uint8_t* mac);

typedef enum { IO, MMIO } AccessType;
typedef struct {
    const char* name;
    uint16_t vendor;
    uint16_t device;
    AccessType type;
    void (*read_mac)(uint32_t bar, uint8_t* mac);
} NICDriver;

// hardware thing (is for vmware/qemu/AMD.)
NICDriver drivers[] = {
    { "RTL8139",  0x10EC, 0x8139, IO,   read_mac_rtl8139 },
    { "E1000",    0x8086, 0x100E, MMIO, read_mac_e1000 },
    { "PCNET",    0x1022, 0x2000, IO,   read_mac_pcnet },
    { NULL, 0, 0, IO, NULL }
};
