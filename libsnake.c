#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma pack(1)
typedef struct {
    uint64_t
        dead:1,
        f_y:3, f_x:3,
        len:5,
        h_y:3, h_x:3,
        dir:46  // 23 dirs, len <= 23
    ;
} state_t;
typedef enum {N,E,S,W} dir_t;
#define MAX_LEN 23
#define WW 8
#define HH 8
#pragma pack()
state_t data_to_state(uint64_t data) {
    union { uint64_t data; state_t state; } u = { data };
    return u.state;
}
uint64_t state_to_data(state_t state) {
    union { state_t state; uint64_t data; } u = { state };
    return u.data;
}
void scan_(char screen[WW*HH], state_t state) {
    memset(screen, '.', WW*HH);
    screen[state.f_x + WW*state.f_y] = 'F';
    screen[state.h_x + WW*state.h_y] = 'H';
    uint64_t dir_ = state.dir >> 2;
    int d_x = state.h_x, d_y = state.h_y;
    for (int i = 1; i < state.len; i++) {
        int dir = dir_ & 0x3;
        if (dir == S) {
            if (d_y <= 0) d_y = HH;
            d_y--;
        } else if (dir == W) {
            if (d_x >= (WW-1)) d_x = -1;
            d_x++;
        } else if (dir == N) {
            if (d_y >= (HH-1)) d_y = -1;
            d_y++;
        } else if (dir == E) {
            if (d_x <= 0) d_x = WW;
            d_x--;
        }
        //printf("i=%d dir=%d d_x=%d d_y=%d\n", i, dir, d_x, d_y);
        screen[d_x + WW*d_y] = 'a' + i - 1;
        dir_ >>= 2;
    }
}
void draw_(char screen[WW*HH], state_t state) {
    scan_(screen, state);
    printf("%s: len=%d dead=%d\n", __func__, state.len, state.dead);
    for (int y = 0; y < HH; y++) {
        for (int x = 0; x < WW; x++) {
            printf("%c", screen[WW * y + x]);
        }
        printf("\n");
    }
}
uint64_t evolve(uint64_t data) {
    static int is_init = 0;
    if (!is_init) {
        srand(0);
        is_init = 1;
    }
    state_t state = data_to_state(data);
    while (1) {
        if ((state.h_x != state.f_x) || (state.h_y != state.f_y)) {
            break;
        }
        state.f_x = rand() % WW;
        state.f_y = rand() % HH;
    }
    char screen[WW*HH];
    scan_(screen, state);
    int d_x = state.h_x, d_y = state.h_y;
    uint64_t dir_ = state.dir;
    int dir = dir_ & 0x3;
    if (dir == N) {
        if (d_y <= 0) d_y = HH;
        d_y--;
    } else if (dir == E) {
        if (d_x >= (WW-1)) d_x = -1;
        d_x++;
    } else if (dir == S) {
        if (d_y >= (HH-1)) d_y = -1;
        d_y++;
    } else if (dir == W) {
        if (d_x <= 0) d_x = WW;
        d_x--;
    }
    if ((d_x == state.f_x) && (d_y == state.f_y)) {
        if (state.len < MAX_LEN) {
            state.len++;
        }
    } else if (screen[d_y * WW + d_x] != '.') {
        state.dead = 1;
    }
    state.h_x = d_x; state.h_y = d_y;
    state.dir = (dir_ << 2) | dir;
    data = state_to_data(state);
    return data;
}
void draw(uint64_t data) {
    state_t state = data_to_state(data);
    char screen[WW*HH];
    draw_(screen, state);
}
