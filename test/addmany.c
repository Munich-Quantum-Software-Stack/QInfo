/*------------------------------------------------------------------------------
Part of the MQSS Project, under the Apache License v2.0 with LLVM Exceptions.
See https://llvm.org/LICENSE.txt for license information.
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <qinfo.h>

#define CHECK_ERR(a,b) { if (a!=QINFO_SUCCESS) { printf("Error: %i at %s\n",a,b); return 1; }}

int i,j;

#define OUTER 10
#define INNER 113

int main(int agrc, char** argv)
{
    QInfo info;
    QInfo_topic topic1,topic2;
    int err;
    char topicname[100];

    err=QInfo_create(&info);
    CHECK_ERR(err,"During Create");

    for (i=0; i<OUTER; i++)
    {
        for (j=0; j<INNER; j++)
        {
            sprintf(topicname,"TOPIC%i",j);

            err=QInfo_topic_add(info, topicname, QINFO_TYPE_INT, &topic1);
            CHECK_ERR(err,"During Adding Topic");
        }

        for (j=0; j<INNER; j++)
        {
            sprintf(topicname,"TOPIC%i",j);

            err=QInfo_topic_query(info, topicname, &topic2);
            CHECK_ERR(err,"During Querying Topic");

            err=QInfo_topic_remove(info,topic2);
            CHECK_ERR(err,"During Removing Topic");
        }
    }

    err=QInfo_free(info);
    CHECK_ERR(err,"During Free");

    return 0;
}
