DAP: ;contiene delle informazioni per leggere i settori dal disco. Molto easy
        db      0x10
        db      0x00
        dw      0x7F
        dw      0x0000
         dw      KERNEL_STARTER_POINT_MAIN_ >> 4
        dq      1
