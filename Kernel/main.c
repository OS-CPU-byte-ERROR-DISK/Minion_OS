#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "include/os_killer/SPD.h"
#include "include/os_killer/Write_on_disk.h"
#include "include/os_killer/BURN_EVERYTHING.h"
#include "Ui.h"
#include "include/network/MAC.h"
#include "include/Shell/shell.h"
#include "include/cmos.h"
#include "include/Hardware__Cazzo_ne_so/HardwarE_.h"
#include "include/network/Network_card_found_x.h"
#include "include/Hardware__Cazzo_ne_so/disk.h"
#include "include/FILESYS/filesystem.h"
#include "include/reboot.h"
#include "include/sound/sound_payload.h"
#include "include/image_TEST/image_printer.h"
#include "include/IDT_2/idt.h"
#include "include/XOR__/xor.h"
#include "include/game_test/Doom.h"
#include "include/printf_dei_poveri/print.h"
extern void init_pic();
void init_interrupts() {
    init_pic();      
    IDT_STARTER_POINT();   
    init_idt_irq();  
}
void main(){
   init_interrupts(); 
  // DOOM_(); doom suicida.
  // XT5(); roba stupida che ho tolto. Ma forse la rimetto.
  ALLUCINAZIONE();
}