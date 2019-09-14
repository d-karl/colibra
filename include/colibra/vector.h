#ifndef COLIBRA_VECTOR_H
#define COLIBRA_VECTOR_H

#include "details/vector.hpp"

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
class Vector : details::Vector<l, T>
{
    using array_type = typename std::array<T, l>;

    using Impl_ = details::Vector<l, T>;

  public:
    /**
     * @brief: Create a new Vector given an arbitrary number of initial
     * values.
     */
    template<typename... P>
    explicit constexpr Vector(T const val1, P const... vals)
        : Impl_(std::move(val1), std::move(vals)...)
    {
    }

    /**
     * @brief: Create a new, empty vector. You must manually define the size and
     * type the Vector holds if you use this function.
     */
    constexpr Vector()
        : Impl_()
    {
    }

    /***************
     *  iterators  *
     ***************/
    auto begin() noexcept -> decltype(std::declval<array_type>().begin())
    {
        return Impl_::begin();
    }

    auto cbegin() noexcept -> decltype(std::declval<array_type>().cbegin())
    {
        return Impl_::cbegin();
    }

    auto end() noexcept -> decltype(std::declval<array_type>().end())
    {
        return Impl_::end();
    }

    auto cend() noexcept -> decltype(std::declval<array_type>().cend())
    {
        return Impl_::cend();
    }

    /**
     * @brief: Get the rank of this vector. This is fixed over its livetime.
     *
     * @return The rank of this vector.
     */
    [[nodiscard]] constexpr size_t rank() const
    {
        return Impl_::rank();
    }

    /**
     * @brief: Access the field with the given index of this Vector with range
     * checking.
     *
     * @throws std::out_of_range When accessing fields out of range.
     *
     * @return Mutable reference to the entry inside this Vector.
     */
    [[nodiscard]] constexpr T &at(const size_t p)
    {
        return Impl_::at(p);
    }

    /**
     * @brief: Access the field with the given index of this Vector with range
     * checking.
     *
     * @throws std::out_of_range When accessing fields out of range.
     *
     * @return Constant reference to the entry inside this Vector.
     */
    [[nodiscard]] constexpr T const &at(const size_t p) const
    {
        return Impl_::at(p);
    }

    /**
     * @brief: Access the field with the given index of this Vector.
     *
     * @warning Does not perform range-checking.
     *
     * @return Mutable reference to the entry inside this Vector.
     */
    [[nodiscard]] constexpr T &operator[](const size_t p)
    {
        return Impl_::operator[](p);
    }

    /**
     * @brief: Access the field with the given index of this Vector.
     *
     * @warning Does not perform range-checking.
     *
     * @return Constant reference to the entry inside this Vector.
     */
    [[nodiscard]] constexpr T const &operator[](const size_t p) const
    {
        return Impl_::operator[](p);
    }

    /**
     * @brief: Dot multiply this Vector with another.
     *
     * This automatically adjuncts one of the Vectors from column-major to
     * row-major. This function also automatically promotes the return type
     * if necessary.
     *
     * @param other The other Vector.
     *
     * @return The possibly promoted scalar multiplication result of this
     * transposed Vector with another of the same rank.
     */
    template<class S, typename R = std::common_type_t<T, S>>
    [[nodiscard]] constexpr R operator*(const Vector<l, S> &other) const
    {
        return Impl_::operator*(other);
    }


    /**
     * @brief: Multiply this Vector with a scalar.
     *
     * This call promotes return type if necessary.
     *
     * @param scalar The scalar value to multiply with.
     *
     * @return The resulting Vector, possibly promoted to a different data
     * type that can best support the arithmetic operation.
     */
    template<class S, typename R = std::common_type_t<T, S>>
    [[nodiscard]] constexpr Vector<l, R> operator*(const S &scalar) const
    {
        return Impl_::operator*(scalar);
    }

    /**
     * @brief: Vector addition.
     *
     * This call promotes return type if necessary.
     *
     * @param other The other vector to add to this one.
     *
     * @return The resulting Vector, possibly promoted to a different data
     * type that can best support the arithmetic operation.
     */
    template<class S, typename R = std::common_type_t<T, S>>
    [[nodiscard]] constexpr auto operator+(const Vector<l, S> &other) const
    {
        return Impl_::operator+(other);
    }

    /**
     * @brief: Vector subtraction.
     *
     * This call promotes return type if necessary.
     *
     * @param other The other Vector to subtract from this one.
     *
     * @return The resulting Vector, possibly promoted to a different data
     * type that can best support the arithmetic operation.
     */
    template<class S, typename R = std::common_type_t<T, S>>
    [[nodiscard]] constexpr auto operator-(const Vector<l, S> &other) const
    {
        return Impl_::operator-(other);
    }

    /**
     * @brief: Negate this vector.
     *
     * @returns The result of this Vector * -1.
     */
    [[nodiscard]] constexpr auto operator-()
    {
        return Impl_::operator-();
    }

    /**
     * @brief: Comparison operator.
     *
     * @return True if Vectors are identical, false otherwise.
     */
    [[nodiscard]] constexpr bool operator==(Vector const &other) const
    {
        return Impl_::operator==(other);
    }

    /**
     * @brief: Inverted comparison operator.
     *
     * @return True if Vectors are different, false otherwise.
     */
    [[nodiscard]] constexpr bool operator!=(Vector const &other) const
    {
        return Impl_::operator!=(other);
    }

    /**
     * @brief: Calculate the norm of this vector.
     *
     * @return The norm of this vector.
     */
    [[nodiscard]] constexpr double norm() const
    {
        return Impl_::norm();
    }

    /**
     * @brief: Calculate the dot product between this and another Vector.
     *
     * This function automatically promotes the result type if necessary.
     *
     * @param other The other Vector
     *
     * @return The dot product.
     */
    template<class S, typename R = std::common_type_t<T, S>>
    [[nodiscard]] constexpr R dot(Vector<l, S> &other) const
    {
        return Impl_::operator*(other);
    }

    /**
     * @brief: Get a ptr to the underlying array in this Vector.
     *
     * @return Ptr to data.
     */
    [[nodiscard]] constexpr const T *data() const
    {
        return Impl_::data();
    }

    /**
     * @brief: Ostream operator to pretty print Vectors.
     */
    friend std::ostream &operator<<(std::ostream &os, const Vector<l, T> &vec)
    {
        return os << static_cast<Impl_>(vec);
    }
};

/**
 * @brief: Template deduction guide to allow deduction of underlying data
 * type from initializer lists.
 */
template<typename R, typename... D>
Vector(R val1, D... vals)->Vector<1 + sizeof...(D), R>;

} // namespace colibra

#endif
