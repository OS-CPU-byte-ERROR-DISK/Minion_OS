global rtc_ind
global init_pic
init_pic:
    mov al, 0xff
    out 0x21, al
    mov al, 0x11
    mov al, 0x20
    mov al, 0x28
    mov al, 0x04
    ret
rtc_ind:
    pusha
    out 0xA0, al 
    out 0x20, al 
    popa
    mov eax, [esp + 4]
    lidt [eax]
    ret
