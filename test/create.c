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
  int err;

  err = QInfo_create(&info);
  CHECK_ERR(err, "During Create");

  err = QInfo_free(info);
  CHECK_ERR(err, "During Free");

  return 0;
}
