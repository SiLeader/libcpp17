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

#ifndef LIBCPP17_TEST_HPP
#define LIBCPP17_TEST_HPP

#include <iostream>
#include <string>

namespace test {
    namespace detail {
        std::ostream& os(bool output_stderr = true) {
            if (output_stderr) {
                return std::clog;
            }
            return std::cout;
        }
    } // namespace detail

    bool is_ok(const std::string& name, bool value, bool output_stderr = true, bool terminate_when_fail = true) {
        if (value) {
            detail::os(output_stderr) << "Test " << name << " success" << std::endl;
        } else {
            detail::os(output_stderr) << "Test " << name << " failed" << std::endl;
            if (terminate_when_fail) {
                std::terminate();
            }
        }

        return value;
    }
} // namespace test

#define TEST_TRUE(name, expr) test::is_ok(name, (expr))
#define TEST_NOTHROW(name, expr)      \
    do {                              \
        try {                         \
            (expr);                   \
            test::is_ok(name, true);  \
        } catch (...) {               \
            test::is_ok(name, false); \
        }                             \
    } while (0)
#define TEST_THROW(name, expr)        \
    do {                              \
        try {                         \
            (expr);                   \
            test::is_ok(name, false); \
        } catch (...) {               \
            test::is_ok(name, true);  \
        }                             \
    } while (0)

#endif //LIBCPP17_TEST_HPP
