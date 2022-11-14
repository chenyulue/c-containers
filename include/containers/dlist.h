#ifndef _CONTAINER_LIST_H_
#define _CONTAINER_LIST_H_

#include "containers/def.h"

list_ref dlist_create(bool is_circular);
node_ref dlist_append(list_ref lst, c_ref data, size_t data_size);
node_ref dlist_prepend(list_ref lst, c_ref data, size_t data_size);
list_ref dlist_traverse(list_ref lst, cref_to_cref fun);
list_ref dlist_delete_all(list_ref lst);
list_ref dlist_destroy(list_ref lst);
c_ref dlist_get_at(list_ref lst, size_t index);
string dlist_show(list_ref lst, cref_to_string show_fun);

/* Utility functions to get information of a list. */

size_t dlist_get_size(list_ref lst);
node_ref dlist_get_head(list_ref lst);
node_ref dlist_get_tail(list_ref lst);
bool dlist_is_circular(list_ref lst);
bool dlist_is_empty(list_ref lst);

/* Functions to manipulate nodes*/

node_ref dlist_make_node(c_ref data, size_t data_size);

#endif