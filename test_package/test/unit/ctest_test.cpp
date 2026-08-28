#include <gtest/gtest.h>
#include <ctest.h>



TEST(CtestSmoke, Compiler) {
    EXPECT_NO_THROW(test_c_compiler());
}



TEST(CtestSmoke, Zlib) {
    EXPECT_NO_THROW(test_c_zlib());
}



TEST(CtestSmoke, Pcre) {
    EXPECT_NO_THROW(test_c_pcre());
}
