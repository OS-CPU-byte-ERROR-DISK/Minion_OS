#include "Struct+sin_scrauso.h"

void ROBO() {
    for (int i = 0; i < ANGLE_RES; i++) {
        float a = 2.0f * PI * i / ANGLE_RES;

        sin_table[i] = a - (a * a * a) / 6.0f + (a * a * a * a * a) / 120.0f;
        cos_table[i] = 1.0f - (a * a) / 2.0f + (a * a * a * a) / 24.0f;
 }
}
void OMINO_FR(int dir) {
    const int rot_step = 8;
    player_angle = (player_angle + dir * rot_step + ANGLE_RES) % ANGLE_RES;
    player.dirX   = cos(player_angle);
    player.dirY   = sin(player_angle);
    player.planeX = cos(player_angle + ANGLE_RES / 4) * 0.66f;
    player.planeY = sin(player_angle + ANGLE_RES / 4) * 0.66f;
}

void KBXY(uint8_t sc) { //keyboard movement handler. 
    const float speed = 0.1f;

    switch (sc) {
        case 0x11:  // W
            player.x += player.dirX * speed;
            player.y += player.dirY * speed;
            break;

        case 0x1F:  // S
            player.x -= player.dirX * speed;
            player.y -= player.dirY * speed;
            break;

        case 0x1E:  // A
            OMINO_FR(-1);
            break;

        case 0x20:  // D
            OMINO_FR(+1);
            break;
   }
}
void DISEGNAMENTO() {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        float camX = 2.0f * x / (float)SCREEN_WIDTH - 1.0f;
        float rayDirX = player.dirX + player.planeX * camX;
        float rayDirY = player.dirY + player.planeY * camX;
        int mapX = (int)player.x;
        int mapY = (int)player.y;
        float deltaDistX = (rayDirX == 0) ? 1e30f : 1.0f / rayDirX;
        float deltaDistY = (rayDirY == 0) ? 1e30f : 1.0f / rayDirY;
        int stepX = (rayDirX < 0) ? -1 : 1;
        int stepY = (rayDirY < 0) ? -1 : 1;
        float sideDistX = (rayDirX < 0)
            ? (player.x - mapX) * absf(deltaDistX)
            : (mapX + 1.0f - player.x) * absf(deltaDistX);

        float sideDistY = (rayDirY < 0)
            ? (player.y - mapY) * absf(deltaDistY)
            : (mapY + 1.0f - player.y) * absf(deltaDistY);
        int hit = 0;
        int side = 0;
        while (!hit) {
            if (sideDistX < sideDistY) {
                sideDistX += absf(deltaDistX);
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += absf(deltaDistY);
                mapY += stepY;
                side = 1;
            }

            if (map[mapY * LARG__ + mapX] > 0)
                hit = 1;
        }
        float perpDist = (side == 0)
        ? (sideDistX - absf(deltaDistX))
         : (sideDistY - absf(deltaDistY));
        int lineHeight = (int)(SCREEN_HEIGHT / perpDist);
        int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
        int drawEnd   =  lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawStart < 0) drawStart = 0;
        if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;
        uint32_t wallColor = 0x02; 
        if (side) wallColor /= 2;
        for (int y = 0; y < SCREEN_HEIGHT; y++) {
            if (y < drawStart)
                SET_DRAW_PIXEL(x, y, 0x0B); 
            else if (y <= drawEnd)
                SET_DRAW_PIXEL(x, y, wallColor);
            else
                SET_DRAW_PIXEL(x, y, 0x0A); 
        }
    }
}

void DOOM_() {
    ROBO();
    while (1) {
        uint8_t sc = read_keyboard_scancode();
        KBXY(sc);
        DISEGNAMENTO();
    }
}
