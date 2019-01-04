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

#ifndef STATIC_STANDARD_ANY_HPP
#define STATIC_STANDARD_ANY_HPP

#include <cstdint>
#include <stdexcept>
#include <type_traits>

#include "optional.hpp"

namespace cpp17 {
    namespace detail {
        namespace any {
            template <class ValueType>
            struct dummy {
                static void f(ValueType*) {
                }
            };
        } // namespace any
        template <class ValueType>
        std::uintptr_t type_id() {
            return reinterpret_cast<std::uintptr_t>(
                    &any::dummy<
                            typename std::remove_reference<
                                    typename std::remove_cv<
                                            ValueType>::type>::type>::f);
        }
    } // namespace detail
    class any {
    private:
        struct _any_base {
            std::uintptr_t type_id;
            _any_base() = delete;
            explicit _any_base(std::uintptr_t t)
                    : type_id(t) {
            }
            _any_base(const _any_base&) = default;
            _any_base(_any_base&&) noexcept = default;
            virtual ~_any_base() = default;
            virtual const void* get() const = 0;
            virtual _any_base* new_instance() const = 0;
        };
        template <class T>
        class _any_holder : public _any_base {
        private:
            T data;

        public:
            _any_holder()
                    : _any_base(detail::type_id<T>()) {
            }
            _any_holder(const _any_holder&) = default;
            _any_holder(_any_holder&&) noexcept = default;

            template <class... Args>
            explicit _any_holder(Args&&... args)
                    : _any_base(detail::type_id<T>()), data(std::forward<Args>(args)...) {
            }

            const void* get() const override {
                return &data;
            }
            _any_base* new_instance() const override {
                return new _any_holder<T>(data);
            }
        };

    private:
        _any_base* _any = nullptr;

    public:
        any()
                : _any(nullptr) {
        }
        any(const any& rhs)
                : any() {
            if (rhs.has_value()) {
                _any = rhs._any->new_instance();
            }
        }
        any(any&& rhs) noexcept
                : _any(rhs._any) {
            rhs._any = nullptr;
        }
        any& operator=(const any& rhs) {
            if (rhs.has_value()) {
                _any = rhs._any->new_instance();
            } else {
                reset();
            }
            return *this;
        }
        any& operator=(any&& rhs) noexcept {
            reset();
            _any = rhs._any;
            rhs._any = nullptr;
            return *this;
        }
        template <class T>
        any& operator=(T&& rhs) {
            using Ty = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
            emplace<Ty>(std::forward<T>(rhs));
            return *this;
        }
        template <class T>
        any(T&& rhs) {
            using Ty = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
            emplace<Ty>(std::forward<T>(rhs));
        }

        ~any() {
            reset();
        }

    public:
        bool has_value() const noexcept {
            return _any != nullptr;
        }
        void reset() noexcept {
            if (!has_value()) return;
            delete _any;
            _any = nullptr;
        }

    public:
        template <class Ty>
        optional<Ty> get() const {
            if (!has_value()) return nullopt;

            if (_any->type_id != detail::type_id<Ty>()) return nullopt;

            return optional<Ty>(*static_cast<const Ty*>(_any->get()));
        }

        template <class T, class... Args>
        void emplace(Args&&... args) {
            reset();
            _any = new _any_holder<T>(std::forward<Args>(args)...);
        }
    };

    struct bad_any_cast : public std::exception {
        using exception::exception;
    };

    template <class T>
    T any_cast(const any& a) {
        auto v = a.get<T>();
        if (!v) throw bad_any_cast();
        return v.value();
    }
} // namespace cpp17

#endif //STATIC_STANDARD_ANY_HPP
