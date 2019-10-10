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

#ifndef LIBCPP17_ONLY_HPP
#define LIBCPP17_ONLY_HPP

#if __cplusplus >= 201402L
#define USE_OVER_CPP14(value) value
#define OVER_CPP14 1
#if __cplusplus < 201703L
#define USE_ONLY_CPP14(value) value
#endif
#define CONDITIONAL_OVER_CPP14(tv, fv) tv
#define USE_ONLY_CPP11(value) /* cannot use in this version */
#else
#define USE_ONLY_CPP11(value) value
#define CONDITIONAL_OVER_CPP14(tv, fv) fv
#define USE_OVER_CPP14(value) /* cannot use in this version */
#define USE_ONLY_CPP14(value) /* cannot use in this version */
#endif

#if __cplusplus >= 201703L
#define USE_OVER_CPP17(value) value
#define OVER_CPP17 1
#else
#define USE_OVER_CPP17(value) /* cannot use in this version */
#endif

#endif //LIBCPP17_ONLY_HPP
