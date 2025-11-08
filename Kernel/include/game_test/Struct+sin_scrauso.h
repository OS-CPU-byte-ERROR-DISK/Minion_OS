#define LARG__   16
#define ALTEZZA__  16
#define PI          3.14159265f
#define ANGLE_RES   1024
float sin_table[ANGLE_RES];
float cos_table[ANGLE_RES];

int player_angle = ANGLE_RES / 4;
typedef struct {
    float x, y;
    float dirX, dirY;
    float planeX, planeY;
} Player;
Player player = {
    .x = 8.0f,
    .y = 6.0f,
    .dirX = 1.0f,
    .dirY = 0.0f,
    .planeX = 0.66f,
    .planeY = 0.0f
};
const int map[LARG__ * ALTEZZA__] = {
    [0 ...(LARG__ * ALTEZZA__ - 1)] = 0,
    [7 * LARG__ + 7]  = 1,
    [5 * LARG__ + 10] = 2
};
static inline float sin(int idx) {
    return sin_table[idx % ANGLE_RES];
}

static inline float cos(int idx) {
    return cos_table[idx % ANGLE_RES];
}

static inline float absf(float x) {
    return (x < 0) ? -x : x;
}