#include <gtest/gtest.h>
#include <etl.hpp>



TEST(EtlVecAdd, Positive) {
    const float a[] = {1.0f, 2.0f, 3.0f};
    const float b[] = {4.0f, 5.0f, 6.0f};
    float y[3] = {0.0f, 0.0f, 0.0f};

    fcpp_vec_add_f32(a, b, y, 3);

    EXPECT_FLOAT_EQ(y[0], 5.0f);
    EXPECT_FLOAT_EQ(y[1], 7.0f);
    EXPECT_FLOAT_EQ(y[2], 9.0f);
}



TEST(EtlVecAdd, BoundaryZeroLength) {
    const float a[] = {1.0f};
    const float b[] = {2.0f};
    float y[1] = {0.0f};

    EXPECT_NO_THROW(fcpp_vec_add_f32(a, b, y, 0));
    EXPECT_FLOAT_EQ(y[0], 0.0f);
}



TEST(EtlVecAdd, Negative) {
    const float a[] = {-1.0f, 2.5f};
    const float b[] = {3.5f, -2.0f};
    float y[2] = {0.0f, 0.0f};

    fcpp_vec_add_f32(a, b, y, 2);

    EXPECT_FLOAT_EQ(y[0], 2.5f);
    EXPECT_FLOAT_EQ(y[1], 0.5f);
}



TEST(EtlVecSub, Positive) {
    const float a[] = {4.0f, 5.0f, 6.0f};
    const float b[] = {1.0f, 2.0f, 3.0f};
    float y[3] = {0.0f, 0.0f, 0.0f};

    fcpp_vec_sub_f32(a, b, y, 3);

    EXPECT_FLOAT_EQ(y[0], 3.0f);
    EXPECT_FLOAT_EQ(y[1], 3.0f);
    EXPECT_FLOAT_EQ(y[2], 3.0f);
}



TEST(EtlVecSub, BoundaryZeroLength) {
    const float a[] = {5.0f};
    const float b[] = {3.0f};
    float y[1] = {0.0f};

    EXPECT_NO_THROW(fcpp_vec_sub_f32(a, b, y, 0));
    EXPECT_FLOAT_EQ(y[0], 0.0f);
}
