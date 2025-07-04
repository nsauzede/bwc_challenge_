#include "libsnake.c"
/******************************************************************************/
#include"ut/ut.h"
TESTMETHOD(test_curly_snake_dies) {
    state_t state = {
        .f_y=3, .f_x=5,
        .len=23,
        .h_y=7, .h_x=5,
      .dir=0x5bfff9555bf
    };
    uint64_t data = state_to_data(state);
    for (int n = 0; n < 6; n++) {
        data = evolve(data);
        //draw(data);
    }
    state = data_to_state(data);
    EXPECT_EQ(1, state.dead);
}
TESTMETHOD(test_grows_one) {
    state_t state = {
        .dead = 0,
        .f_y = 3, .f_x = 4,
        .len = 2,
        .h_y = 3, .h_x = 3,
        .dir = 0x00000000005
    };
    uint64_t data = state_to_data(state);
    data = evolve(data);
    state = data_to_state(data);
    EXPECT_EQ(3, state.len);
}
TESTMETHOD(test_is_dead) {
    state_t state = {
        .dead = 0,
        .f_y = 3, .f_x = 5,
        .len = 2,
        .h_y = 3, .h_x = 3,
        .dir = 0x00000000007
    };
    uint64_t data = state_to_data(state);
    data = evolve(data);
    state = data_to_state(data);
    EXPECT_EQ(1, state.dead);
}
TESTMETHOD(test_is_alive) {
    state_t state = {
        .dead = 0,
        .f_y = 3, .f_x = 5,
        .len = 2,
        .h_y = 3, .h_x = 3,
        .dir = 0x00000000005
    };
    uint64_t data = state_to_data(state);
    data = evolve(data);
    state = data_to_state(data);
    EXPECT_EQ(0, state.dead);
}
TESTMETHOD(test_data_size_is_eight_bytes) {
    EXPECT_EQ(8, sizeof(state_t));
}
