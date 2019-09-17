#include "colibra/matrix.h"
#include "doctest.h"

#include <complex>
#include <iostream>

using colibra::Matrix;

TEST_CASE("Construction")
{

    // clang-format off
    constexpr Matrix a = {{0 , 1  , 2},
                          {3 , 4  , 5},
                          {6 , 7  , 8},
                          {9 , 10 , 11}};
    // clang-format on

    CHECK(a.height() == 4);
    CHECK(a.width() == 3);

    size_t running = 0;
    for (const auto &row : a)
    {
        for (const auto &entry : row)
        {
            CHECK(running++ == entry);
        }
    }

    constexpr Matrix b = {{1.2, 2.3, 3.4, 5.7}};
    CHECK(b.width() == 4);

    CHECK(typeid(b[0][0]) == typeid(double));
}
