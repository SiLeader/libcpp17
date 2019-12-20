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

#include <type_traits>

#include <cpp17/detail/utility.hpp>

namespace cpp17 {
    template <class... Ts>
    class variant {
        typename std::aligned_storage<
                detail::max_of<std::size_t, sizeof(Ts)...>::value,
                detail::max_of<std::size_t, alignof(Ts)...>::value>::type _storage;
    };
} // namespace cpp17
