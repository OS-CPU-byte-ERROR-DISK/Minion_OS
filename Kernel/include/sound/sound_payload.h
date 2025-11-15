#include "sound_payload_define.h"
#include "melodia.h"
void play_beep(uint32_t frequency, uint32_t duration_ms) {
    if (frequency == 0) {
        for (volatile uint32_t i = 0; i < duration_ms * 10000; i++) {
            __asm__ volatile ("nop");
        }
        return;
    }

    uint16_t divisor = (uint16_t)(PIT_BASE_FREQ / frequency);
    outb(PIT_CONTROL_PORT, 0xB6);
    outb(PIT_CHANNEL2_PORT, divisor & 0xFF);
    outb(PIT_CHANNEL2_PORT, (divisor >> 8) & 0xFF);

    uint8_t speaker_ctrl = inb(SPEAKER_CTRL_PORT);
    if ((speaker_ctrl & 3) != 3) {
        outb(SPEAKER_CTRL_PORT, speaker_ctrl | 3);
    }
    for (volatile uint32_t i = 0; i < duration_ms * 100000; i++) {
        __asm__ volatile ("nop");
    }
    speaker_ctrl = inb(SPEAKER_CTRL_PORT) & 0xFC;
    outb(SPEAKER_CTRL_PORT, speaker_ctrl);
}

void play_melody(const Note* melody, int length) {
    for (int i = 0; i < length; i++) {
        play_beep(melody[i].frequency, melody[i].duration_ms);
        for (volatile uint32_t j = 0; j < 50000; j++) {
            __asm__ volatile ("nop");
        }
    }
}

const int melody_len = sizeof(SOUND_M) / sizeof(Note);

void MUSIC_X() {
    play_melody(SOUND_M, melody_len);
    shutdown();

}