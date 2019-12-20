//
// Created by cerussite on 2019/11/05.
//

#ifndef LIBCPP17_UTILITY_HPP
#define LIBCPP17_UTILITY_HPP

#include <cstddef>

namespace cpp17 {
    template <class T>
    std::size_t size(const T& t) {
        return t.size();
    }
    template <class T, std::size_t N>
    std::size_t size(const T (&a)[N]) {
        return N;
    }

    template <class T>
    auto data(const T& t) -> decltype(t.data()) {
        return t.data();
    }
    template <class T, std::size_t N>
    const T* data(const T (&a)[N]) {
        return a;
    }
    template <class T, std::size_t N>
    T* data(T (&a)[N]) {
        return a;
    }

    template <class T>
    auto begin(const T& t) -> decltype(t.begin()) {
        return t.begin();
    }
    template <class T, std::size_t N>
    const T* begin(const T (&a)[N]) {
        return a;
    }
    template <class T, std::size_t N>
    T* begin(T (&a)[N]) {
        return a;
    }
    template <class T>
    auto end(const T& t) -> decltype(t.end()) {
        return t.end();
    }
    template <class T, std::size_t N>
    const T* end(const T (&a)[N]) {
        return a;
    }
    template <class T, std::size_t N>
    T* end(T (&a)[N]) {
        return a;
    }

    namespace detail {
        template <class T, T...>
        struct max_of;

        template <class T, T v1, T... vs>
        struct max_of<T, v1, vs...> {
            static constexpr T value = v1 > max_of<T, vs...>::value ? v1 : max_of<T, vs...>::value;
        };

        template <class T, T v1>
        struct max_of<T, v1> {
            static constexpr T value = v1;
        };
    } // namespace detail
} // namespace cpp17

#endif //LIBCPP17_UTILITY_HPP
