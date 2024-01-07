#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <qinfo.h>
    

/*----------------------------------------*/
/* Type Concretizations  */

typedef struct QInfo_value_space_d
{
    QInfo_value value;
    int         occupied;
    int         type;
    char        *name;
} QInfo_value_space_t;

typedef struct QInfo_impl_d
{
    int topic_space_size;
    int topic_space_occupied;
    QInfo_value_space_t *value_space;
} QInfo_impl_t;


/*----------------------------------------*/
/* Object creations/frees */

/*.....................................*/
/* Create an empty info oject */
/*  IN:  n/a
    OUT: reference to created info object
*/

int QInfo_create(QInfo *info)
{
    int i;
    
    *info=(QInfo_impl_t*) malloc(sizeof(QInfo_impl_t));
    if (*info==NULL)
        return QINFO_ERROR_OUTOFMEM;
    
    (*info)->topic_space_size=QINFO_INTERNAL_SPACEGRANULARITY;
    (*info)->topic_space_occupied=0;
    
    (*info)->value_space=(QInfo_value_space_t*) malloc(sizeof(QInfo_value_space_t)*(*info)->topic_space_size);
    if ((*info)->value_space==NULL)
    {
        free(*info);
        return QINFO_ERROR_OUTOFMEM;
    }
    
    for (i=0; i<(*info)->topic_space_size; i++)
    {
        (*info)->value_space[i].occupied=0;
        (*info)->value_space[i].name=NULL;
        (*info)->value_space[i].type=QINFO_TYPE_INT;
        (*info)->value_space[i].value.value_int=0;
    }
        
    return QINFO_SUCCESS;
}


/*.....................................*/
/* Duplicate an info oject */
/*  IN:  reference to exitsing info object
    OUT: reference to created info object
*/

int QInfo_duplicate(QInfo info_in, QInfo *info_out)
{
    int i;
    
    *info_out=(QInfo_impl_t*) malloc(sizeof(QInfo_impl_t));
    if (*info_out==NULL)
        return QINFO_ERROR_OUTOFMEM;
    
    (*info_out)->topic_space_size=info_in->topic_space_size;
    (*info_out)->topic_space_occupied=info_in->topic_space_occupied;
    
    (*info_out)->value_space=(QInfo_value_space_t*) malloc(sizeof(QInfo_value_space_t)*(*info_out)->topic_space_size);
    if ((*info_out)->value_space==NULL)
    {
        free(*info_out);
        return QINFO_ERROR_OUTOFMEM;
    }
    
    for (i=0; i<(*info_out)->topic_space_size; i++)
    {
        if (info_in->value_space[i].occupied)
        {
            (*info_out)->value_space[i].occupied=
                info_in->value_space[i].occupied;
            (*info_out)->value_space[i].type=
                info_in->value_space[i].type;
            (*info_out)->value_space[i].name=strdup(info_in->value_space[i].name);
            if (info_in->value_space[i].type==QINFO_TYPE_STRING)
            {
                if (info_in->value_space[i].value.value_string!=NULL)
                {
                   (*info_out)->value_space[i].value.value_string=
                        strdup(info_in->value_space[i].value.value_string);
                }
                else
                {
                    (*info_out)->value_space[i].value.value_string=NULL;
                }
            }
            else
            {
                (*info_out)->value_space[i].value=info_in->value_space[i].value;
            }
        }
        else
        {
            (*info_out)->value_space[i].name=NULL;
            (*info_out)->value_space[i].type=QINFO_TYPE_INT;
            (*info_out)->value_space[i].value.value_int=0;
        }
    }
    
    return QINFO_SUCCESS;
}


/*.....................................*/
/* Delete/Free an info oject */
/*  IN:  reference to info object to be freed
    OUT: n/a
*/

int QInfo_free(QInfo info)
{
    int i;
    
    for (i=0; i<info->topic_space_size; i++)
    {
        if (info->value_space[i].occupied)
        {
            free(info->value_space[i].name);
            if ((info->value_space[i].type==QINFO_TYPE_STRING) && (info->value_space[i].value.value_string!=NULL))
            {
                free(info->value_space[i].value.value_string);
            }
        }
    }
    
    free(info->value_space);
    free(info);
    
    return QINFO_SUCCESS;
}


/*----------------------------------------*/
/* Topic Management */

/*.....................................*/
/* Add a topic (name) to an info object */
/*  IN:  reference to info object
         name of the topic to be added
         type of topic
    OUT: reference to topic
*/

int QInfo_topic_add(QInfo info, char *topicname, QInfo_type type, QInfo_topic *topic)
{
    int i, old_size;
    
    /* Check if topic exists */
    
    for (i=0; i<info->topic_space_size; i++)
    {
        if (info->value_space[i].occupied)
        {
            if (strcmp(info->value_space[i].name,topicname)==0)
                return QINFO_ERROR_TOPICEXISTS;
        }
    }
    
    /* Check if there is space */
    
    if (info->topic_space_size==info->topic_space_occupied)
    {
        /* Need more value space */
        old_size=info->topic_space_size;
        info->topic_space_size += QINFO_INTERNAL_SPACEGRANULARITY;
        info->value_space=(QInfo_value_space_t*) realloc(info->value_space,sizeof(QInfo_value_space_t)*(info->topic_space_size));
        if (info->value_space==NULL)
            return QINFO_ERROR_OUTOFMEM;
        
        for (i=old_size; i<info->topic_space_size; i++)
        {
            info->value_space[i].occupied=0;
            info->value_space[i].name=NULL;
            info->value_space[i].type=QINFO_TYPE_INT;
            info->value_space[i].value.value_int=0;
        }
    }
        
    /* Find empty slot and occupy it */
  
    for (i=0; i<info->topic_space_size; i++)
    {
        if (info->value_space[i].occupied==0)
        {
            info->value_space[i].name=strdup(topicname);
            if (info->value_space[i].name==NULL)
                return QINFO_ERROR_OUTOFMEM;
            info->value_space[i].occupied=1;
            info->value_space[i].type=type;
            if (type==QINFO_TYPE_STRING)
                info->value_space[i].value.value_string=NULL;
            info->topic_space_occupied++;
            *topic=i;
            return QINFO_SUCCESS;
        }
    }
    
    /* Somehow we had no space */
    /* This should never happen */
    
    return QINFO_ERROR_FATAL;
}


/*.....................................*/
/* Remove a topic from an info oject */
/*  IN:  reference to info object that has the topic to be removed
         reference to the topic to be removed
    OUT: n/a
*/

int QInfo_topic_remove(QInfo info, QInfo_topic topic)
{
    if (info->value_space[topic].occupied)
    {
        info->topic_space_occupied--;
        free(info->value_space[topic].name);
        info->value_space[topic].occupied=0;
        info->value_space[topic].name=NULL;
        if ((info->value_space[topic].type==QINFO_TYPE_STRING) &&
            (info->value_space[topic].value.value_string!=NULL))
            free(info->value_space[topic].value.value_string);
        info->value_space[topic].type=QINFO_TYPE_INT;
        info->value_space[topic].value.value_int=0;
        return QINFO_SUCCESS;
    }
    return QINFO_WARN_NOTOPIC;
}


/*.....................................*/
/* Find a topic in the info oject based on the name */
/*  IN:  reference to info object to be freed
    OUT: topic found
*/

int QInfo_topic_query(QInfo info, char *topicname, QInfo_topic *topic)
{
    int i;
    
    /* Check if topic exists */
    
    for (i=0; i<info->topic_space_size; i++)
    {
        if (info->value_space[i].occupied)
        {
            if (strcmp(info->value_space[i].name,topicname)==0)
            {
                *topic=i;
                return QINFO_SUCCESS;
            }
        }
    }
    return QINFO_WARN_NOTOPIC;
}



/*.....................................*/
/* Start an iterator for searchng  all topics in an info object */
/*  IN:  reference to info object to be searched
    OUT: Iterator to be started
*/

int QInfo_topic_startiterator(QInfo info, QInfo_iterator *iter)
{
    *iter=0;
    return QINFO_SUCCESS;
}


/*.....................................*/
/* Iterate through all topics in an info object */
/*  IN:  reference to info object to be searched
    I/O: Iterator to allow retart
    OUT: Reference of the next topic
         Name of the next topic
*/

int QInfo_topic_getnext(QInfo info, QInfo_iterator *iter, QInfo_topic *topic, char **topicname)
{
    int i=0;

    for (i=*iter; i<info->topic_space_size; i++)
    {
        if (info->value_space[i].occupied)
        {
            *topicname=strdup(info->value_space[i].name);
            *iter=i+1;
            *topic=i;
            return QINFO_SUCCESS;
        }
    }
    return QINFO_WARN_NOTOPIC;
}


/*.....................................*/
/* Iterate through all topics in an info object */
/*  IN:  reference to info object to be searched
         Topic to be queried
    OUT: Type of the topic
*/


int QInfo_topic_gettype(QInfo info, QInfo_topic topic, QInfo_type *type)
{
    if (info->value_space[topic].occupied)
    {
        *type=info->value_space[topic].type;
        return QINFO_SUCCESS;
    }
    return QINFO_WARN_NOTOPIC;
}


/*----------------------------------------*/
/* Value Management */

/*.....................................*/
/* Set the value of a topic  */
/*  IN:  reference to info object
         topic to be set
         value to be set
    OUT: n/a
*/

int QInfo_topic_set(QInfo info, QInfo_topic topic, QInfo_value *val)
{
    if (info->value_space[topic].type==QINFO_TYPE_STRING)
        info->value_space[topic].value.value_string=strdup(val->value_string);
    else
        info->value_space[topic].value=*val;
    return QINFO_SUCCESS;
}


/*.....................................*/
/* Read the value of a topic */
/*  IN:  reference to info object
         topic to be set
    OUT: return value that has been read
*/

int QInfo_topic_get(QInfo info, QInfo_topic topic, QInfo_value *val)
{
    *val=info->value_space[topic].value;
    return QINFO_SUCCESS;
}


/*----------------------------------------*/
/* The End. */
