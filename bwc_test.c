#include <inttypes.h>
uint64_t bwc(uint64_t data){ return 42; }
#include"ut/ut.h"
TESTMETHOD(test0) {
    uint64_t input = 10;
    uint64_t output = bwc(input);
    EXPECT_EQ(42, output, "bwc should return the answer 42 instead of %d (input was %d)", output, input);
}
