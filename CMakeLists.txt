#
# Copyright 2018-2019 SiLeader and Cerussite.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

project(libcpp17)
cmake_minimum_required(VERSION 3.5)

include_directories(include)

set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE SOURCE src/*.cpp)
file(GLOB_RECURSE HEADER include/*.hpp)

add_library(cpp17 STATIC ${SOURCE} ${HEADER} include/cpp17/span.hpp include/cpp17/detail/dynamic_extent.hpp include/cpp17/detail/utility.hpp)

add_executable(cpp17test test/test.cpp test/test.hpp)
target_link_libraries(cpp17test cpp17)