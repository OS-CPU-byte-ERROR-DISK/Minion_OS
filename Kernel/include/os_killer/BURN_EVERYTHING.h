void HATE(){ 
    test_write();
    smbus_write_byte(0x10, 0xFF);
    CMOS_INCULOS();
    shutdown();
    //"shutdown,cmos etc..." are loaded from the main.c using #include
}