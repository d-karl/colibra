#ifndef COLIBRA_DETAILS_VECTOR_HPP
#define COLIBRA_DETAILS_VECTOR_HPP

#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
#include <numeric>
#include <ostream>
#include <type_traits>

namespace colibra {
template<size_t l, typename T>
class Vector;

namespace details {

template<typename T, size_t... Idx>
constexpr auto sum(const Vector<sizeof...(Idx), T> vec,
                   std::index_sequence<Idx...>)
{
    return (... + vec[Idx]);
}

template<size_t l, typename T>
class Vector
{
    using array_type = typename std::array<T, l>;

  public:
    template<typename... P>
    explicit constexpr Vector(T const &&val1, P const &&... vals)
        : m_array {val1, vals...}
    {
    }

    constexpr Vector()
        : m_array()
    {
        static_assert(l > 0, "Can not declare Vectors with 0 elements");
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

    [[nodiscard]] constexpr size_t rank() const
    {
        return l;
    }

    [[nodiscard]] constexpr T &at(const size_t p)
    {
        return m_array.at(p);
    }

    [[nodiscard]] constexpr T const &at(const size_t p) const
    {
        return m_array.at(p);
    }

    [[nodiscard]] constexpr T &operator[](const size_t p)
    {
        return m_array[p];
    }

    [[nodiscard]] constexpr T const &operator[](const size_t p) const
    {
        return m_array[p];
    }

    template<class S, typename R = std::common_type_t<T, S>>
    [[nodiscard]] constexpr R operator*(const Vector<l, S> &other) const
    {
        return sum(apply_each(other,
                              std::multiplies<R>(),
                              std::make_index_sequence<l> {}),
                   std::make_index_sequence<l> {});
    }

    template<class S, typename R = std::common_type_t<T, S>>
    [[nodiscard]] constexpr auto operator*(const S &scalar) const
    {

        return apply_each(
            scalar, std::multiplies<R>(), std::make_index_sequence<l> {});
    }

    template<class S, typename R = std::common_type_t<T, S>>
    [[nodiscard]] constexpr auto operator+(const Vector<l, S> &other) const
    {
        return apply_each(
            other, std::plus<R>(), std::make_index_sequence<l> {});
    }

    template<class S, typename R = std::common_type_t<T, S>>
    [[nodiscard]] constexpr auto operator-(const Vector<l, S> &other) const
    {
        return apply_each(
            other, std::minus<R>(), std::make_index_sequence<l> {});
    }

    [[nodiscard]] constexpr auto operator-()
    {
        return apply_each(std::negate<T>(), std::make_index_sequence<l> {});
    }

    [[nodiscard]] constexpr bool operator==(Vector<l, T> const &other) const
    {
        return m_array == other.m_array;
    }

    [[nodiscard]] constexpr bool operator!=(Vector<l, T> const &other) const
    {
        return m_array != other.m_array;
    }

    [[nodiscard]] constexpr double norm() const
    {
        double norm = 0;
        for (const auto &i : m_array)
        {
            norm += i * i;
        }
        return sqrt(norm);
    }

    [[nodiscard]] constexpr const T *data() const
    {
        return m_array.data();
    }

    friend std::ostream &operator<<(std::ostream &os, const Vector<l, T> &vec)
    {
        // lengt l = 0 is impossible due to static assert.
        if constexpr (l > 1)
        {
            os << "{ " << vec.m_array[0];
            for (auto it = vec.m_array.cbegin() + 1; it != vec.m_array.cend();
                 it++)
            {
                os << ", " << *it;
            }

            os << " }";
        }
        else
        {
            os << "{ " << vec.m_array[0] << " ";
        }

        return os;
    }

  private:
    array_type m_array;

    template<typename S, class Op, size_t... Idx>
    constexpr auto
    apply_each(const S &fac, const Op &op, std::index_sequence<Idx...>) const
    {
        return colibra::Vector<l, std::common_type_t<T, S>> {
            op(m_array[Idx], fac)...};
    }

    template<typename O, class Op, size_t... Idx>
    constexpr auto apply_each(const Vector<l, O> &other,
                              const Op &          op,
                              std::index_sequence<Idx...>) const
    {
        return colibra::Vector<l, std::common_type_t<T, O>> {
            op(m_array[Idx], other[Idx])...};
    }

    template<class Op, size_t... Idx>
    constexpr auto apply_each(const Op &op, std::index_sequence<Idx...>) const
    {
        return colibra::Vector {op(m_array[Idx])...};
    }
};

} // namespace details

} // namespace colibra

#endif
