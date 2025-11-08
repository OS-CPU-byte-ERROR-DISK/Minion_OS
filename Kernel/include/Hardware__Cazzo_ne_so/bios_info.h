#define BIOS_START 0xF0000
#define BIOS_END   0x100000
#define GREEN_ON_BLACK 0x2F
#define RED_ON_BLACK   0x4F
#define CYAN_ON_BLACK  0x3F
#define WHITE_ON_BLACK 0x0F
void BIOS_INFOSZ() {
    const char* known_vendors[] = {
        "American", "Award", "Phoenix", "AMI", "Insyde",
        "Dell", "HP", "Lenovo", "ASUS", "Acer", "MSI", "Gigabyte",
        "Toshiba", "Sony", "Samsung", "Intel", "Compaq", "Packard",
        "Fujitsu", "Medion", "eMachines", "Hewlett", "Gateway",
        "ASRock", "Biostar", "Clevo", "Shuttle", "Tyan", "Supermicro",
        "Panasonic", "Sharp", "Microsoft", "Zotac", "Huawei"
    };

    const char* bios_types[] = {
        "SeaBIOS", "AMI", "Phoenix", "Award", "Insyde"
    };

    const char* vm_signatures[] = {
        "VirtualBox", "QEMU", "Bochs", "VMware", "Hyper-V", "KVM"
    };

    volatile char* bios = (volatile char*)BIOS_START;
    char vendor[64] = {0};
    char version[64] = {0};
    char date[32] = {0};
    char bios_type[32] = {0};
    char vm_name[32] = {0};

    // search for BIOS informations
    for (int i = 0; i < (BIOS_END - BIOS_START - 8); i++) {
        for (int t = 0; t < sizeof(bios_types) / sizeof(bios_types[0]); t++) {
            const char* sig = bios_types[t];
            int j = 0;
            while (sig[j] && bios[i + j] == sig[j]) j++;
            if (sig[j] == '\0') {
                int k = 0;
                for (; k < 31 && bios[i + k] >= 32 && bios[i + k] <= 126; k++) {
                    bios_type[k] = bios[i + k];
                }
                bios_type[k] = '\0';
                goto found_type;
            }
        }
    }
found_type:

    // check if is a vm
    for (int i = 0; i < (BIOS_END - BIOS_START - 16); i++) {
        for (int v = 0; v < sizeof(vm_signatures) / sizeof(vm_signatures[0]); v++) {
            const char* sig = vm_signatures[v];
            int j = 0;
            while (sig[j] && bios[i + j] == sig[j]) j++;
            if (sig[j] == '\0') {
                int k = 0;
                for (; k < 31 && bios[i + k] >= 32 && bios[i + k] <= 126; k++) {
                    vm_name[k] = bios[i + k];
                }
                vm_name[k] = '\0';
                goto found_vm;
            }
        }
    }
found_vm:

    // search BIOS vendor
    for (int i = 0; i < (BIOS_END - BIOS_START - 32); i++) {
        for (int v = 0; v < sizeof(known_vendors)/sizeof(known_vendors[0]); v++) {
            const char* match = known_vendors[v];
            int j = 0;
            while (match[j] && bios[i + j] == match[j]) j++;

            if (match[j] == '\0') {
                int k = 0;
                for (; k < 63 && bios[i + k] >= 32 && bios[i + k] <= 126; k++) {
                    vendor[k] = bios[i + k];
                }
                vendor[k] = '\0';
                goto found_vendor;
            }
        }
    }
found_vendor:

    // search for BIOS data
    for (int i = 0; i < (BIOS_END - BIOS_START - 10); i++) {
        if ((bios[i + 2] == '/' || bios[i + 4] == '/') && bios[i] >= '0' && bios[i] <= '9') {
            int valid = 1;
            for (int j = 0; j < 10; j++) {
                char c = bios[i + j];
                if (!(c == '/' || (c >= '0' && c <= '9'))) {
                    valid = 0;
                    break;
                }
            }
            if (valid) {
                for (int j = 0; j < 10; j++) date[j] = bios[i + j];
                date[10] = '\0';
                break;
            }
        }
    }

    // bios version
    const char* version_keywords[] = {"Version", "Ver", "REV", "Rev", "BIOS ", "V."};
    for (int i = 0; i < (BIOS_END - BIOS_START - 16); i++) {
        for (int w = 0; w < sizeof(version_keywords)/sizeof(version_keywords[0]); w++) {
            const char* kw = version_keywords[w];
            int j = 0;
            while (kw[j] && bios[i + j] == kw[j]) j++;
            if (kw[j] == '\0') {
                int k = 0;
                for (; k < 60 && bios[i + k] >= 32 && bios[i + k] <= 126; k++) {
                    version[k] = bios[i + k];
                }
                version[k] = '\0';
                goto found_version;
            }
        }
    }
found_version:

    int y = 160;  
    int x = 10; 

    if (vendor[0])
        draw_text(x, y, vendor, GREEN_ON_BLACK, 0x00);
    else
        draw_text(x, y, "BIOS: Unknown vendor", RED_ON_BLACK, 0x00);
    y += 20;

    if (version[0]) {
        draw_text(x, y, version, CYAN_ON_BLACK, 0x00);
        y += 20;
    }

    if (date[0]) {
        draw_text(x, y, date, WHITE_ON_BLACK, 0x00);
        y += 20;
    }

    if (bios_type[0]) {
        char msg[64] = "BIOS type: ";
        int i = 11;
        for (int j = 0; bios_type[j]; j++) msg[i++] = bios_type[j];
        msg[i] = '\0';
        draw_text(x, y, msg, 0x1E, 0x00);
        y += 20;
    }

    if (vm_name[0]) {
        char msg[64] = "Environment: ";
        int i = 13;
        for (int j = 0; vm_name[j]; j++) msg[i++] = vm_name[j];
        msg[i] = '\0';
        draw_text(x, y, msg, 0x3F, 0x00);
        y += 20;
    }

    uint32_t eax, ebx, ecx, edx;

    cpuid(1, &eax, &ebx, &ecx, &edx);
    if (ecx & (1 << 5)) {
        draw_text(x, y, "Intel VT-x: Supported", 0x2F, 0x00);
    } else {
        draw_text(x, y, "Intel VT-x: Not supported", 0x4F, 0x00);
    }
    y += 20;
 // search for intel/amd virtualization
    cpuid(0x80000000, &eax, &ebx, &ecx, &edx);
    if (eax >= 0x80000001) {
        cpuid(0x80000001, &eax, &ebx, &ecx, &edx);
        if (ecx & (1 << 2)) {
            draw_text(x, y, "AMD-V: Supported", 0x2F, 0x00);
        } else {
            draw_text(x, y, "AMD-V: Not supported", 0x4F, 0x00);
        }
    } else {
        draw_text(x, y, "AMD-V: CPUID not available", 0x4F, 0x00);
    }
}