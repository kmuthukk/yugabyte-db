// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
// The following only applies to changes made to this file as part of YugaByte development.
//
// Portions Copyright (c) YugaByte, Inc.
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
// Some portions Copyright 2013 The Chromium Authors. All rights reserved.

#include "yb/gutil/strings/util.h"
#include "yb/util/string_util.h"

#include <gtest/gtest.h>

using std::string;

namespace yb {

TEST(StringUtilTest, MatchPatternTest) {
  EXPECT_TRUE(MatchPattern("www.google.com", "*.com"));
  EXPECT_TRUE(MatchPattern("www.google.com", "*"));
  EXPECT_FALSE(MatchPattern("www.google.com", "www*.g*.org"));
  EXPECT_TRUE(MatchPattern("Hello", "H?l?o"));
  EXPECT_FALSE(MatchPattern("www.google.com", "http://*)"));
  EXPECT_FALSE(MatchPattern("www.msn.com", "*.COM"));
  EXPECT_TRUE(MatchPattern("Hello*1234", "He??o\\*1*"));
  EXPECT_FALSE(MatchPattern("", "*.*"));
  EXPECT_TRUE(MatchPattern("", "*"));
  EXPECT_TRUE(MatchPattern("", "?"));
  EXPECT_TRUE(MatchPattern("", ""));
  EXPECT_FALSE(MatchPattern("Hello", ""));
  EXPECT_TRUE(MatchPattern("Hello*", "Hello*"));
  // Stop after a certain recursion depth.
  EXPECT_FALSE(MatchPattern("123456789012345678", "?????????????????*"));

  // Test UTF8 matching.
  EXPECT_TRUE(MatchPattern("heart: \xe2\x99\xa0", "*\xe2\x99\xa0"));
  EXPECT_TRUE(MatchPattern("heart: \xe2\x99\xa0.", "heart: ?."));
  EXPECT_TRUE(MatchPattern("hearts: \xe2\x99\xa0\xe2\x99\xa0", "*"));
  // Invalid sequences should be handled as a single invalid character.
  EXPECT_TRUE(MatchPattern("invalid: \xef\xbf\xbe", "invalid: ?"));
  // If the pattern has invalid characters, it shouldn't match anything.
  EXPECT_FALSE(MatchPattern("\xf4\x90\x80\x80", "\xf4\x90\x80\x80"));

  // This test verifies that consecutive wild cards are collapsed into 1
  // wildcard (when this doesn't occur, MatchPattern reaches it's maximum
  // recursion depth).
  EXPECT_TRUE(MatchPattern("Hello" ,
                           "He********************************o"));
}

TEST(StringUtilTest, TestEqualsIgnoreCase) {
  ASSERT_TRUE(EqualsIgnoreCase(string("abcd"), string("ABCD")));
  ASSERT_TRUE(EqualsIgnoreCase(string("AbCd"), string("aBCD")));
  ASSERT_TRUE(EqualsIgnoreCase(string(""), string("")));
  ASSERT_FALSE(EqualsIgnoreCase(string("ABCD"), string("ABC")));
  ASSERT_FALSE(EqualsIgnoreCase(string("ABC"), string("ABCD")));
  ASSERT_FALSE(EqualsIgnoreCase(string("ABCD"), string("ABC")));
  ASSERT_FALSE(EqualsIgnoreCase(string("ABC"), string("ABE")));
}

TEST(StringUtilTest, TestAppendWithSeparator) {
  string s;
  AppendWithSeparator("foo", &s);
  ASSERT_EQ(s, "foo");
  AppendWithSeparator("bar", &s);
  ASSERT_EQ(s, "foo, bar");
  AppendWithSeparator("foo", &s, " -- ");
  ASSERT_EQ(s, "foo, bar -- foo");

  s = "";
  AppendWithSeparator(string("foo"), &s);
  ASSERT_EQ(s, "foo");
  AppendWithSeparator(string("bar"), &s);
  ASSERT_EQ(s, "foo, bar");
  AppendWithSeparator(string("foo"), &s, " -- ");
  ASSERT_EQ(s, "foo, bar -- foo");
}

TEST(StringUtilTest, TestCollectionToString) {
  std::vector<std::string> v{"foo", "123", "bar", ""};
  ASSERT_EQ("[foo, 123, bar, ]", VectorToString(v));
  ASSERT_EQ("[foo, 123, bar, ]", RangeToString(v.begin(), v.end()));
  ASSERT_EQ("[]", RangeToString(v.begin(), v.begin()));
  ASSERT_EQ("[foo]", RangeToString(v.begin(), v.begin() + 1));
}

} // namespace yb
