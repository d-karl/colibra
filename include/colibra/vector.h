#ifndef COLIBRA_VECTOR_H
#define COLIBRA_VECTOR_H

#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
#include <numeric>
#include <ostream>
#include <type_traits>

namespace colibra {

/**
 * @brief: A Vector class that is templated in its size and data type.
 *
 * This is the foundation of Vector operations in colibra. Since distinct types
 * are generated per size, this allows the compiler to warn you when using
 * incompatible types in operations.
 *
 * @tparam l The size of the vector.
 * @tparam T The data type of this vector.
 * @tparam P Parameter pack, all of these must be equal to T. Implementation
 * detail that allows constructor magic to auto determine the size on creation.
 */
template<size_t l, typename T>
class Vector
{
    using array_type = typename std::array<T, l>;

  public:
    /**
     * @brief: Create a new Vector given an arbitrary number of initial values.
     */
    template<typename... P>
    explicit constexpr Vector(T const val1, P const... vals)
        : m_array {std::move(val1), std::move(vals)...}
    {
    }

    /**
     * @brief: Create a new, empty vector. You must manually define the size and
     * type the Vector holds if you use this function.
     */
    constexpr Vector()
        : m_array()
    {
        static_assert(l > 0, "Can not declare Vectors with 0 elements");
    }

    /***************
     *  iterators  *
     ***************/
    inline auto begin() noexcept -> decltype(std::declval<array_type>().begin())
    {
        return m_array.begin();
    }

    inline auto cbegin() noexcept
        -> decltype(std::declval<array_type>().cbegin())
    {
        return m_array.cbegin();
    }

    inline auto end() noexcept -> decltype(std::declval<array_type>().end())
    {
        return m_array.end();
    }

    inline auto cend() noexcept -> decltype(std::declval<array_type>().cend())
    {
        return m_array.cend();
    }

    /**
     * @brief: Get the rank of this vector. This is fixed over its livetime.
     *
     * @return The rank of this vector.
     */
    [[nodiscard]] constexpr size_t rank() const
    {
        return l;
    }

    /**
     * @brief: Access the field with the given index of this Vector.
     *
     * @throws std::out_of_range When accessing elements outside of the valid
     * range.
     *
     * @return Mutable reference to the entry inside this Vector.
     */
    [[nodiscard]] constexpr T &operator[](const size_t p)
    {
        return m_array.at(p);
    }

    /**
     * @brief: Access the field with the given index of this Vector.
     *
     * @throws std::out_of_range When accessing elements outside of the valid
     * range.
     *
     * @return Constant reference to the entry inside this Vector.
     */
    [[nodiscard]] constexpr T const &operator[](const size_t p) const
    {
        return m_array.at(p);
    }

    /**
     * @brief: Multiply this Vector with another Vector, creating a scalar.
     *
     * This automatically adjuncts one of the Vectors from column-major to
     * row-major. This function also automatically promotes the return type if
     * necessary.
     *
     * @param other The other Vector.
     *
     * @return The possibly promoted scalar multiplication result of this
     * transposed Vector with another of the same rank.
     */
    template<class S, typename R = std::common_type_t<T, S>>
    [[nodiscard]] constexpr R operator*(const Vector<l, S> &other) const
    {
        return apply_each(
                   other, std::multiplies<R>(), std::make_index_sequence<l> {})
            .sum(std::make_index_sequence<l> {});
    }

    /**
     * @brief: Multiply this Vector with a scalar.
     *
     * This call promotes return type if necessary.
     *
     * @param scalar The scalar value to multiply with.
     *
     * @return The resulting Vector, possibly promoted to a different data type
     * that can best support the arithmetic operation.
     */
    template<class S, typename R = std::common_type_t<T, S>>
    [[nodiscard]] constexpr Vector<l, R> operator*(const S &scalar) const
    {

        return apply_each(
            scalar, std::multiplies<R>(), std::make_index_sequence<l> {});
    }

    /**
     * @brief: Vector addition.
     *
     * This call promotes return type if necessary.
     *
     * @param other The other vector to add to this one.
     *
     * @return The resulting Vector, possibly promoted to a different data type
     * that can best support the arithmetic operation.
     */
    template<class S, typename R = std::common_type_t<T, S>>
    [[nodiscard]] constexpr auto operator+(const Vector<l, S> &other) const
    {
        return apply_each(
            other, std::plus<R>(), std::make_index_sequence<l> {});
    }

    /**
     * @brief: Vector subtraction.
     *
     * This call promotes return type if necessary.
     *
     * @param other The other Vector to subtract from this one.
     *
     * @return The resulting Vector, possibly promoted to a different data type
     * that can best support the arithmetic operation.
     */
    template<class S, typename R = std::common_type_t<T, S>>
    [[nodiscard]] constexpr auto operator-(const Vector<l, S> &other) const
    {
        return apply_each(
            other, std::minus<R>(), std::make_index_sequence<l> {});
    }

    /**
     * @brief: Negate this vector.
     *
     * @returns The result of this Vector * -1.
     */
    [[nodiscard]] constexpr auto operator-()
    {
        return apply_each(std::negate<T>(), std::make_index_sequence<l> {});
    }

    /**
     * @brief: Comparison operator.
     *
     * @return True if Vectors are identical, false otherwise.
     */
    [[nodiscard]] constexpr bool operator==(Vector const &other) const
    {
        return m_array == other.m_array;
    }

    /**
     * @brief: Inverted comparison operator.
     *
     * @return True if Vectors are different, false otherwise.
     */
    [[nodiscard]] constexpr bool operator!=(Vector const &other) const
    {
        return m_array != other.m_array;
    }

    /**
     * @brief: Calculate the norm of this vector.
     *
     * @return The norm of this vector.
     */
    [[nodiscard]] constexpr double norm() const
    {
        double norm = 0;
        for (const auto &i : m_array)
        {
            norm += i * i;
        }
        return sqrt(norm);
    }

    /**
     * @brief: Ostream operator to pretty print Vectors.
     */
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
        return Vector<l, std::common_type_t<T, S>> {op(m_array[Idx], fac)...};
    }

    template<typename O, class Op, size_t... Idx>
    constexpr auto apply_each(const Vector<l, O> &other,
                              const Op &          op,
                              std::index_sequence<Idx...>) const
    {
        return Vector<l, std::common_type_t<T, O>> {
            op(m_array[Idx], other[Idx])...};
    }

    template<class Op, size_t... Idx>
    constexpr auto apply_each(const Op &op, std::index_sequence<Idx...>) const
    {
        return Vector {op(m_array[Idx])...};
    }

    template<size_t... Idx>
    constexpr auto sum(std::index_sequence<Idx...>) const
    {
        return (... + m_array[Idx]);
    }
};

/**
 * @brief: Template deduction guide to allow deduction of underlying data type
 * from initializer lists.
 */
template<typename R, typename... D>
Vector(R val1, D... vals)->Vector<1 + sizeof...(D), R>;

} // namespace colibra

#endif
