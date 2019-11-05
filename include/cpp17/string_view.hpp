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

#pragma once

#include <limits>
#include <stdexcept>
#include <string>

#include <cpp17/detail/only.hpp>

namespace cpp17 {
    template <class CharT, class Traits = std::char_traits<CharT>>
    class basic_string_view {
    public:
        using traits_type = Traits;
        using value_type = CharT;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using reference = value_type&;
        using const_reference = const value_type&;
        using const_iterator = const_pointer;
        using iterator = const_iterator;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = reverse_iterator;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

    public:
        static constexpr size_type npos = -1;

    private:
        const_pointer _first;
        size_type _length;

    public:
        constexpr basic_string_view() noexcept
                : _first(nullptr), _length(0) {
        }
        constexpr basic_string_view(const basic_string_view&) noexcept = default;
        constexpr basic_string_view(basic_string_view&&) noexcept = default;
        constexpr basic_string_view(const_pointer str) noexcept
                : _first(str), _length(__builtin_strlen(str)) {
        }
        constexpr basic_string_view(const_pointer str, size_type len) noexcept
                : _first(str), _length(len) {
        }
        constexpr basic_string_view(const std::basic_string<CharT>& s)
                : _first(s.data()), _length(s.size()) {
        }

        USE_OVER_CPP17(constexpr)
        basic_string_view& operator=(const basic_string_view&) noexcept = default;
        USE_OVER_CPP17(constexpr)
        basic_string_view& operator=(basic_string_view&&) noexcept = default;

        ~basic_string_view() = default;

    public:
        constexpr const_iterator begin() const noexcept {
            return _first;
        }
        constexpr const_iterator end() const noexcept {
            return begin() + size();
        }

        constexpr const_iterator cbegin() const noexcept {
            return begin();
        }
        constexpr const_iterator cend() const noexcept {
            return end();
        }

        constexpr const_reverse_iterator rbegin() const noexcept {
            return const_reverse_iterator(end());
        }
        constexpr const_reverse_iterator rend() const noexcept {
            return const_reverse_iterator(begin());
        }

        constexpr const_reverse_iterator crbegin() const noexcept {
            return rbegin();
        }
        constexpr const_reverse_iterator crend() const noexcept {
            return rend();
        }

    public:
        constexpr size_type size() const noexcept {
            return _length;
        }
        constexpr size_type length() const noexcept {
            return size();
        }

        constexpr size_type max_size() const noexcept {
            return std::numeric_limits<size_type>::max();
        }

        constexpr bool empty() const noexcept {
            return size() == 0;
        }

    public:
        constexpr const_pointer data() const {
            return _first;
        }

        constexpr const_reference operator[](size_type pos) const noexcept {
            return data()[pos];
        }

        constexpr const_reference at(size_type pos) const {
            return size() <= pos ? throw std::out_of_range("") : (*this)[pos];
        }

        constexpr const_reference front() const noexcept {
            return data()[0];
        }
        constexpr const_reference back() const noexcept {
            return data()[size() - 1];
        }

    public:
        USE_OVER_CPP17(constexpr)
        void remove_prefix(size_type n) {
            _first += n;
            _length -= n;
        }
        USE_OVER_CPP17(constexpr)
        void remove_suffix(size_type n) {
            _length -= n;
        }

    public:
        USE_OVER_CPP17(constexpr)
        void swap(basic_string_view& s) noexcept {
            using std::swap;
            swap(_first, s._first);
            swap(_length, s._length);
        }

    public:
        size_type copy(pointer s, size_type n, size_type pos = 0) const {
            if (pos > size()) throw std::out_of_range("");
            traits_type::copy(s, data() + pos, n);
            return n;
        }

    private:
        constexpr size_type _substr_helper(size_type a, size_type b) const {
            return a < b ? a : b;
        }

    public:
        constexpr basic_string_view substr(size_type pos = 0, size_type n = npos) const {
            return basic_string_view(data() + pos, _substr_helper(size() - pos, n));
        }

    private:
        constexpr int _compare_helper(int res, int r) const {
            return res != 0 ? res : r;
        }

    public:
        constexpr int compare(basic_string_view sv) const noexcept {
            return _compare_helper(traits_type::compare(data(), sv.data(), std::min(size(), sv.size())), size() - sv.size());
        }
        constexpr int compare(size_type pos1, size_type n1, basic_string_view sv) const {
            return substr(pos1, n1).compare(sv);
        }
        constexpr int compare(size_type pos1, size_type n1, basic_string_view sv, size_type pos2, size_type n2) const {
            return substr(pos1, n1).compare(sv.substr(pos2, n2));
        }
        constexpr int compare(const CharT* s) const {
            return compare(basic_string_view(s));
        }
        constexpr int compare(size_type pos1, size_type n1, const CharT* s) const {
            return substr(pos1, n1).compare(basic_string_view(s));
        }
        constexpr int compare(size_type pos1, size_type n1, const CharT* s, size_type n2) const {
            return substr(pos1, n1).compare(basic_string_view(s, n2));
        }

    public:
        constexpr bool starts_with(basic_string_view x) const noexcept {
            return substr(0, x.size()) == x;
        }
        constexpr bool starts_with(value_type x) const noexcept {
            return !empty() && traits_type::eq(front(), x);
        }
        constexpr bool starts_with(const CharT* x) const {
            return starts_with(basic_string_view(x));
        }

    public:
        constexpr bool ends_with(basic_string_view x) const noexcept {
            return size() >= x.size() && compare(size() - x.size(), npos, x) == 0;
        }
        constexpr bool ends_with(CharT x) const noexcept {
            return !empty() && traits_type::eq(back(), x);
        }
        constexpr bool ends_with(const CharT* x) const {
            return ends_with(basic_string_view(x));
        }

    public:
        constexpr size_type find(basic_string_view sv, size_type pos = 0) const noexcept {
            return pos < size() ? (compare(pos, sv.size(), sv) == 0 ? pos : find(sv, ++pos)) : npos;
        }
        constexpr size_type find(CharT c, size_type pos = 0) const noexcept {
            return find(basic_string_view(&c, 1), pos);
        }
        constexpr size_type find(const CharT* s, size_type pos, size_type n) const {
            return find(basic_string_view(s, n), pos);
        }
        constexpr size_type find(const CharT* s, size_type pos = 0) const {
            return find(basic_string_view(s), pos);
        }

    public:
        std::basic_string<CharT> str() const {
            return std::basic_string<CharT>(begin(), end());
        }
    };

    template <class CharT>
    constexpr bool operator==(basic_string_view<CharT> lhs, basic_string_view<CharT> rhs) {
        return lhs.compare(rhs) == 0;
    }

    template <class CharT>
    constexpr bool operator!=(basic_string_view<CharT> lhs, basic_string_view<CharT> rhs) {
        return !(lhs == rhs);
    }

    template <class CharT>
    constexpr bool operator<(basic_string_view<CharT> lhs, basic_string_view<CharT> rhs) {
        return lhs.compare(rhs) < 0;
    }

    template <class CharT>
    constexpr bool operator>=(basic_string_view<CharT> lhs, basic_string_view<CharT> rhs) {
        return !(lhs < rhs);
    }

    template <class CharT>
    constexpr bool operator<=(basic_string_view<CharT> lhs, basic_string_view<CharT> rhs) {
        return lhs < rhs || lhs == rhs;
    }

    template <class CharT>
    constexpr bool operator>(basic_string_view<CharT> lhs, basic_string_view<CharT> rhs) {
        return !(lhs <= rhs);
    }

    template <class CharT>
    std::basic_string<CharT> operator+(std::basic_string<CharT> lhs, const basic_string_view<CharT>& rhs) {
        lhs.insert(std::end(lhs), rhs.begin(), rhs.end());
        return lhs;
    }
    template <class CharT>
    std::basic_string<CharT> operator+(const basic_string_view<CharT>& lhs, const std::basic_string<CharT>& rhs) {
        auto r = rhs;
        r.insert(std::begin(r), lhs.begin(), lhs.end());
        return r;
    }
    template <class CharT>
    std::basic_string<CharT> operator+(const CharT* lhs, const basic_string_view<CharT>& rhs) {
        return std::basic_string<CharT>(lhs) + rhs;
    }
    template <class CharT>
    std::basic_string<CharT> operator+(const basic_string_view<CharT>& lhs, const CharT* rhs) {
        return lhs + std::basic_string<CharT>(rhs);
    }

    template <class CharT>
    std::basic_ostream<CharT>& operator<<(std::basic_ostream<CharT>& os, const basic_string_view<CharT>& s) {
        os.write(s.data(), s.size());
        return os;
    }

    using string_view = basic_string_view<char>;
    using wstring_view = basic_string_view<wchar_t>;
    using u16string_view = basic_string_view<char16_t>;
    using u32string_view = basic_string_view<char32_t>;
} // namespace cpp17
