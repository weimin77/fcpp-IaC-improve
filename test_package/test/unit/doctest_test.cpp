#include <gtest/gtest.h>
#include <type_traits>
#include <vector>
#include <doctest.hpp>



TEST(DoctestTransform, PositiveSquare) {
    std::vector<int> numbers = {1, 2, 3, 4, 5};

    inplace_transform(numbers, [](int& n) { n *= n; });

    EXPECT_EQ(numbers, (std::vector<int>{1, 4, 9, 16, 25}));
}



TEST(DoctestTransform, BoundaryEmpty) {
    std::vector<int> empty;

    inplace_transform(empty, [](int& n) { n += 1; });

    EXPECT_TRUE(empty.empty());
}



TEST(DoctestClass, Hierarchy) {
    static_assert(std::is_base_of_v<Base, SubBase1>);
    static_assert(std::is_base_of_v<Base, SubBase2>);
    static_assert(std::is_base_of_v<SubBase2, SubSubBase>);
    SUCCEED();
}
