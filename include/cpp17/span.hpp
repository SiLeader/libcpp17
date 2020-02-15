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

#ifndef LIBCPP17_SPAN_HPP
#define LIBCPP17_SPAN_HPP

#include <array>
#include <cstddef>
#include <iterator>
#include <type_traits>

#include <cpp17/detail/dynamic_extent.hpp>
#include <cpp17/detail/utility.hpp>

namespace cpp17 {
    template <class T, std::size_t Extent = dynamic_extent>
    class span {
    public:
        using element_type = T;
        using value_type = typename std::remove_cv<T>::type;
        using index_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using pointer = element_type*;
        using const_pointer = const element_type*;
        using reference = element_type&;
        using const_reference = const element_type&;

        using iterator = pointer;
        using const_iterator = const_pointer;

        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        static constexpr index_type extent = Extent;

    private:
        const_pointer _data;
        index_type _length;

    public:
        constexpr span() noexcept
                : _data(nullptr), _length(0) {
        }
        constexpr span(const_pointer ptr, index_type count)
                : _data(ptr), _length(count) {
        }
        constexpr span(const_pointer first, const_pointer last)
                : span(first, last - first) {
        }

        template <std::size_t N>
        constexpr span(element_type (&a)[N]) noexcept
                : span(a, N) {
        }
        template <std::size_t N>
        constexpr span(std::array<value_type, N>& arr)
                : span(arr.data(), arr.size()) {
        }
        template <std::size_t N>
        constexpr span(const std::array<value_type, N>& arr)
                : span(arr.data(), arr.size()) {
        }

        template <class Container>
        constexpr span(Container& c)
                : span(cpp17::data(c), cpp17::size(c)) {
        }
        template <class Container>
        constexpr span(const Container& c)
                : span(cpp17::data(c), cpp17::size(c)) {
        }

        constexpr span(const span&) noexcept = default;
        constexpr span(span&&) noexcept = default;

        span& operator=(const span&) noexcept = default;
        span& operator=(span&&) noexcept = default;

        ~span() = default;

    public:
        template <std::size_t Count>
        constexpr span<element_type, Count> first() const {
            return {data(), Count};
        }

        constexpr span<element_type, dynamic_extent> first(index_type count) const {
            return {data(), count};
        }

    public:
        template <std::size_t Count>
        constexpr span<element_type, Count> last() const {
            return {data() + (size() - Count), Count};
        }

        constexpr span<element_type, dynamic_extent> last(index_type count) const {
            return {data() + (size() - count), count};
        }

    public:
        template <std::size_t Offset, std::size_t Count = dynamic_extent>
        constexpr span<element_type, Count != dynamic_extent ? Count : (Extent != dynamic_extent ? Extent - Offset : dynamic_extent)> subspan() const {
            return {data() + Offset, Count != dynamic_extent ? Count : size() - Offset};
        }

        constexpr span<element_type, dynamic_extent> subspan(index_type offset, index_type count = dynamic_extent) const {
            return {data() + offset, count != dynamic_extent ? size() - offset : count};
        }

    public:
        constexpr index_type size() const noexcept {
            return _length;
        }
        constexpr index_type size_bytes() const noexcept {
            return size() * sizeof(element_type);
        }
        constexpr bool empty() const noexcept {
            return size() == 0;
        }

    public:
        constexpr const_reference operator[](index_type i) const {
            return data()[i];
        }
        constexpr const_reference front() const {
            return (*this)[0];
        }
        constexpr const_reference back() const {
            return (*this)[size() - 1];
        }
        constexpr const_pointer data() const noexcept {
            return _data;
        }

    public:
        constexpr const_iterator begin() const noexcept {
            return data();
        }
        constexpr const_iterator end() const noexcept {
            return data() + size();
        }
        constexpr const_iterator cbegin() const noexcept {
            return data();
        }
        constexpr const_iterator cend() const noexcept {
            return data() + size();
        }

    public:
        constexpr reverse_iterator rbegin() const noexcept {
            return std::reverse_iterator<iterator>(end());
        }
        constexpr reverse_iterator rend() const noexcept {
            return std::reverse_iterator<iterator>(begin());
        }
        constexpr const_reverse_iterator crbegin() const noexcept {
            return std::reverse_iterator<const_iterator>(cend());
        }
        constexpr const_reverse_iterator crend() const noexcept {
            return std::reverse_iterator<const_iterator>(cbegin());
        }
    };
} // namespace cpp17

#endif //LIBCPP17_SPAN_HPP
