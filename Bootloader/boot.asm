[ORG 0x7C00]
%define VESA_VIDEO_MODE  0x4103     
%define DAP_ST_TR 127        
%define KERNEL_STARTER_POINT_MAIN_ 0x7E00   
Main: ;contiene tutto.
    ModeInfoBuffer 
    times 256 db 0
    mov ax, 0x4F02
    mov bx, VESA_VIDEO_MODE
    int 0x10 
    mov ax, 0x4F01
    mov cx, VESA_VIDEO_MODE
    lea di, ModeInfoBuffer
    int 0x10
    mov eax, [ModeInfoBuffer + 0x0C]
    mov [VIDEO_MEM_PTR], eax
    xor ax, ax
    call .disk_str  ;roba del disco.
    call imposta_gdt ; imposta/avvia la GDT
    jmp CODESEG:Protected_MODE_X

.disk_str:
    mov ah, 0x42
    push cx
    mov si, DAP
    int 0x13
    add word  [DAP + 6], 0xFE0
    add dword [DAP + 8], DAP_ST_TR
    pop cx
    loop .disk_str
    ret
%include "Bootloader/gdt.asm"
%include "Bootloader/protected_mode___.asm"
%include "Bootloader/DAP.asm"
VIDEO_MEM_PTR dd 0              
times 510 - ($ - $$) db 0x00
dw 0xAA55
