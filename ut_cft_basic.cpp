#include "cftrace.hpp"
#include <gtest/gtest.h>

using namespace std::string_view_literals;

TEST(cftrace, pack) {
    auto p = cftrace::pack(1, 2, 3, 4, 5);

    ASSERT_EQ(std::tuple_size_v<decltype(p)>, 5);
    ASSERT_EQ(std::get<0>(p), 1);
    ASSERT_EQ(std::get<1>(p), 2);
    ASSERT_EQ(std::get<2>(p), 3);
    ASSERT_EQ(std::get<3>(p), 4);
    ASSERT_EQ(std::get<4>(p), 5);

}

TEST(cftrace, size) {
    ASSERT_EQ(cftrace::size(1, 2, 3, 4, 5), 5 * sizeof(int));
    ASSERT_EQ(cftrace::size(1, 1.0, 1.0f), sizeof(int) + sizeof(double) + sizeof(float));
    ASSERT_EQ(cftrace::size(std::array<char, 10>{}, 1, 2), 10 * sizeof(char) + 2 * sizeof(int));
    ASSERT_EQ(cftrace::size(std::string_view("hello"sv), 1, 2), 5 + 2 * sizeof(int));
}

TEST(cftrace, serialize) {
    std::array<char, 128> buf;
    cftrace::serialize(std::span<char>(buf), 1, 2, 3, 4, 5);
    ASSERT_EQ(*reinterpret_cast<int*>(buf.data()), 1);
    ASSERT_EQ(*reinterpret_cast<int*>(buf.data() + sizeof(int)), 2);
    ASSERT_EQ(*reinterpret_cast<int*>(buf.data() + 2 * sizeof(int)), 3);
    ASSERT_EQ(*reinterpret_cast<int*>(buf.data() + 3 * sizeof(int)), 4);
    ASSERT_EQ(*reinterpret_cast<int*>(buf.data() + 4 * sizeof(int)), 5);

    cftrace::serialize(std::span<char>(buf), 1, 1.0, 1.0f);
    ASSERT_EQ(*reinterpret_cast<int*>(buf.data()), 1);
    ASSERT_EQ(*reinterpret_cast<double*>(buf.data() + sizeof(int)), 1.0);
    ASSERT_EQ(*reinterpret_cast<float*>(buf.data() + sizeof(int) + sizeof(double)), 1.0f);
}
