/*------------------------------------------------------------------------------
Part of the MQSS Project, under the Apache License v2.0 with LLVM Exceptions.
See https://llvm.org/LICENSE.txt for license information.
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
------------------------------------------------------------------------------*/

#include "qinfo.h"

#include <cstdint>
#include <cstdlib>
#include <gtest/gtest.h>
#include <string>

class QInfoTest : public ::testing::Test {
protected:
  void SetUp() override {
    ASSERT_TRUE(QInfo_is_Success(QInfo_create(&info))) << "Creation failed";
  }

  void TearDown() override {
    ASSERT_TRUE(QInfo_is_Success(QInfo_free(info))) << "Free failed";
  }

  QInfo info{};
};

TEST_F(QInfoTest, addAndDelete) {
  QInfo_index index{};
  ASSERT_TRUE(
      QInfo_is_Success(QInfo_add(info, "test", QINFO_TYPE_INT32, &index)))
      << "Could not add key";

  char *key{};
  ASSERT_TRUE(QInfo_is_Success(QInfo_get_key(info, index, &key)))
      << "Could not get key";
  ASSERT_STREQ(key, "test") << "Wrong key";
  free(key); // NOLINT(*-owning-memory, *-no-malloc)

  QInfo_index index2{};
  ASSERT_TRUE(QInfo_is_Success(QInfo_query(info, "test", &index2)))
      << "Could not query key";
  ASSERT_EQ(index, index2) << "Different indices for the same key";
}

TEST_F(QInfoTest, addManyEntries) {
  constexpr int outer = 10;
  constexpr int inner = 113;

  QInfo_index index{};
  for (int i = 0; i < outer; ++i) {
    for (int j = 0; j < inner; ++j) {
      const std::string key = "key_" + std::to_string(j);
      ASSERT_TRUE(QInfo_is_Success(
          QInfo_add(info, key.c_str(), QINFO_TYPE_INT32, &index)))
          << "Could not add key";
    }
    for (int j = 0; j < inner; ++j) {
      const std::string key = "key_" + std::to_string(j);
      ASSERT_TRUE(QInfo_is_Success(QInfo_query(info, key.c_str(), &index)))
          << "Could not query key";

      ASSERT_TRUE(QInfo_is_Success(QInfo_remove(info, index)))
          << "Could not remove key";
    }
    ASSERT_TRUE(QInfo_empty(info)) << "Info is not empty";
  }
}

TEST_F(QInfoTest, intValues) {
  QInfo_index index{};
  ASSERT_TRUE(
      QInfo_is_Success(QInfo_add(info, "int", QINFO_TYPE_INT32, &index)))
      << "Could not add key";

  QInfo_index index2{};
  ASSERT_TRUE(QInfo_is_Success(QInfo_query(info, "int", &index2)))
      << "Could not query key";
  ASSERT_EQ(index, index2) << "Different indices for the same key";

  QINFO_TYPE type{};
  ASSERT_TRUE(QInfo_is_Success(QInfo_get_type(info, index, &type)))
      << "Could not get type";
  ASSERT_EQ(type, QINFO_TYPE_INT32) << "Wrong type";

  const int value = 42;
  ASSERT_TRUE(QInfo_is_Success(QInfo_set_i32(info, index, value)))
      << "Could not set int value";

  int value2{};
  ASSERT_TRUE(QInfo_is_Success(QInfo_get_val_i32(info, index, &value2)))
      << "Could not get int value";
  ASSERT_EQ(value, value2) << "Values do not match";

  ASSERT_TRUE(QInfo_is_Error(QInfo_get_val_i64(info, index, nullptr)))
      << "Should not be able to get long value";

  ASSERT_TRUE(QInfo_is_Error(QInfo_get_val_f(info, index, nullptr)))
      << "Should not be able to get float value";

  ASSERT_TRUE(QInfo_is_Error(QInfo_get_val_d(info, index, nullptr)))
      << "Should not be able to get double value";

  ASSERT_TRUE(QInfo_is_Error(QInfo_get_val_c(info, index, nullptr)))
      << "Should not be able to get string value";
}

TEST_F(QInfoTest, longValues) {
  QInfo_index index{};
  ASSERT_TRUE(
      QInfo_is_Success(QInfo_add(info, "long", QINFO_TYPE_INT64, &index)))
      << "Could not add key";

  QInfo_index index2{};
  ASSERT_TRUE(QInfo_is_Success(QInfo_query(info, "long", &index2)))
      << "Could not query key";
  ASSERT_EQ(index, index2) << "Different indices for the same key";

  QINFO_TYPE type{};
  ASSERT_TRUE(QInfo_is_Success(QInfo_get_type(info, index, &type)))
      << "Could not get type";
  ASSERT_EQ(type, QINFO_TYPE_INT64) << "Wrong type";

  const int64_t value = 42LL;
  ASSERT_TRUE(QInfo_is_Success(QInfo_set_i64(info, index, value)))
      << "Could not set long value";

  int64_t value2{};
  ASSERT_TRUE(QInfo_is_Success(QInfo_get_val_i64(info, index, &value2)))
      << "Could not get long value";
  ASSERT_EQ(value, value2) << "Values do not match";

  ASSERT_TRUE(QInfo_is_Error(QInfo_get_val_i32(info, index, nullptr)))
      << "Should not be able to get int value";

  ASSERT_TRUE(QInfo_is_Error(QInfo_get_val_f(info, index, nullptr)))
      << "Should not be able to get float value";

  ASSERT_TRUE(QInfo_is_Error(QInfo_get_val_d(info, index, nullptr)))
      << "Should not be able to get double value";

  ASSERT_TRUE(QInfo_is_Error(QInfo_get_val_c(info, index, nullptr)))
      << "Should not be able to get string value";
}

TEST_F(QInfoTest, floatValues) {
  QInfo_index index{};
  ASSERT_TRUE(
      QInfo_is_Success(QInfo_add(info, "float", QINFO_TYPE_FLOAT, &index)))
      << "Could not add key";

  QInfo_index index2{};
  ASSERT_TRUE(QInfo_is_Success(QInfo_query(info, "float", &index2)))
      << "Could not query key";
  ASSERT_EQ(index, index2) << "Different indices for the same key";

  QINFO_TYPE type{};
  ASSERT_TRUE(QInfo_is_Success(QInfo_get_type(info, index, &type)))
      << "Could not get type";
  ASSERT_EQ(type, QINFO_TYPE_FLOAT) << "Wrong type";

  const float value = 42.0F;
  ASSERT_TRUE(QInfo_is_Success(QInfo_set_f(info, index, value)))
      << "Could not set float value";

  float value2{};
  ASSERT_TRUE(QInfo_is_Success(QInfo_get_val_f(info, index, &value2)))
      << "Could not get float value";
  ASSERT_EQ(value, value2) << "Values do not match";

  ASSERT_TRUE(QInfo_is_Error(QInfo_get_val_i32(info, index, nullptr)))
      << "Should not be able to get int value";

  ASSERT_TRUE(QInfo_is_Error(QInfo_get_val_i64(info, index, nullptr)))
      << "Should not be able to get long value";

  ASSERT_TRUE(QInfo_is_Error(QInfo_get_val_d(info, index, nullptr)))
      << "Should not be able to get double value";

  ASSERT_TRUE(QInfo_is_Error(QInfo_get_val_c(info, index, nullptr)))
      << "Should not be able to get string value";
}

TEST_F(QInfoTest, doubleValues) {
  QInfo_index index{};
  ASSERT_TRUE(
      QInfo_is_Success(QInfo_add(info, "double", QINFO_TYPE_DOUBLE, &index)))
      << "Could not add key";

  QInfo_index index2{};
  ASSERT_TRUE(QInfo_is_Success(QInfo_query(info, "double", &index2)))
      << "Could not query key";
  ASSERT_EQ(index, index2) << "Different indices for the same key";

  QINFO_TYPE type{};
  ASSERT_TRUE(QInfo_is_Success(QInfo_get_type(info, index, &type)))
      << "Could not get type";
  ASSERT_EQ(type, QINFO_TYPE_DOUBLE) << "Wrong type";

  const double value = 42.0;
  ASSERT_TRUE(QInfo_is_Success(QInfo_set_d(info, index, value)))
      << "Could not set double value";

  double value2{};
  ASSERT_TRUE(QInfo_is_Success(QInfo_get_val_d(info, index, &value2)))
      << "Could not get double value";
  ASSERT_EQ(value, value2) << "Values do not match";

  ASSERT_TRUE(QInfo_is_Error(QInfo_get_val_i32(info, index, nullptr)))
      << "Should not be able to get int value";

  ASSERT_TRUE(QInfo_is_Error(QInfo_get_val_i64(info, index, nullptr)))
      << "Should not be able to get long value";

  ASSERT_TRUE(QInfo_is_Error(QInfo_get_val_f(info, index, nullptr)))
      << "Should not be able to get float value";

  ASSERT_TRUE(QInfo_is_Error(QInfo_get_val_c(info, index, nullptr)))
      << "Should not be able to get string value";
}

TEST_F(QInfoTest, stringValues) {
  QInfo_index index{};
  ASSERT_TRUE(
      QInfo_is_Success(QInfo_add(info, "string", QINFO_TYPE_STRING, &index)))
      << "Could not add key";

  QInfo_index index2{};
  ASSERT_TRUE(QInfo_is_Success(QInfo_query(info, "string", &index2)))
      << "Could not query key";
  ASSERT_EQ(index, index2) << "Different indices for the same key";

  QINFO_TYPE type{};
  ASSERT_TRUE(QInfo_is_Success(QInfo_get_type(info, index, &type)))
      << "Could not get type";
  ASSERT_EQ(type, QINFO_TYPE_STRING) << "Wrong type";

  const std::string str = "Hello, World!";
  ASSERT_TRUE(QInfo_is_Success(QInfo_set_c(info, index, str.c_str())))
      << "Could not set string value";

  char *value2{};
  ASSERT_TRUE(QInfo_is_Success(QInfo_get_val_c(info, index, &value2)))
      << "Could not get string value";
  ASSERT_STREQ(str.c_str(), value2) << "Values do not match";
  free(value2); // NOLINT(*-owning-memory, *-no-malloc)

  ASSERT_TRUE(QInfo_is_Error(QInfo_get_val_i32(info, index, nullptr)))
      << "Should not be able to get int value";

  ASSERT_TRUE(QInfo_is_Error(QInfo_get_val_i64(info, index, nullptr)))
      << "Should not be able to get long value";

  ASSERT_TRUE(QInfo_is_Error(QInfo_get_val_f(info, index, nullptr)))
      << "Should not be able to get float value";

  ASSERT_TRUE(QInfo_is_Error(QInfo_get_val_d(info, index, nullptr)))
      << "Should not be able to get double value";
}

TEST_F(QInfoTest, duplicate) {
  const int value = 42;
  QInfo_index index{};
  ASSERT_TRUE(
      QInfo_is_Success(QInfo_add(info, "int", QINFO_TYPE_INT32, &index)))
      << "Could not add key";
  ASSERT_TRUE(QInfo_is_Success(QInfo_set_i32(info, index, value)))
      << "Could not set int value";

  ASSERT_TRUE(
      QInfo_is_Success(QInfo_add(info, "string", QINFO_TYPE_STRING, &index)))
      << "Could not add key";
  const char *str = "Hello, World!";
  ASSERT_TRUE(QInfo_is_Success(QInfo_set_c(info, index, str)))
      << "Could not set string value";

  QInfo info2{};
  ASSERT_TRUE(QInfo_is_Success(QInfo_duplicate(info, &info2)))
      << "Could not duplicate info";

  QInfo_index index2{};
  ASSERT_TRUE(QInfo_is_Success(QInfo_query(info2, "int", &index2)))
      << "Could not query key";
  int value2{};
  ASSERT_TRUE(QInfo_is_Success(QInfo_get_val_i32(info2, index2, &value2)))
      << "Could not get int value";
  ASSERT_EQ(value, value2) << "Values do not match";

  ASSERT_TRUE(QInfo_is_Success(QInfo_query(info2, "string", &index2)))
      << "Could not query key";
  char *str2{};
  ASSERT_TRUE(QInfo_is_Success(QInfo_get_val_c(info2, index2, &str2)))
      << "Could not get string value";
  ASSERT_STREQ(str, str2) << "Values do not match";
  free(str2); // NOLINT(*-owning-memory, *-no-malloc)

  ASSERT_TRUE(QInfo_is_Success(QInfo_free(info2))) << "Free failed";
}

TEST_F(QInfoTest, stringRemove) {
  QInfo_index index{};
  ASSERT_TRUE(
      QInfo_is_Success(QInfo_add(info, "string", QINFO_TYPE_STRING, &index)))
      << "Could not add key";

  const std::string str = "Hello, World!";
  ASSERT_TRUE(QInfo_is_Success(QInfo_set_c(info, index, str.c_str())))
      << "Could not set string value";

  ASSERT_TRUE(QInfo_is_Success(QInfo_remove(info, index)))
      << "Could not remove key";

  char *value{};
  ASSERT_TRUE(QInfo_is_Warning(QInfo_get_val_c(info, index, &value)))
      << "Should not be able to get string value";
}

TEST_F(QInfoTest, errorOnExistingKey) {
  QInfo_index index{};
  ASSERT_TRUE(
      QInfo_is_Success(QInfo_add(info, "test", QINFO_TYPE_INT32, &index)))
      << "Could not add key";

  ASSERT_TRUE(QInfo_is_Error(QInfo_add(info, "test", QINFO_TYPE_INT32, &index)))
      << "Should not be able to add existing key";
}

TEST_F(QInfoTest, errOutOfBounds) {
  const QInfo_index index = -1;

  ASSERT_TRUE(QInfo_is_Error(QInfo_remove(info, index)))
      << "Should not be able to remove non-existing key";

  ASSERT_TRUE(QInfo_is_Error(QInfo_get_type(info, index, nullptr)))
      << "Should not be able to get type of non-existing key";

  ASSERT_TRUE(QInfo_is_Error(QInfo_get_key(info, index, nullptr)))
      << "Should not be able to get key of non-existing key";

  ASSERT_TRUE(QInfo_is_Error(QInfo_get_val_i32(info, index, nullptr)))
      << "Should not be able to get value of non-existing key";

  ASSERT_TRUE(QInfo_is_Error(QInfo_get_val_i64(info, index, nullptr)))
      << "Should not be able to get value of non-existing key";

  ASSERT_TRUE(QInfo_is_Error(QInfo_get_val_f(info, index, nullptr)))
      << "Should not be able to get value of non-existing key";

  ASSERT_TRUE(QInfo_is_Error(QInfo_get_val_d(info, index, nullptr)))
      << "Should not be able to get value of non-existing key";

  ASSERT_TRUE(QInfo_is_Error(QInfo_get_val_c(info, index, nullptr)))
      << "Should not be able to get value of non-existing key";

  ASSERT_TRUE(QInfo_is_Error(QInfo_set_i32(info, index, 0)))
      << "Should not be able to set value of non-existing key";

  ASSERT_TRUE(QInfo_is_Error(QInfo_set_i64(info, index, 0)))
      << "Should not be able to set value of non-existing key";

  ASSERT_TRUE(QInfo_is_Error(QInfo_set_f(info, index, 0)))
      << "Should not be able to set value of non-existing key";

  ASSERT_TRUE(QInfo_is_Error(QInfo_set_d(info, index, 0)))
      << "Should not be able to set value of non-existing key";

  ASSERT_TRUE(QInfo_is_Error(QInfo_set_c(info, index, nullptr)))
      << "Should not be able to set value of non-existing key";
}

TEST_F(QInfoTest, warnNotOccupied) {
  const QInfo_index index = 0;

  ASSERT_TRUE(QInfo_is_Warning(QInfo_remove(info, index)))
      << "Should not be able to remove non-existing key";

  ASSERT_TRUE(QInfo_is_Warning(QInfo_get_type(info, index, nullptr)))
      << "Should not be able to get type of non-existing key";

  ASSERT_TRUE(QInfo_is_Warning(QInfo_get_key(info, index, nullptr)))
      << "Should not be able to get key of non-existing key";

  ASSERT_TRUE(QInfo_is_Warning(QInfo_get_val_i32(info, index, nullptr)))
      << "Should not be able to get value of non-existing key";

  ASSERT_TRUE(QInfo_is_Warning(QInfo_get_val_i64(info, index, nullptr)))
      << "Should not be able to get value of non-existing key";

  ASSERT_TRUE(QInfo_is_Warning(QInfo_get_val_f(info, index, nullptr)))
      << "Should not be able to get value of non-existing key";

  ASSERT_TRUE(QInfo_is_Warning(QInfo_get_val_d(info, index, nullptr)))
      << "Should not be able to get value of non-existing key";

  ASSERT_TRUE(QInfo_is_Warning(QInfo_get_val_c(info, index, nullptr)))
      << "Should not be able to get value of non-existing key";

  ASSERT_TRUE(QInfo_is_Warning(QInfo_set_i32(info, index, 0)))
      << "Should not be able to set value of non-existing key";

  ASSERT_TRUE(QInfo_is_Warning(QInfo_set_i64(info, index, 0)))
      << "Should not be able to set value of non-existing key";

  ASSERT_TRUE(QInfo_is_Warning(QInfo_set_f(info, index, 0)))
      << "Should not be able to set value of non-existing key";

  ASSERT_TRUE(QInfo_is_Warning(QInfo_set_d(info, index, 0)))
      << "Should not be able to set value of non-existing key";

  ASSERT_TRUE(QInfo_is_Warning(QInfo_set_c(info, index, nullptr)))
      << "Should not be able to set value of non-existing key";
}

TEST_F(QInfoTest, warnQueryNotFound) {
  QInfo_index index{};
  ASSERT_TRUE(QInfo_is_Warning(QInfo_query(info, "test", &index)))
      << "Should not be able to query non-existing key";
}

TEST_F(QInfoTest, errorInvalidType) {
  QInfo_index index{};
  ASSERT_TRUE(
      QInfo_is_Success(QInfo_add(info, "test", QINFO_TYPE_INT32, &index)))
      << "Could not add key";

  ASSERT_TRUE(QInfo_is_Error(QInfo_set_i64(info, index, 0)))
      << "Should not be able to set long value for int key";

  ASSERT_TRUE(QInfo_is_Error(QInfo_set_f(info, index, 0)))
      << "Should not be able to set float value for int key";

  ASSERT_TRUE(QInfo_is_Error(QInfo_set_d(info, index, 0)))
      << "Should not be able to set double value for int key";

  ASSERT_TRUE(QInfo_is_Error(QInfo_set_c(info, index, nullptr)))
      << "Should not be able to set string value for int key";

  QInfo_index index2{};
  ASSERT_TRUE(
      QInfo_is_Success(QInfo_add(info, "test2", QINFO_TYPE_INT64, &index2)))
      << "Could not add key";

  ASSERT_TRUE(QInfo_is_Error(QInfo_set_i32(info, index2, 0)))
      << "Should not be able to set int value for long key";
}

TEST_F(QInfoTest, iteration) {
  for (int i = 0; i < 10; ++i) {
    const std::string key = "key_" + std::to_string(i);
    QInfo_index index{};
    ASSERT_TRUE(QInfo_is_Success(
        QInfo_add(info, key.c_str(), QINFO_TYPE_INT32, &index)))
        << "Could not add key";
    ASSERT_TRUE(QInfo_is_Success(QInfo_set_i32(info, index, i)))
        << "Could not set int value";
  }

  for (QInfo_index i = QInfo_begin(info); i < QInfo_end(info);
       QInfo_next(info, &i)) {
    char *key{};
    ASSERT_TRUE(QInfo_is_Success(QInfo_get_key(info, i, &key)))
        << "Could not get key";
    int value{};
    ASSERT_TRUE(QInfo_is_Success(QInfo_get_val_i32(info, i, &value)))
        << "Could not get int value";
    ASSERT_EQ(value, i) << "Values do not match";
    free(key); // NOLINT(*-owning-memory, *-no-malloc)
  }
}

TEST_F(QInfoTest, onEmptyObjectBeginEqualsEnd) {
  ASSERT_TRUE(QInfo_empty(info)) << "Info should be empty";
  ASSERT_EQ(QInfo_begin(info), QInfo_end(info))
      << "Begin and end should be equal";
}
