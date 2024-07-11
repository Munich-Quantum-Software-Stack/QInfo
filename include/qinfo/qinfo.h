/*------------------------------------------------------------------------------
Part of the MQSS Project, under the Apache License v2.0 with LLVM Exceptions.
See https://llvm.org/LICENSE.txt for license information.
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
------------------------------------------------------------------------------*/

/**
 * @file qinfo.h
 * @brief Header file for the QInfo API.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Status codes returned by the API.
 */
enum QINFO_STATUS {
  QINFO_WARN_NOKEY = 2,
  QINFO_WARN_GENERAL = 1,
  QINFO_SUCCESS = 0,
  QINFO_ERROR_FATAL = -1,
  QINFO_ERROR_OUTOFMEM = -2,
  QINFO_ERROR_KEYEXISTS = -3,
  QINFO_ERROR_OUTOFBOUNDS = -4,
  QINFO_ERROR_INVALIDTYPE = -5
};

/**
 * @brief Check if the return value is a success.
 * @details Only the return value of QINFO_SUCCESS is considered a success.
 * @param err The return value.
 * @return 1 if the return value is a success, 0 otherwise.
 */
inline int QInfo_is_Success(const int err) { return err == QINFO_SUCCESS; }

/**
 * @brief Check if the return value is a warning (non-fatal).
 * @details Any return value greater than QINFO_SUCCESS is considered a warning.
 * @param err The return value.
 * @return 1 if the return value is a warning, 0 otherwise.
 */
inline int QInfo_is_Warning(const int err) { return err > QINFO_SUCCESS; }

/**
 * @brief Check if the return value is an error.
 * @details Any return value less than QINFO_SUCCESS is considered an error.
 * @param err The return value.
 * @return 1 if the return value is an error, 0 otherwise.
 */
inline int QInfo_is_Error(const int err) { return err < QINFO_SUCCESS; }

/**
 * @brief Types for values stored in a QInfo object.
 */
enum QINFO_TYPE {
  QINFO_TYPE_INT = 0,
  QINFO_TYPE_LONG = 1,
  QINFO_TYPE_FLOAT = 2,
  QINFO_TYPE_DOUBLE = 3,
  QINFO_TYPE_STRING = 4
};

/**
 * @brief A container for unordered key-value pairs with heterogeneous values.
 * @details QInfo is a container for unordered key-value pairs with
 * heterogeneous values. The keys are strings and the values can be integers,
 * longs, floats, doubles, or strings. The keys are unique within a QInfo
 * object. The values can be accessed using the key or the index of the
 * key-value pair.
 * @note The QInfo object is realized as an opaque pointer to a struct. The user
 * should not access the members of the struct directly. The user should use the
 * API functions to interact with the QInfo object.
 */
typedef struct QInfo_impl_d *QInfo;

/**
 * Index of an element in a QInfo object.
 */
typedef int QInfo_index;

/**
 * Index-based iterator over a QInfo object.
 */
typedef QInfo_index QInfo_iterator;

/**
 * @brief Creates a new QInfo object.
 * @details This function creates a new QInfo object. The newly created object
 * is empty, i.e. it contains no key-value pairs.
 * @param[out] info QInfo object created (handle).
 * @return QINFO_SUCCESS on success, an error code otherwise.
 *
 * @note The user is responsible for freeing the QInfo object using the
 * QInfo_free function when the object is no longer needed.
 *
 * @see QInfo_free
 */
int QInfo_create(QInfo *info);

/**
 * @brief Create a new QInfo object as a copy of an existing QInfo object.
 * @details This function duplicates an existing info object, creating a new
 * info object with the same key-value pairs and the same ordering of keys.
 * @param[in] info_in QInfo object (handle) to duplicate.
 * @param[out] info_out QInfo object (handle) created as a copy of @p info_in.
 * @return QINFO_SUCCESS on success, an error code otherwise.
 */
int QInfo_duplicate(QInfo info_in, QInfo *info_out);

/**
 * @brief Frees a QInfo object.
 * @details This function frees @p info and sets it to QINFO_NULL.
 * @param[in,out] info QInfo object (handle).
 * @return QINFO_SUCCESS on success, an error code otherwise.
 */
int QInfo_free(QInfo info);

/**
 * @brief Adds a new entry to @p info.
 * @details This function adds a new entry to @p info with the key @p key and
 * the type @p type. The index of the new entry is stored in @p index. Overrides
 * the value if a value for the same key was previously set. A subsequent call
 * to QInfo_set with the returned index will update the value of the entry.
 * @param[in,out] info QInfo object (handle).
 * @param[in] key Key (null-terminated string).
 * @param[in] type Type of the value to be stored.
 * @param[out] index Index of the new entry.
 * @return QINFO_SUCCESS on success, an error code otherwise.
 */
int QInfo_add(QInfo info, const char *key, enum QINFO_TYPE type,
              QInfo_index *index);

/**
 * @brief Removes the entry at the index @p index from @p info.
 * @param[in,out] info QInfo object (handle).
 * @param[in] index Index of the entry to remove.
 * @return QINFO_SUCCESS on success, an error code otherwise.
 */
int QInfo_remove(QInfo info, QInfo_index index);

/**
 * @brief Queries the index of the entry with the key @p key in @p info.
 * @details Retrieves the index of the entry with the key @p key in @p info. If
 * no entry with the key @p key exists, the function returns QINFO_WARN_NOKEY.
 * A subsequent call to QInfo_get_key, QInfo_get_type, QInfo_get_val, QInfo_set,
 * or QInfo_remove with the returned index will operate on the requested entry.
 * @param[in] info QInfo object (handle).
 * @param[in] key Key (null-terminated string).
 * @param[out] index Index of the entry with the key @p key.
 * @return QINFO_SUCCESS on success, an error code otherwise.
 */
int QInfo_query(QInfo info, const char *key, QInfo_index *index);

/**
 * @brief Gets the key stored at the index @p index in @p info.
 * @param[in] info QInfo object (handle).
 * @param[in] index Index of the entry.
 * @param[out] key Key stored at the index @p index.
 * @return QINFO_SUCCESS on success, an error code otherwise.
 * @note The caller is responsible for freeing the memory allocated for the key.
 */
int QInfo_get_key(QInfo info, QInfo_index index, char **key);

/**
 * @brief Gets the type of the value stored at the index @p index in @p info.
 * @param[in] info QInfo object (handle).
 * @param[in] index Index of the entry.
 * @param[out] type Type of the value stored at the index @p index.
 * @return QINFO_SUCCESS on success, an error code otherwise.
 */
int QInfo_get_type(QInfo info, QInfo_index index, enum QINFO_TYPE *type);

/**
 * @brief Gets the integer value stored at the index @p index in @p info.
 * @param[in] info QInfo object (handle).
 * @param[in] index Index of the entry.
 * @param[out] val Value stored at the index @p index.
 * @return QINFO_SUCCESS on success, an error code otherwise.
 */
int QInfo_get_val_i(QInfo info, QInfo_index index, int *val);

/**
 * @brief Gets the long value stored at the index @p index in @p info.
 * @param[in] info QInfo object (handle).
 * @param[in] index Index of the entry.
 * @param[out] val Value stored at the index @p index.
 * @return QINFO_SUCCESS on success, an error code otherwise.
 */
int QInfo_get_val_l(QInfo info, QInfo_index index, long *val);

/**
 * @brief Gets the float value stored at the index @p index in @p info.
 * @param[in] info QInfo object (handle).
 * @param[in] index Index of the entry.
 * @param[out] val Value stored at the index @p index.
 * @return QINFO_SUCCESS on success, an error code otherwise.
 */
int QInfo_get_val_f(QInfo info, QInfo_index index, float *val);

/**
 * @brief Gets the double value stored at the index @p index in @p info.
 * @param[in] info QInfo object (handle).
 * @param[in] index Index of the entry.
 * @param[out] val Value stored at the index @p index.
 * @return QINFO_SUCCESS on success, an error code otherwise.
 */
int QInfo_get_val_d(QInfo info, QInfo_index index, double *val);

/**
 * @brief Gets the string value stored at the index @p index in @p info.
 * @param[in] info QInfo object (handle).
 * @param[in] index Index of the entry.
 * @param[out] val Value stored at the index @p index.
 * @return QINFO_SUCCESS on success, an error code otherwise.
 * @note The caller is responsible for freeing memory allocated for the value.
 */
int QInfo_get_val_c(QInfo info, QInfo_index index, char **val);

/**
 * @brief Sets the integer value stored at the index @p index in @p info.
 * @param[in,out] info QInfo object (handle).
 * @param[in] index Index of the entry.
 * @param[in] val Value to set.
 * @return QINFO_SUCCESS on success, an error code otherwise.
 */
int QInfo_set_i(QInfo info, QInfo_index index, int val);

/**
 * @brief Sets the long value stored at the index @p index in @p info.
 * @param[in,out] info QInfo object (handle).
 * @param[in] index Index of the entry.
 * @param[in] val Value to set.
 * @return QINFO_SUCCESS on success, an error code otherwise.
 */
int QInfo_set_l(QInfo info, QInfo_index index, long val);

/**
 * @brief Sets the float value stored at the index @p index in @p info.
 * @param[in,out] info QInfo object (handle).
 * @param[in] index Index of the entry.
 * @param[in] val Value to set.
 * @return QINFO_SUCCESS on success, an error code otherwise.
 */
int QInfo_set_f(QInfo info, QInfo_index index, float val);

/**
 * @brief Sets the double value stored at the index @p index in @p info.
 * @param[in,out] info QInfo object (handle).
 * @param[in] index Index of the entry.
 * @param[in] val Value to set.
 * @return QINFO_SUCCESS on success, an error code otherwise.
 */
int QInfo_set_d(QInfo info, QInfo_index index, double val);

/**
 * @brief Sets the string value stored at the index @p index in @p info.
 * @param[in,out] info QInfo object (handle).
 * @param[in] index Index of the entry.
 * @param[in] val Value to set.
 * @return QINFO_SUCCESS on success, an error code otherwise.
 */
int QInfo_set_c(QInfo info, QInfo_index index, const char *val);

/**
 * @brief Gets an iterator to the first entry in @p info.
 * @param[in] info QInfo object (handle).
 * @return Iterator to the first entry in @p info.
 *
 * @see QInfo_end
 * @see QInfo_next
 */
QInfo_iterator QInfo_begin(QInfo info);

/**
 * @brief Gets an iterator to the end of @p info.
 * @details The returned iterator points to one past the last entry in @p info.
 * It can be used to determine if an iterator has reached the end of @p info.
 * @param[in] info QInfo object (handle).
 * @return Iterator to the end of @p info.
 *
 * @see QInfo_begin
 * @see QInfo_next
 */
QInfo_iterator QInfo_end(QInfo info);

/**
 * @brief Advances the iterator @p iter to the next entry in @p info.
 * @param[in] info QInfo object (handle).
 * @param[in,out] iter Iterator to advance.
 */
void QInfo_next(QInfo info, QInfo_iterator *iter);

/**
 * @brief Determine whether the QInfo object is empty.
 * @param[in] info QInfo object (handle).
 * @return 1 if the QInfo object is empty, 0 otherwise.
 */
int QInfo_empty(QInfo info);

#ifdef __cplusplus
} // extern "C"
#endif
