//
// Created by cerussite on 2019/11/05.
//

#ifndef LIBCPP17_DYNAMIC_EXTENT_HPP
#define LIBCPP17_DYNAMIC_EXTENT_HPP

#include <cstddef>
#include <limits>

namespace cpp17 {
    constexpr std::size_t dynamic_extent = std::numeric_limits<std::size_t>::max();
} // namespace cpp17

#endif //LIBCPP17_DYNAMIC_EXTENT_HPP
