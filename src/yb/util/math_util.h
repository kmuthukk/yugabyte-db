//
// Copyright (c) YugaByte, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
// in compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied.  See the License for the specific language governing permissions and limitations
// under the License.
//

#ifndef YB_UTIL_MATH_UTIL_H_
#define YB_UTIL_MATH_UTIL_H_

#include <vector>

namespace yb {

// Returns the standard deviation of a bunch of numbers.
double standard_deviation(std::vector<double> data);

template <class T>
constexpr T constexpr_max(const T& lhs, const T& rhs) {
  return lhs > rhs ? lhs : rhs;
}

template <class T1, class T2, class... Args>
constexpr T1 constexpr_max(const T1& t1, const T2& t2, const Args&&... args) {
  return constexpr_max(t1, constexpr_max(t2, args...));
}

template <class T>
constexpr T ceil_div(const T& n, const T& div) {
  return (n + div - 1) / div;
}

}  // namespace yb

#endif // YB_UTIL_MATH_UTIL_H_
