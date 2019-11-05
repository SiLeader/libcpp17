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

#include <cpp17/any.hpp>
#include <cpp17/optional.hpp>
#include <cpp17/span.hpp>
#include <cpp17/string_view.hpp>

#include "test.hpp"

int main() {
    cpp17::any any;
    TEST_NOTHROW("assign int", any = 1);
    TEST_NOTHROW("get as int", cpp17::any_cast<int>(any));
    TEST_THROW("get as short", cpp17::any_cast<short>(any));
    TEST_TRUE("compare with 1", cpp17::any_cast<int>(any) == 1);

    cpp17::optional<int> opt;
    TEST_TRUE("not has value", !opt.has_value());
    opt = 3;
    TEST_TRUE("has value", opt.has_value());
    TEST_TRUE("value is 3", opt.value() == 3);
    opt.reset();
    TEST_TRUE("not has value", !opt.has_value());

    cpp17::string_view sv;
    TEST_TRUE("empty", sv.empty());
    sv = "123";
    TEST_TRUE("size = 3", sv.size() == 3);
    TEST_TRUE("starts_with 12", sv.starts_with("12"));
    TEST_TRUE("ends_with 23", sv.ends_with("23"));
    TEST_TRUE("not starts_with 23", !sv.starts_with("23"));
    TEST_TRUE("not ends_with 12", !sv.ends_with("12"));
    TEST_TRUE("find 23 == 1", sv.find("23") == 1);

    {
        cpp17::span<int> spn;
        TEST_TRUE("empty", spn.empty());
    }
    std::array<int, 10> array = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    cpp17::span<int> spn(array);
    TEST_TRUE("spn.size() == array.size()", spn.size() == array.size());
}
