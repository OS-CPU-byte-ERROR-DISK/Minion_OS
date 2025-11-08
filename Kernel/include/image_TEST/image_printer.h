#include "image_data.h" 
 void X_X(const uint8_t *img_data, int img_w, int img_h, int screen_w, int screen_h) {
    for (int y = 0; y < screen_h; y++) {
        for (int x = 0; x < screen_w; x++) {
            int src_x = x * img_w / screen_w;
            int src_y = y * img_h / screen_h;
            VIDEO_MEMORY_PTR[y * screen_w + x] = img_data[src_y * img_w + src_x];
        }   }}
void PRINTER_IMAGE() {
     X_X(IMAGEX, img_w, img_h, SCREEN_WIDTH, SCREEN_HEIGHT);
    while (1) {
        MUSIC_X();
    }
}

// vi spiego. Praticamente nell'ora di arte ho fatto un disegno del cazzo e si. L'ho messo come foto.