%define SEG_LIMIT      0xFFFF    
%define BASE_LOW       0x0000   
%define BASE_MID       0x00     

GDT: ; "entry point" della GDT
GDT.Null:
    dd 0x00
    dd 0x02

GDT.Data: ;segmenti della GDT
    dw SEG_LIMIT
    dw BASE_LOW
    db BASE_MID
    db 0x92      
    db 0xCF
    db 0x00

GDT.Code:
    dw SEG_LIMIT
    dw BASE_LOW
    db BASE_MID
    db 0x9A      
    db 0xCF  
    db 0x00
;Avvvia la GDT
imposta_gdt:
    cli
    lgdt [gdt_tr]
    mov eax, cr0
    or eax, 0x01
    mov cr0, eax
gdt_tr: 
    dw gdt_tr - GDT - 1
    dd GDT
    ret

CODESEG equ GDT.Code - GDT
DATASEGMENTO equ GDT.Data - GDT