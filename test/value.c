#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <qinfo.h>

#define CHECK_ERR(a,b) { if (a!=QINFO_SUCCESS) { printf("Error: %i at %s\n",a,b); return 1; }}

int check_info(QInfo info, int val1, float val2, char *val3)
{
    QInfo_iterator iter;
    QInfo_topic topic;
    QInfo_value val;
    QInfo_type type;
    int counter,err;
    char *topicname;

    err=QInfo_topic_startiterator(info,&iter);
    CHECK_ERR(err,"During Iterator Start");

    counter=0;
    do {
        err=QInfo_topic_getnext(info,&iter,&topic,&topicname);
        if (err!=QINFO_WARN_NOTOPIC)
        {
            CHECK_ERR(err, "During Iterator");

            err=QInfo_topic_gettype(info,topic,&type);
            CHECK_ERR(err,"During Get Type");

            err=QInfo_topic_get(info,topic,&val);
            CHECK_ERR(err,"During Get Value");

            if (strcmp(topicname,"TOPIC_INT")==0)
            {
                printf("VALUE %i should be %i\n",val.value_int,val1);

               if (type!=QINFO_TYPE_INT)
                    CHECK_ERR(-1,"Wrong Int Type");
                if (val.value_int!=val1)
                    CHECK_ERR(-1,"Wrong Int Value");

                counter++;
            }

            if (strcmp(topicname,"TOPIC_FLOAT")==0)
            {
                printf("VALUE %f should be %f\n",val.value_float,val2);

                if (type!=QINFO_TYPE_FLOAT)
                    CHECK_ERR(-1,"Wrong Float Type");
                if (val.value_float!=val2)
                    CHECK_ERR(-1,"Wrong Float Value");
                counter++;
            }

            if (strcmp(topicname,"TOPIC_STRING")==0)
            {
                printf("VALUE %s should be %s\n",val.value_string,val3);

                if (type!=QINFO_TYPE_STRING)
                    CHECK_ERR(-1,"Wrong String Type");
                if (strcmp(val.value_string,val3)!=0)
                    CHECK_ERR(-1,"Wrong String Value");
                counter++;
            }
            free(topicname);
        }
    } while (err!=QINFO_WARN_NOTOPIC);

    if (counter!=3)
        CHECK_ERR(-1,"Not all topics found");

    return 0;
}

int main(int agrc, char** argv)
{
    QInfo info, info2;
    QInfo_topic topic1,topic2,topic3;
    QInfo_value val;
    int err;

    err=QInfo_create(&info);
    CHECK_ERR(err,"During Create");

    err=QInfo_topic_add(info, "TOPIC_INT", QINFO_TYPE_INT, &topic1);
    CHECK_ERR(err,"During Adding Topic 1");

    err=QInfo_topic_add(info, "TOPIC_FLOAT", QINFO_TYPE_FLOAT, &topic2);
    CHECK_ERR(err,"During Adding Topic 2");

    err=QInfo_topic_add(info, "TOPIC_STRING", QINFO_TYPE_STRING, &topic3);
    CHECK_ERR(err,"During Adding Topic 3");

    val.value_int=42;
    err=QInfo_topic_set(info,topic1,&val);
    CHECK_ERR(err,"During Set Value 1");

    val.value_float=0.52;
    err=QInfo_topic_set(info,topic2,&val);
    CHECK_ERR(err,"During Set Value 2");

    val.value_string=strdup("My New String");
    err=QInfo_topic_set(info,topic3,&val);
    CHECK_ERR(err,"During Set Value 3");

    err=check_info(info, 42, 0.52, "My New String");
    CHECK_ERR(err,"During Check 1");

    err=QInfo_duplicate(info,&info2);
    CHECK_ERR(err,"During Duplicate");

    err=check_info(info2, 42, 0.52, "My New String");
    CHECK_ERR(err,"During Check 2");

    val.value_float=5.52;
    err=QInfo_topic_set(info2,topic2,&val);
    CHECK_ERR(err,"During Set Value 4");

    err=check_info(info, 42, 0.52, "My New String");
    CHECK_ERR(err,"During Check 3");
    err=check_info(info2, 42, 5.52, "My New String");
    CHECK_ERR(err,"During Check 4");

    err=QInfo_free(info);
    CHECK_ERR(err,"During Free1");

    val.value_int=-4;
    err=QInfo_topic_set(info2,topic1,&val);
    CHECK_ERR(err,"During Set Value 5");

    err=check_info(info2, -4, 5.52, "My New String");
    CHECK_ERR(err,"During Check 5");

    err=QInfo_free(info2);
    CHECK_ERR(err,"During Free2");

    return 0;
}
