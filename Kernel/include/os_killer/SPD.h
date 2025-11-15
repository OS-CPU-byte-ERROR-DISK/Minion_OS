#define SMBUS_BASE_ADDR 0x8000A000
#define SMBUS_CTRL_REG  (SMBUS_BASE_ADDR + 0x02)
#define SMBUS_STATUS_REG (SMBUS_BASE_ADDR + 0x00)
#define SMBUS_DATA_REG   (SMBUS_BASE_ADDR + 0x04)
#define SMBUS_CMD_REG    (SMBUS_BASE_ADDR + 0x03)
#define SPD_I2C_ADDR     0xA2
void smbus_write_byte(uint8_t offset, uint8_t value) {
    *(volatile uint8_t *)SMBUS_CMD_REG = offset;
    *(volatile uint8_t *)SMBUS_DATA_REG = value;
    *(volatile uint8_t *)(SMBUS_BASE_ADDR + 0x04) = SPD_I2C_ADDR;
    *(volatile uint8_t *)SMBUS_CTRL_REG = 0x48;
    while (*(volatile uint8_t *)SMBUS_STATUS_REG & 0x01);
    if (*(volatile uint8_t *)SMBUS_STATUS_REG & 0x02) {
        shutdown();
    }
}
