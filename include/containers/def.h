#ifndef _CONTAINER_COMMON_H_
#define _CONTAINER_COMMON_H_

typedef void *c_ref;
typedef char *string;

typedef enum op_status
{
    OP_SUCCESS,
    ALLOC_FAIL,
} op_status;

typedef struct node
{
    c_ref data;

#ifndef SINGLE_LINKED_LIST
    struct node *prec;
#endif

    struct node *next;
} *node_ref, node;

typedef struct list *list_ref, list;

typedef c_ref (*cref_to_cref)(c_ref nd_data, size_t index);
typedef string (*cref_to_string)(c_ref nd_data, size_t index);

#endif