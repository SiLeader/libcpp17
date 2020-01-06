//
// Copyright 2018-2019 SiLeader and Cerussite.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef CPP17_OPTIONAL_HPP
#define CPP17_OPTIONAL_HPP

#include <type_traits>
#include <utility>

#include "detail/only.hpp"


namespace cpp17 {
    struct nullopt_t {
        constexpr explicit nullopt_t(int) noexcept {
        }
    };
    struct in_place_t {
        explicit in_place_t() = default;
    };

    constexpr nullopt_t nullopt{0};
    constexpr in_place_t in_place{};

    template <class T>
    class optional {
    public:
        using value_type = T;

    private:
        typename std::aligned_storage<sizeof(T), alignof(T)>::type _cb;
        T* _buf;

    public:
        constexpr optional() noexcept
                : _cb(), _buf(nullptr) {
        }
        constexpr optional(nullopt_t) noexcept
                : optional() {
        }
        constexpr optional(const T& v)
                : _cb(), _buf(new (&_cb) T(v)) {
        }
        constexpr optional(T&& v)
                : _cb(), _buf(new (&_cb) T(std::move(v))) {
        }
        template <class... Args>
        explicit constexpr optional(in_place_t, Args&&... args)
                : _cb(), _buf(new (&_cb) T(std::forward<Args>(args)...)) {
        }

        USE_OVER_CPP14(constexpr)
        optional(const optional& rhs) noexcept(
                std::is_nothrow_copy_constructible<T>::value)
                : optional() {
            if (rhs.has_value()) {
                emplace(rhs.value());
            }
        }
        USE_OVER_CPP14(constexpr)
        optional(optional&& rhs) noexcept(
                std::is_nothrow_move_constructible<T>::value)
                : _cb(), _buf() {
            if (rhs.has_value()) {
                emplace(std::move(*rhs._buf));
            }
        }

        ~optional() {
            reset();
        }

        optional& operator=(const optional& rhs) noexcept(
                std::is_nothrow_copy_assignable<T>::value) {
            reset();
            if (rhs.has_value()) {
                emplace(rhs.value());
            }
            return *this;
        }
        optional& operator=(optional&& rhs) noexcept(
                std::is_nothrow_move_assignable<T>::value) {
            reset();
            if (rhs.has_value()) {
                emplace(std::move(*rhs._buf));
            }
            return *this;
        }

        optional& operator=(nullopt_t) {
            reset();
            return *this;
        }
        optional& operator=(const T& v) {
            emplace(v);
            return *this;
        }
        optional& operator=(T&& v) {
            emplace(std::move(v));
            return *this;
        }

    public:
        template <class... Args>
        T& emplace(Args&&... args) {
            reset();
            _buf = new (&_cb)T(std::forward<Args>(args)...);
            return *_buf;
        }
        void reset() {
            if (has_value()) {
                _buf->~T();
                _buf = nullptr;
            }
        }

    public:
        bool has_value() const noexcept {
            return _buf != nullptr;
        }
        explicit operator bool() const noexcept {
            return has_value();
        }

    public:
        constexpr T* operator->() noexcept {
            return _buf;
        }

#if OVER_CPP14
        constexpr const T* operator->() const noexcept {
            return _buf;
        }
#endif

    public:
#if OVER_CPP14
        constexpr T& value() & noexcept {
            return *_buf;
        }
#endif

        constexpr const T& value() const& noexcept {
            return *_buf;
        }

#if OVER_CPP14
        constexpr T&& value() && noexcept {
            return std::move(*_buf);
        }
#endif
        constexpr const T&& value() const&& noexcept {
            return std::move(*_buf);
        }

#if OVER_CPP14
        constexpr T& operator*() & noexcept {
            return value();
        }
#endif
        constexpr const T& operator*() const& noexcept {
            return value();
        }

#if OVER_CPP14
        constexpr T&& operator*() && noexcept {
            return std::move(value());
        }
#endif
        constexpr const T&& operator*() const&& noexcept {
            return std::move(value());
        }

    public:
        template <class U>
        constexpr T value_or(U&& v) const& {
            return has_value() ? value() : static_cast<T>(std::forward<U>(v));
        }
#if OVER_CPP14
        template <class U>
        constexpr T value_or(U&& v) && {
            return has_value() ? value() : static_cast<T>(std::forward<U>(v));
        }
#endif

    public:
        void swap(optional& rhs) {
            using std::swap;
            if (has_value()) {
                if (rhs.has_value()) {
                    swap(value(), rhs.value());
                } else {
                    rhs.value() = std::move(value());
                    reset();
                }
            } else {
                if (rhs.has_value()) {
                    value() = std::move(rhs.value());
                    rhs.reset();
                }
            }
        }
    };

    template <class T>
    void swap(optional<T>& lhs, optional<T>& rhs) {
        lhs.swap(rhs);
    }

    template <class T>
    constexpr optional<typename std::decay<T>::type> make_optional(T&& v) {
        return optional<typename std::decay<T>::type>(std::forward<T>(v));
    }

    template <class T, class... Args>
    constexpr optional<T> make_optional(Args&&... args) {
        return optional<T>(in_place, std::forward<Args>(args)...);
    }
} // namespace cpp17

#endif // CPP17_OPTIONAL_HPP
