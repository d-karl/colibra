#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "colibra/vector.h"
#include "doctest.h"

#include <complex>
#include <iostream>

using namespace colibra;
using doctest::Approx;

TEST_CASE("Vector")
{
    constexpr auto b_1 {2.5};
    constexpr auto b_2 {3.1};
    constexpr auto b_3 {4.2};

    constexpr Vector b {b_1, b_2, b_3};

    constexpr Vector vec_i {2, 3, 4};

    SUBCASE("Comparisons")
    {
        CHECK(b.rank() == 3);
        CHECK(b[0] == Approx(b_1));
        CHECK(b[1] == Approx(b_2));
        CHECK(b[2] == Approx(b_3));
        CHECK(typeid(vec_i[0]) == typeid(int));
        Vector b2 = b;
        CHECK(b == b2);

        b2[2] = 2.5;
        CHECK(b != b2);
    }

    SUBCASE("Null vector")
    {
        constexpr Vector<3, double> null_vector_3d {};
        CHECK(null_vector_3d.rank() == 3);
        for (int n3d = 0; n3d < 3; ++n3d)
        {
            CHECK(null_vector_3d[n3d] == Approx(0.0));
        }
    }

    SUBCASE("Complex")
    {
        constexpr Vector c1 {std::complex(10.2, 4.2), std::complex(2.0, 42.0)};
        const auto c2 = c1 + c1; // operator+ of std::complex is not constexpr
        const auto c3 = -c1;
        for (int i = 0; i < c1.rank(); ++i)
        {
            CHECK(c2[i].real() == Approx(c1[i].real() * 2));
            CHECK(c3[i].real() == Approx((-c1[i]).real()));
        }
    }

    SUBCASE("Norm")
    {
        constexpr Vector<2, double> n {1.0};
        CHECK(n[1] == Approx(0.0));
        CHECK(n.norm() == Approx(1.0));
    }

    SUBCASE("Multiplication")
    {
        constexpr auto b2(b * static_cast<int>(2));

        SUBCASE("Vec(int) * 2")
        {

            CHECK(typeid(b2) == typeid(b));

            for (int i = 0; i < b2.rank(); ++i)
            {
                CHECK(b2[i] == b[i] * 2.0);
            }
            constexpr auto i2(vec_i * 3);
            CHECK(typeid(i2) == typeid(vec_i));
            for (int i = 0; i < i2.rank(); ++i)
            {
                CHECK(i2[i] == vec_i[i] * 3);
            }
            constexpr auto i3 = vec_i * 3.1;
            CHECK(typeid(i3[0]) == typeid(double));
            for (int i = 0; i < i3.rank(); ++i)
            {
                CHECK(i3[i] == vec_i[i] * 3.1);
            }
        }

        SUBCASE("Vec * Vec")
        {
            constexpr auto b3(b2 * b);
            double         scalar = 0;
            for (int j = 0; j < b.rank(); ++j)
            {
                scalar += b[j] * b2[j];
            }

            std::cout << b3 << std::endl;
        }

        SUBCASE("Vec addition")
        {
            constexpr auto b3 = b2 + b;
            for (int j = 0; j < b.rank(); ++j)
            {
                CHECK(b3[j] == (b[j] + b2[j]));
            }
        }

        SUBCASE("Vec subtraction")
        {
            constexpr auto b3(b - b2);
            for (int j = 0; j < b.rank(); ++j)
            {
                CHECK(b3[j] == (b[j] - b2[j]));
            }
        }

        SUBCASE("Vec negation")
        {
            constexpr auto b3 = -b;
            for (int j = 0; j < b.rank(); ++j)
            {
                CHECK(b3[j] == (-b[j]));
            }
        }

        // constexpr auto breaks {b * "hello"};
    }
}

// TEST_CASE("Sparse Vector creation")
// {
//     Sparse_Vector<100> s {std::pair<size_t, double>(42, 126.0),
//                           std::pair<size_t, double>(0, 1.0)};

//     CHECK(s[0] == Approx(1.0));
//     CHECK(s[2] == 0.0);
//     CHECK(s[42] == Approx(126.0));

//     Sparse_Vector<50, float> f {std::pair<size_t, float>(5, 41.0)};
//     CHECK(f[5] == Approx(41.0));
//     auto in_sv = f[5];
//     bool same  = typeid(in_sv) == typeid(float);
//     CHECK(same);
// }
