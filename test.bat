@echo off
setlocal enabledelayedexpansion
color 2
del /Q END\*.o END\*.bin END\*.elf END\*.o END\*.elf adof.img disk.raw 2>nul
nasm -fbin Bootloader\boot.asm -o END\boot.bin || goto :errore
set CFLAGS=-ffreestanding -m32 -g -fno-pic -fno-pie -no-pie
i686-elf-gcc %CFLAGS% -c Kernel\main.c -o END\kernel.o || goto :errore
nasm -felf Kernel\Connettatore.asm -o END\start.o || goto :errore
nasm -felf IDT.asm -o END/idt.o || goto :errore
nasm -felf IDT.asm -o END\idt.o || goto :errore
i686-elf-ld -m elf_i386 -Ttext 0x7E00 -o END\fullkernel.elf ^
  END\start.o END\kernel.o END\idt.o || goto :errore

i686-elf-objcopy -O binary END\fullkernel.elf END\fullkernel.bin || goto :errore
copy /b END\boot.bin + END\fullkernel.bin Minion_OS.img >nul || goto :errore
REM Avvia QEMU
qemu-system-i386 Minion_OS.img -audiodev sdl,id=speaker -machine pcspk-audiodev=speaker -net nic,model=rtl8139 -net user
goto :fine
:errore
color 4
echo.
echo EVVIVA è ANDATO A PUTTANE. SII
pause
exit /b 1
:fine
endlocal
