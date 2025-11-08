#define MAX_FILES 16
#define MAX_FILENAME_LEN 16
#define MAX_FILE_SIZE 480

typedef struct {
    bool used;
    char name[MAX_FILENAME_LEN];
    uint8_t content[MAX_FILE_SIZE];
    int size;
    uint8_t sec, min, hour, day, month, year;
} file_t;

file_t filesystem[MAX_FILES];

uint8_t ram_disk_sector[512];
bool ram_disk_initialized = false;

void cmos_tiex(uint8_t* sec, uint8_t* min, uint8_t* hour,
                    uint8_t* day, uint8_t* month, uint8_t* year) {
    *sec   = bcd_to_bin(cmos_read(0x00));
    *min   = bcd_to_bin(cmos_read(0x02));
    *hour  = bcd_to_bin(cmos_read(0x04));
    *day   = bcd_to_bin(cmos_read(0x07));
    *month = bcd_to_bin(cmos_read(0x08));
    *year  = bcd_to_bin(cmos_read(0x09));
}