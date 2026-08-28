#include <gtest/gtest.h>
#include <tuple>
#include <vector>
#include <cpptest.hpp>



TEST(CpptestPerson, PositiveGreet) {
    const Person alice("Alice", 25);
    EXPECT_EQ(alice.greet(), "Hello, I'm Alice");
    EXPECT_EQ(alice.name, "Alice");
    EXPECT_EQ(alice.age, 25);
}



TEST(CpptestPerson, BoundaryEmptyName) {
    const Person nobody("", 0);
    EXPECT_EQ(nobody.greet(), "Hello, I'm ");
}



TEST(CpptestTestSum, BoundaryEmpty) {
    const std::vector<int> empty;
    EXPECT_EQ(test_sum(empty), 0);
}



TEST(CpptestTestSum, BoundarySingle) {
    const std::vector<int> one = {42};
    EXPECT_EQ(test_sum(one), 42);
}



TEST(CpptestTestSum, Negative) {
    const std::vector<int> neg = {-1, -2, -3};
    EXPECT_EQ(test_sum(neg), -6);
}



TEST(CpptestColor, PositiveComponents) {
    const Color<int> color(255, 0, 0);
    EXPECT_EQ(color.components(), std::make_tuple(255, 0, 0));
}



TEST(CpptestColor, SetComponents) {
    Color<int> color;
    color.set(10, 20, 30);
    EXPECT_EQ(color.components(), std::make_tuple(10, 20, 30));
}



TEST(CpptestColor, BoundaryDefault) {
    const Color<int> color;
    EXPECT_EQ(color.components(), std::make_tuple(0, 0, 0));
}



TEST(CpptestSmoke, Hello) {
    EXPECT_NO_THROW(test_hello());
}



TEST(CpptestSmoke, Eigen) {
    EXPECT_NO_THROW(test_eigen());
}



TEST(CpptestSmoke, Zlib) {
    EXPECT_NO_THROW(test_cpp_zlib());
}
