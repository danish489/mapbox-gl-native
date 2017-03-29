#include <mbgl/test/util.hpp>

#include <mbgl/util/size.hpp>

#include <limits>

using namespace mbgl;

TEST(Size, EmptySize) {
    Size size;
    ASSERT_FALSE(size);

    // Some platforms provides default width/height assigned as -1.
    size = { static_cast<uint32_t>(-1), static_cast<uint32_t>(-1) };
    ASSERT_FALSE(size);

    size.width = size.height = 1u;
    ASSERT_TRUE(size);
}
