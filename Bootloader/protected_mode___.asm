[BITS    32]
Protected_MODE_X:
mov     ax, DATASEGMENTO
 mov     ds, ax
 mov     es, ax
 mov     gs, ax
 mov     ss, ax
 mov     ebp, 0x90000
 mov     esp, ebp
 mov     fs, ax
 jmp     KERNEL_STARTER_POINT_MAIN_

