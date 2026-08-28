#include <gtest/gtest.h>
#include <net.hpp>



TEST(NetPredict, Positive) {
    EXPECT_EQ(predict_random_sample(), 0);
}



TEST(NetTrain, NoThrow) {
    EXPECT_NO_THROW(train_with_random_data());
}
