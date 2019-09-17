#ifndef COLIBRA_MATRIX_H
#define COLIBRA_MATRIX_H

#include <algorithm>
#include <array>
#include <cstdarg>
#include <iostream>
#include <type_traits>

namespace colibra {

template<size_t rows, size_t columns, typename T>
class Matrix
{
    using array_type = std::array<std::array<T, columns>, rows>;

  public:
    template<typename R, typename... P, size_t N>
    constexpr Matrix(const R (&nested_list)[N], const P (&... nested_others)[N])
        : m_array()
    {
        static_assert(N > 0, "Expect a Matrix to have at least 1 column.");
        for (int i = 0; i < N; ++i)
        {
            m_array[0][i] = std::move(nested_list[i]);
        }
        if constexpr (sizeof...(P) != 0)
        {
            assign_recursive(1, nested_others...);
        }
    }

    constexpr size_t width() const
    {
        return columns;
    }
    constexpr size_t height() const
    {
        return rows;
    }

    constexpr std::array<T, columns> operator[](const size_t p)
    {
        return m_array[p];
    }

    constexpr std::array<T, columns> const operator[](const size_t p) const
    {
        return m_array[p];
    }

    auto begin() noexcept -> decltype(std::declval<array_type>().begin())
    {
        return m_array.begin();
    }

    auto cbegin() noexcept -> decltype(std::declval<array_type>().cbegin())
    {
        return m_array.cbegin();
    }

    auto end() noexcept -> decltype(std::declval<array_type>().end())
    {
        return m_array.end();
    }

    auto cend() noexcept -> decltype(std::declval<array_type>().cend())
    {
        return m_array.cend();
    }

  private:
    array_type m_array;

    template<typename P, size_t N>
    constexpr void inline assign_array(size_t row, const P (&nested_list)[N])
    {
        for (int i = 0; i < N; ++i)
        {
            m_array[row][i] = std::move(nested_list[i]);
        }
    }

    template<typename R, typename... P, size_t N>
    constexpr void assign_recursive(size_t row,
                                    const R (&nested_list)[N],
                                    const P (&... nested_others)[N])
    {
        assign_array<R, N>(row, nested_list);
        assign_recursive(++row, nested_others...);
    }

    template<typename R, size_t N>
    constexpr void assign_recursive(size_t row, const R (&nested_list)[N])
    {
        assign_array<R, N>(row, nested_list);
    }
};

template<typename R, typename... P, size_t N>
Matrix(const R (&nested_list)[N], const P (&... nested_others)[N])
    ->Matrix<1 + sizeof...(P), N, R>;

} // namespace colibra

#endif
