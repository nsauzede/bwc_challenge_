#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#pragma pack(1)
typedef struct {
    uint64_t
        dead:1,
        running:1,
        f_y:3, f_x:3,
        len:6,
        h_y:3, h_x:3,
        dir:44  // 22 dirs, len <=23
    ;
} state_t;
typedef enum {N,E,S,W} dir_t;
#define WW 8
#define HH 8
#pragma pack()
uint64_t bwc(uint64_t data) {
return 42;
}
state_t data_to_state(uint64_t data) {
    union { uint64_t data; state_t state; } u = { data };
    return u.state;
}
uint64_t state_to_data(state_t state) {
    union { state_t state; uint64_t data; } u = { state };
    return u.data;
}
void draw(uint64_t data) {
    state_t state = data_to_state(data);
    char screen[WW*HH];
    memset(screen, '.', WW*HH);
    screen[state.f_x + WW*state.f_y] = 'F';
    screen[state.h_x + WW*state.h_y] = 'H';
    uint64_t dir_ = state.dir;
    int len = state.len;
    int d_x = state.h_x, d_y = state.h_y;
    for (int i = 1; i < len; i++) {
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
        if (screen[d_x + WW*d_y] != '.') {
            state.dead = 1;
            //printf("DEAD!!!!!\n");
        }
        screen[d_x + WW*d_y] = 'a' + i - 1;
        dir_ >>= 2;
    }
    int s = sizeof(state);
    //EXPECT_EQ(4, s, "sizeof(state_t)=%d", s);
#if 1
    printf("sz=%d, ", s);
    printf("h_x=%d, h_y=%d, len=%d, dir=%d DEAD=%d", state.h_x, state.h_y, len, state.dir, state.dead);
    printf("\n");
#endif
    for (int y = 0; y < HH; y++) {
        for (int x = 0; x < WW; x++) {
            printf("%c", screen[WW * y + x]);
        }
        printf("\n");
    }
}
uint64_t evolve(uint64_t data) {
    state_t state = data_to_state(data);
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
        if (state.len < 23) {
            state.len++;
        }
        //state.f_x = 0;
        //state.f_y = 0;
        state.f_x = (state.f_x + 1) % 8;
    }
    state.h_x = d_x; state.h_y = d_y;
    state.dir = (dir_ << 2) | dir;
    data = state_to_data(state);
    return data;
}
#include"ut/ut.h"
TESTMETHOD(test2) {
    state_t state = {
        .f_y=3, .f_x=5,
        .len=2,
        .h_y=3, .h_x=3,
        .dir=0x00000000005
    };
    uint64_t data = state_to_data(state);
    printf("\n");
    for (int n = 0; n < 8; n++) {
        draw(data);
        data = evolve(data);
    }
}
TESTMETHOD(test1) {
    state_t state = {
        .f_y=3, .f_x=5,
        .len=23,
        .h_y=7, .h_x=5,
      .dir=0x56fffe5556f
    };
    uint64_t data = state_to_data(state);
    printf("\n");
    for (int n = 0; n < 7; n++) {
        draw(data);
        data = evolve(data);
    }
}
TESTMETHOD(test0) {
    uint64_t input = 10;
    uint64_t output = bwc(input);
    EXPECT_EQ(42, output, "bwc should return the answer 42 instead of %d (input was %d)", output, input);
}
