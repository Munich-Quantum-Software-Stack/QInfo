/*------------------------------------------------------------------------------
Part of the MQSS Project, under the Apache License v2.0 with LLVM Exceptions.
See https://llvm.org/LICENSE.txt for license information.
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
------------------------------------------------------------------------------*/

/**
 * @file qinfo.c
 * @brief Implementation file for the QInfo API.
 */

#include "qinfo.h"

#include <stdlib.h>
#include <string.h>

/**
 * @brief Internal granularity for space allocation within the QInfo object.
 */
const int QINFO_INTERNAL_SPACEGRANULARITY = 10;

/**
 * @brief QInfo value union.
 * @details This union is used to store the value for a key in a QInfo object.
 */
typedef union QInfo_value_d {
  int value_int;
  long value_long;
  float value_float;
  double value_double;
  char *value_string;
} QInfo_value;

/**
 * @brief Internal structure for representing key value pairs in a QInfo object.
 */
typedef struct QInfo_value_space_d {
  QInfo_value value;    /**< The value of the key. */
  int occupied;         /**< Flag indicating if the key is occupied. */
  enum QINFO_TYPE type; /**< The type of the value. */
  char *name;           /**< The name of the key. */
} QInfo_value_space_t;

/**
 * @brief Internal structure for representing a QInfo object.

 */
typedef struct QInfo_impl_d {
  int size;                         /**< The size of the value space. */
  int num_occupied;                 /**< The number of occupied keys. */
  QInfo_value_space_t *value_space; /**< The list of key-value pairs. */
} QInfo_impl_t;

int QInfo_create(QInfo *info) {
  *info = (QInfo_impl_t *)malloc(sizeof(QInfo_impl_t));
  if (*info == NULL) {
    return QINFO_ERROR_OUTOFMEM;
  }

  (*info)->size = QINFO_INTERNAL_SPACEGRANULARITY;
  (*info)->num_occupied = 0;

  (*info)->value_space = (QInfo_value_space_t *)malloc(
      sizeof(QInfo_value_space_t) * (unsigned long)(*info)->size);
  if ((*info)->value_space == NULL) {
    free(*info);
    return QINFO_ERROR_OUTOFMEM;
  }

  for (int i = 0; i < (*info)->size; ++i) {
    (*info)->value_space[i].occupied = 0;
    (*info)->value_space[i].name = NULL;
    (*info)->value_space[i].type = QINFO_TYPE_INT;
    (*info)->value_space[i].value.value_int = 0;
  }

  return QINFO_SUCCESS;
}

int QInfo_duplicate(QInfo info_in, QInfo *info_out) {
  *info_out = (QInfo_impl_t *)malloc(sizeof(QInfo_impl_t));
  if (*info_out == NULL) {
    return QINFO_ERROR_OUTOFMEM;
  }

  (*info_out)->size = info_in->size;
  (*info_out)->num_occupied = info_in->num_occupied;

  (*info_out)->value_space = (QInfo_value_space_t *)malloc(
      sizeof(QInfo_value_space_t) * (unsigned long)(*info_out)->size);
  if ((*info_out)->value_space == NULL) {
    free(*info_out);
    return QINFO_ERROR_OUTOFMEM;
  }

  for (int i = 0; i < (*info_out)->size; ++i) {
    if (!info_in->value_space[i].occupied) {
      (*info_out)->value_space[i].name = NULL;
      (*info_out)->value_space[i].type = QINFO_TYPE_INT;
      (*info_out)->value_space[i].value.value_int = 0;
      continue;
    }

    (*info_out)->value_space[i].occupied = info_in->value_space[i].occupied;
    (*info_out)->value_space[i].type = info_in->value_space[i].type;
    (*info_out)->value_space[i].name = strdup(info_in->value_space[i].name);
    if (info_in->value_space[i].type == QINFO_TYPE_STRING) {
      if (info_in->value_space[i].value.value_string != NULL) {
        (*info_out)->value_space[i].value.value_string =
            strdup(info_in->value_space[i].value.value_string);
      } else {
        (*info_out)->value_space[i].value.value_string = NULL;
      }
    } else {
      (*info_out)->value_space[i].value = info_in->value_space[i].value;
    }
  }

  return QINFO_SUCCESS;
}

int QInfo_free(QInfo info) {
  for (int i = 0; i < info->size; ++i) {
    if (!info->value_space[i].occupied) {
      continue;
    }

    free(info->value_space[i].name);
    if (info->value_space[i].type == QINFO_TYPE_STRING) {
      free(info->value_space[i].value.value_string);
    }
  }
  free(info->value_space);
  free(info);
  return QINFO_SUCCESS;
}

int QInfo_add(QInfo info, const char *key, const enum QINFO_TYPE type,
              QInfo_index *index) {
  // Check if key exists
  for (int i = 0; i < info->size; ++i) {
    if (info->value_space[i].occupied &&
        strcmp(info->value_space[i].name, key) == 0) {
      return QINFO_ERROR_KEYEXISTS;
    }
  }

  // Check if there is space
  if (info->num_occupied == info->size) {
    // Need more space
    int old_size = info->size;
    info->size += QINFO_INTERNAL_SPACEGRANULARITY;

    QInfo_value_space_t *new_value_space = (QInfo_value_space_t *)realloc(
        info->value_space,
        sizeof(QInfo_value_space_t) * (unsigned long)info->size);
    if (new_value_space == NULL) {
      return QINFO_ERROR_OUTOFMEM;
    }
    info->value_space = new_value_space;

    for (int i = old_size; i < info->size; ++i) {
      info->value_space[i].occupied = 0;
      info->value_space[i].name = NULL;
      info->value_space[i].type = QINFO_TYPE_INT;
      info->value_space[i].value.value_int = 0;
    }
  }

  // Find empty slot and occupy it
  for (int i = 0; i < info->size; ++i) {
    if (info->value_space[i].occupied) {
      continue;
    }

    info->value_space[i].name = strdup(key);
    if (info->value_space[i].name == NULL) {
      return QINFO_ERROR_OUTOFMEM;
    }
    info->value_space[i].occupied = 1;
    info->value_space[i].type = type;
    if (type == QINFO_TYPE_STRING) {
      info->value_space[i].value.value_string = NULL;
    }
    info->num_occupied++;
    *index = i;
    return QINFO_SUCCESS;
  }

  // Somehow we had no space, which should never happen.
  return QINFO_ERROR_FATAL;
}

static inline int Check_index(QInfo info, const QInfo_index index) {
  if (index < 0 || index >= info->size) {
    return QINFO_ERROR_OUTOFBOUNDS;
  }

  if (!info->value_space[index].occupied) {
    return QINFO_WARN_NOKEY;
  }

  return QINFO_SUCCESS;
}

int QInfo_remove(QInfo info, const QInfo_index index) {
  const int err = Check_index(info, index);
  if (!QInfo_is_Success(err)) {
    return err;
  }

  free(info->value_space[index].name);
  if (info->value_space[index].type == QINFO_TYPE_STRING) {
    free(info->value_space[index].value.value_string);
  }

  info->value_space[index].occupied = 0;
  info->value_space[index].name = NULL;
  info->value_space[index].type = QINFO_TYPE_INT;
  info->value_space[index].value.value_int = 0;
  info->num_occupied--;
  return QINFO_SUCCESS;
}

int QInfo_query(QInfo info, const char *key, QInfo_index *index) {
  for (int i = 0; i < info->size; ++i) {
    if (info->value_space[i].occupied &&
        strcmp(info->value_space[i].name, key) == 0) {
      *index = i;
      return QINFO_SUCCESS;
    }
  }
  return QINFO_WARN_NOKEY;
}

int QInfo_get_key(QInfo info, const QInfo_index index, char **key) {
  const int err = Check_index(info, index);
  if (!QInfo_is_Success(err)) {
    return err;
  }

  *key = strdup(info->value_space[index].name);
  if (*key == NULL) {
    return QINFO_ERROR_OUTOFMEM;
  }

  return QINFO_SUCCESS;
}

int QInfo_get_type(QInfo info, const QInfo_index index, enum QINFO_TYPE *type) {
  const int err = Check_index(info, index);
  if (!QInfo_is_Success(err)) {
    return err;
  }

  *type = info->value_space[index].type;
  return QINFO_SUCCESS;
}

int QInfo_get_val_i(QInfo info, const QInfo_index index, int *val) {
  const int err = Check_index(info, index);
  if (!QInfo_is_Success(err)) {
    return err;
  }

  if (info->value_space[index].type != QINFO_TYPE_INT) {
    return QINFO_ERROR_INVALIDTYPE;
  }

  *val = info->value_space[index].value.value_int;
  return QINFO_SUCCESS;
}

int QInfo_get_val_l(QInfo info, const QInfo_index index, long *val) {
  const int err = Check_index(info, index);
  if (!QInfo_is_Success(err)) {
    return err;
  }

  if (info->value_space[index].type != QINFO_TYPE_LONG) {
    return QINFO_ERROR_INVALIDTYPE;
  }

  *val = info->value_space[index].value.value_long;
  return QINFO_SUCCESS;
}

int QInfo_get_val_f(QInfo info, const QInfo_index index, float *val) {
  const int err = Check_index(info, index);
  if (!QInfo_is_Success(err)) {
    return err;
  }

  if (info->value_space[index].type != QINFO_TYPE_FLOAT) {
    return QINFO_ERROR_INVALIDTYPE;
  }

  *val = info->value_space[index].value.value_float;
  return QINFO_SUCCESS;
}

int QInfo_get_val_d(QInfo info, const QInfo_index index, double *val) {
  const int err = Check_index(info, index);
  if (!QInfo_is_Success(err)) {
    return err;
  }

  if (info->value_space[index].type != QINFO_TYPE_DOUBLE) {
    return QINFO_ERROR_INVALIDTYPE;
  }

  *val = info->value_space[index].value.value_double;
  return QINFO_SUCCESS;
}

int QInfo_get_val_c(QInfo info, const QInfo_index index, char **val) {
  const int err = Check_index(info, index);
  if (!QInfo_is_Success(err)) {
    return err;
  }

  if (info->value_space[index].type != QINFO_TYPE_STRING) {
    return QINFO_ERROR_INVALIDTYPE;
  }

  *val = strdup(info->value_space[index].value.value_string);
  if (*val == NULL) {
    return QINFO_ERROR_OUTOFMEM;
  }

  return QINFO_SUCCESS;
}

int QInfo_set_i(QInfo info, const QInfo_index index, int val) {
  const int err = Check_index(info, index);
  if (!QInfo_is_Success(err)) {
    return err;
  }

  if (info->value_space[index].type != QINFO_TYPE_INT) {
    return QINFO_ERROR_INVALIDTYPE;
  }

  info->value_space[index].value.value_int = val;
  return QINFO_SUCCESS;
}

int QInfo_set_l(QInfo info, const QInfo_index index, long val) {
  const int err = Check_index(info, index);
  if (!QInfo_is_Success(err)) {
    return err;
  }

  if (info->value_space[index].type != QINFO_TYPE_LONG) {
    return QINFO_ERROR_INVALIDTYPE;
  }

  info->value_space[index].value.value_long = val;
  return QINFO_SUCCESS;
}

int QInfo_set_f(QInfo info, const QInfo_index index, float val) {
  const int err = Check_index(info, index);
  if (!QInfo_is_Success(err)) {
    return err;
  }

  if (info->value_space[index].type != QINFO_TYPE_FLOAT) {
    return QINFO_ERROR_INVALIDTYPE;
  }

  info->value_space[index].value.value_float = val;
  return QINFO_SUCCESS;
}

int QInfo_set_d(QInfo info, const QInfo_index index, double val) {
  const int err = Check_index(info, index);
  if (!QInfo_is_Success(err)) {
    return err;
  }

  if (info->value_space[index].type != QINFO_TYPE_DOUBLE) {
    return QINFO_ERROR_INVALIDTYPE;
  }

  info->value_space[index].value.value_double = val;
  return QINFO_SUCCESS;
}

int QInfo_set_c(QInfo info, const QInfo_index index, const char *val) {
  const int err = Check_index(info, index);
  if (!QInfo_is_Success(err)) {
    return err;
  }

  if (info->value_space[index].type != QINFO_TYPE_STRING) {
    return QINFO_ERROR_INVALIDTYPE;
  }

  free(info->value_space[index].value.value_string);
  info->value_space[index].value.value_string = strdup(val);
  if (info->value_space[index].value.value_string == NULL) {
    return QINFO_ERROR_OUTOFMEM;
  }

  return QINFO_SUCCESS;
}

QInfo_iterator QInfo_begin(QInfo info) {
  for (int i = 0; i < info->size; ++i) {
    if (info->value_space[i].occupied) {
      return i;
    }
  }
  return QInfo_end(info);
}

QInfo_iterator QInfo_end(QInfo info) { return info->size; }

void QInfo_next(QInfo info, QInfo_iterator *iter) {
  for (int i = *iter + 1; i < info->size; ++i) {
    if (info->value_space[i].occupied) {
      *iter = i;
      return;
    }
  }
  *iter = QInfo_end(info);
}

int QInfo_empty(QInfo info) { return info->num_occupied == 0; }
