/*------------------------------------------------------------------------------
Part of the MQSS Project, under the Apache License v2.0 with LLVM Exceptions.
See https://llvm.org/LICENSE.txt for license information.
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
------------------------------------------------------------------------------*/

#pragma once

/*----------------------------------------*/
/* QInfo Constants for API usage*/

#define QINFO_NULL NULL;


/* Error classes, i.e., status returned by API */

#define QINFO_WARN_NOTOPIC        2
#define QINFO_WARN_GENERAL        1
#define QINFO_SUCCESS             0
#define QINFO_ERROR_FATAL         -1
#define QINFO_ERROR_OUTOFMEM      -2
#define QINFO_ERROR_TOPICEXISTS   -3

#define QINFO_IS_SUCCESS(err) (err==QINFO_SUCCESS)
#define QINFO_IS_WARNING(err) (err>QINFO_SUCCESS)
#define QINFO_IS_ERROR(err)   (err!=QINFO_SUCCESS)
#define QINFO_IS_FATAL(err)   (err<QINFO_SUCCESS)


/* Types for topics */

#define QINFO_TYPE_INT      0
#define QINFO_TYPE_LONG     1
#define QINFO_TYPE_FLOAT    2
#define QINFO_TYPE_DOUBLE   3
#define QINFO_TYPE_STRING   4


/*----------------------------------------*/
/* Internal constants */

#define QINFO_INTERNAL_SPACEGRANULARITY 10


/*----------------------------------------*/
/* Info Type defintions */

typedef union QInfo_value_d
{
    int     value_int;
    long    value_long;
    float   value_float;
    double  value_double;
    char*   value_string;
} QInfo_value;

typedef struct QInfo_impl_d *QInfo;

typedef int  QInfo_topic;
typedef int  QInfo_iterator;
typedef int  QInfo_type;


/*----------------------------------------*/
/* Function defintions */

#ifdef __cplusplus
extern "C" {
#endif

int QInfo_create(QInfo *info);
int QInfo_duplicate(QInfo info_in, QInfo *info_out);
int QInfo_free(QInfo info);
int QInfo_topic_add(QInfo info, char *topicname, QInfo_type type, QInfo_topic *topic);
int QInfo_topic_remove(QInfo info, QInfo_topic topic);
int QInfo_topic_query(QInfo info, char *topicname, QInfo_topic *topic);
int QInfo_topic_startiterator(QInfo info, QInfo_iterator *iter);
int QInfo_topic_getnext(QInfo info, QInfo_iterator *iter, QInfo_topic *topic, char** topicname);
int QInfo_topic_gettype(QInfo info, QInfo_topic topic, QInfo_type *type);
int QInfo_topic_set(QInfo info, QInfo_topic topic, QInfo_value *val);
int QInfo_topic_get(QInfo info, QInfo_topic topic, QInfo_value *val);

#ifdef __cplusplus
} // extern "C"
#endif
