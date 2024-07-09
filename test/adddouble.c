/*------------------------------------------------------------------------------
Part of the MQSS Project, under the Apache License v2.0 with LLVM Exceptions.
See https://llvm.org/LICENSE.txt for license information.
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
------------------------------------------------------------------------------*/

#include <qinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHECK_ERR(a, b)                                                        \
  {                                                                            \
    if (a != QINFO_SUCCESS) {                                                  \
      printf("Error: %i at %s\n", a, b);                                       \
      return 1;                                                                \
    }                                                                          \
  }

int main(int agrc, char **argv) {
  QInfo info;
  QInfo_topic topic1, topic2;
  int err;

  err = QInfo_create(&info);
  CHECK_ERR(err, "During Create");

  err = QInfo_topic_add(info, "TOPIC1", QINFO_TYPE_INT, &topic1);
  CHECK_ERR(err, "During Adding Topic");

  err = QInfo_topic_query(info, "TOPIC1", &topic2);
  CHECK_ERR(err, "During Querying Topic");

  if (topic1 != topic2)
    CHECK_ERR(-1, "Different Topics");

  err = QInfo_topic_remove(info, topic1);
  CHECK_ERR(err, "During Removing Topic");

  err = QInfo_free(info);
  CHECK_ERR(err, "During Free");

  return 0;
}
