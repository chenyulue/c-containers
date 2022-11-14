#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "containers/dlist.h"

struct list
{
    size_t size;
    node_ref head, tail;
    bool is_circular;
};

static node_ref dlist_destroy_node(node_ref a_node);

/* Core functions of doubly linked list.*/

list_ref dlist_create(bool is_circular)
{
    list_ref new_lst = calloc(1, sizeof(list));

    if (!new_lst)
        return NULL;

    new_lst->is_circular = is_circular;
    return new_lst;
}

node_ref dlist_append(list_ref lst, c_ref data, size_t data_size)
{
    node_ref new_node = dlist_make_node(data, data_size);

    if (!new_node)
        return NULL;

    if (dlist_is_empty(lst))
    {
        lst->head = lst->tail = new_node;
    }
    else
    {
        lst->tail->next = new_node;
        new_node->prec = lst->tail;
        lst->tail = new_node;
    }

    if (dlist_is_circular(lst))
    {
        lst->head->prec = lst->tail;
        lst->tail->next = lst->head;
    }

    lst->size++;

    return new_node;
}

node_ref dlist_prepend(list_ref lst, c_ref data, size_t data_size)
{
    node_ref new_node = dlist_make_node(data, data_size);

    if (!new_node)
        return NULL;

    if (dlist_is_empty(lst))
    {
        lst->head = lst->tail = new_node;
    }
    else
    {
        lst->head->prec = new_node;
        new_node->next = lst->head;
        lst->head = new_node;
    }

    if (dlist_is_circular(lst))
    {
        lst->head->prec = lst->tail;
        lst->tail->next = lst->head;
    }

    lst->size++;

    return new_node;
}

list_ref dlist_traverse(list_ref lst, cref_to_cref fun)
{
    if (dlist_is_empty(lst))
        return lst;

    node_ref tmp_node = lst->head;
    bool is_circular = dlist_is_circular(lst);
    int i = 0;

    while (is_circular ? tmp_node->next != lst->head
                       : tmp_node->next)
    {
        node_ref current = tmp_node;
        c_ref orig_data = current->data;
        tmp_node = tmp_node->next;
        current->data = fun(orig_data, i);
        if (orig_data != current->data)
            free(orig_data);
        i++;
    }
    c_ref orig_data = tmp_node->data;
    tmp_node->data = fun(orig_data, i);
    if (orig_data != tmp_node->data)
        free(orig_data);

    return lst;
}

list_ref dlist_delete_all(list_ref lst)
{
    if (dlist_is_empty(lst))
        return lst;

    node_ref tmp_node = lst->head;
    bool is_circular = dlist_is_circular(lst);

    while (is_circular ? tmp_node->next != lst->head
                       : tmp_node->next)
    {
        node_ref current = tmp_node;
        tmp_node = tmp_node->next;
        dlist_destroy_node(current);
    }
    dlist_destroy_node(tmp_node);

    lst->head = lst->tail = NULL;
    lst->size = 0;

    return lst;
}

list_ref dlist_destroy(list_ref lst)
{
    list_ref nlst = dlist_delete_all(lst);
    free(nlst);
    return NULL;
}

c_ref dlist_get_at(list_ref lst, size_t index)
{
    size_t size = dlist_get_size(lst);
    if (index >= size)
        return NULL;

    node_ref current = lst->head;
    size_t i = 0;
    while (i < index)
    {
        current = current->next;
        i++;
    }

    return current->data;
}

/* Returned string need to be freed when not used. */
string dlist_show(list_ref lst, cref_to_string show_fun)
{
    size_t current_len = 0;
    size_t total_len = 100;
    string lst_str = calloc(total_len, sizeof(char));

    if (!lst_str) return NULL;

    if (dlist_is_empty(lst)) return lst_str;

    node_ref current_node = lst->head;
    size_t size = dlist_get_size(lst);
    size_t i = 0;
    while (i < size)
    {
        string tmp_str = show_fun(current_node->data, i);

        if (!tmp_str) return NULL;

        size_t tmp_len = strlen(tmp_str);

        if (current_len + tmp_len + 1 > total_len)
        {
            node_ref new_str = realloc(lst_str, 2*(current_len + tmp_len + 1));
            if (!new_str) 
            {
                lst_str = new_str;
                total_len = 2 * (current_len + tmp_len + 1);
            } else 
            {   
                free(lst_str);
                return NULL;
            }
        }

        strcpy(lst_str + current_len, tmp_str);
        free(tmp_str);
        current_len += tmp_len;

        i++;
        current_node = current_node->next;
    }

    return lst_str;
}

/* Functions to get information of a list. */

size_t dlist_get_size(list_ref lst)
{
    return lst->size;
}

node_ref dlist_get_head(list_ref lst)
{
    return lst->head;
}

node_ref dlist_get_tail(list_ref lst)
{
    return lst->tail;
}

bool dlist_is_circular(list_ref lst)
{
    return lst->is_circular;
}

bool dlist_is_empty(list_ref lst)
{
    return !lst->size;
}

/* Functions to manipulate nodes */

node_ref dlist_make_node(c_ref data, size_t data_size)
{
    node_ref new_node = calloc(1, sizeof(node));

    if (!new_node)
        return NULL;

    new_node->data = calloc(1, data_size);
    memcpy(new_node->data, data, data_size);

    return new_node;
}

static node_ref dlist_destroy_node(node_ref a_node)
{
    free(a_node->data);
    free(a_node);
    return NULL;
}

/* Other utilities */
