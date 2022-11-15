#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <exotic/cester.h>
#include "containers/dlist.h"

#ifdef CIRCULAR_LIST
    #define IS_CIRCULAR true
#else
    #define IS_CIRCULAR false
#endif

CESTER_BODY(
c_ref div_by_2(c_ref nd, size_t index)
{
    int *data = nd;
    double *dbl = calloc(1, sizeof(double));
    *dbl = *data / 2.0;
    return dbl;
}

string show_int(c_ref data, size_t index)
{
    string s = calloc(50, sizeof(char));
    if (!s) return NULL;

    int n = 0;
    
    switch (index)
    {
        case 0: 
            n = sprintf(s, "{%d,", *(int*)data);
            break;
        case 3:
            n = sprintf(s, "%d}", *(int*)data);
            break;
        default:
            n = sprintf(s, "%d,", *(int*)data);
    }

    if (n == -1) return NULL;

    return s;
}
)

CESTER_COMMENT(
    Test cases for the linked list.
)

CESTER_BEFORE_EACH(inst, test_name, index,
    list_ref lst = dlist_create(IS_CIRCULAR);

    if (test_name == "traversing_a_list" || 
        test_name == "deleting_all_elements" || 
        test_name == "destroying_a_list" || 
        test_name == "getting_an_element_by_index" ||
        test_name == "converting_a_list_to_a_string")
    {
        int arr[3] = {1,2,3};
        for (int i = 0; i < 3; i++)
            dlist_append(lst, &arr[i], sizeof(int));
    }

    inst->arg = lst;
)

CESTER_TEST(creating_a_new_list, inst,
    list_ref lst = inst->arg;

    cester_assert_int_eq(0, dlist_get_size(lst));

#ifdef CIRCULAR_LIST
    cester_assert_true(dlist_is_circular(lst));
#else
    cester_assert_false(dlist_is_circular(lst));
#endif

    cester_assert_true(dlist_is_empty(lst));
    cester_assert_null(dlist_get_head(lst));
    cester_assert_null(dlist_get_tail(lst));
)

CESTER_TEST(appending_elements_at_end, inst,
    list_ref lst = inst->arg;

    int x = 1;
    node_ref a_node = dlist_append(lst, &x, sizeof(x));
    cester_assert_false(dlist_is_empty(lst));
    cester_assert_ptr_eq(dlist_get_head(lst), dlist_get_tail(lst));
    cester_assert_int_eq(x, *(int*)dlist_get_head(lst)->data);
    cester_assert_int_eq(1, dlist_get_size(lst));
#ifdef CIRCULAR_LIST
    cester_assert_ptr_eq(dlist_get_head(lst), dlist_get_tail(lst)->next);
    cester_assert_ptr_eq(dlist_get_tail(lst), dlist_get_head(lst)->prec);
#else
    cester_assert_null(dlist_get_head(lst)->prec);
    cester_assert_null(dlist_get_tail(lst)->next);
#endif

    x = 2;
    a_node = dlist_append(lst, &x, sizeof(x));
    cester_assert_ptr_eq(dlist_get_tail(lst), dlist_get_head(lst)->next);
    cester_assert_ptr_eq(dlist_get_head(lst), dlist_get_tail(lst)->prec);
    cester_assert_int_eq(x, *(int*)dlist_get_tail(lst)->data);
    cester_assert_int_eq(2, dlist_get_size(lst));
#ifdef CIRCULAR_LIST
    cester_assert_ptr_eq(dlist_get_head(lst), dlist_get_tail(lst)->next);
    cester_assert_ptr_eq(dlist_get_tail(lst), dlist_get_head(lst)->prec);
#else
    cester_assert_null(dlist_get_head(lst)->prec);
    cester_assert_null(dlist_get_tail(lst)->next);
#endif    
)

CESTER_TEST(prepending_elements_at_head, inst,
    list_ref lst = inst->arg;

    int x = 1;
    node_ref a_node = dlist_prepend(lst, &x, sizeof(x));
    cester_assert_false(dlist_is_empty(lst));
    cester_assert_ptr_eq(dlist_get_head(lst), dlist_get_tail(lst));
    cester_assert_int_eq(x, *(int*)dlist_get_head(lst)->data);
    cester_assert_int_eq(1, dlist_get_size(lst));
#ifdef CIRCULAR_LIST
    cester_assert_ptr_eq(dlist_get_head(lst), dlist_get_tail(lst)->next);
    cester_assert_ptr_eq(dlist_get_tail(lst), dlist_get_head(lst)->prec);
#else
    cester_assert_null(dlist_get_head(lst)->prec);
    cester_assert_null(dlist_get_tail(lst)->next);
#endif

    x = 2;
    a_node = dlist_prepend(lst, &x, sizeof(x));
    cester_assert_ptr_eq(dlist_get_tail(lst), dlist_get_head(lst)->next);
    cester_assert_ptr_eq(dlist_get_head(lst), dlist_get_tail(lst)->prec);
    cester_assert_int_eq(x, *(int*)dlist_get_head(lst)->data);
    cester_assert_int_eq(2, dlist_get_size(lst));
#ifdef CIRCULAR_LIST
    cester_assert_ptr_eq(dlist_get_head(lst), dlist_get_tail(lst)->next);
    cester_assert_ptr_eq(dlist_get_tail(lst), dlist_get_head(lst)->prec);
#else
    cester_assert_null(dlist_get_head(lst)->prec);
    cester_assert_null(dlist_get_tail(lst)->next);
#endif    
)

CESTER_TEST(traversing_a_list, inst,
    list_ref lst = inst->arg;
    
    cester_assert_int_eq(3, dlist_get_size(lst));
    cester_assert_int_eq(1, *(int*)dlist_get_head(lst)->data);
    cester_assert_int_eq(2, *(int*)dlist_get_head(lst)->next->data);
    cester_assert_int_eq(3, *(int*)dlist_get_head(lst)->next->next->data);

    list_ref nlst = dlist_traverse(lst, div_by_2);
    cester_assert_ptr_eq(lst, nlst);
    cester_assert_int_eq(3, dlist_get_size(lst));
    cester_assert_double_eq(1/2.0, *(double*)dlist_get_head(lst)->data);
    cester_assert_double_eq(2/2.0, *(double*)dlist_get_head(lst)->next->data);
    cester_assert_double_eq(3/2.0, *(double*)dlist_get_head(lst)->next->next->data);
)

CESTER_TEST(deleting_all_elements, inst,
    list_ref lst = inst->arg;
    
    cester_assert_int_eq(3, dlist_get_size(lst));

    list_ref nlst = dlist_delete_all(lst);
    cester_assert_ptr_eq(lst, nlst);
    cester_assert_int_eq(0, dlist_get_size(lst));
    cester_assert_null(dlist_get_head(lst));
    cester_assert_null(dlist_get_tail(lst));
    cester_assert_not_null(nlst);
)

CESTER_TEST(destroying_a_list, inst,
    list_ref lst = inst->arg;
    
    cester_assert_int_eq(3, dlist_get_size(lst));

    list_ref nlst = dlist_destroy(lst);
    cester_assert_ptr_ne(lst, nlst);
    cester_assert_null(nlst);
)

CESTER_TEST(getting_an_element_by_index, inst,
    list_ref lst = inst->arg;

    cester_assert_int_eq(3, dlist_get_size(lst));

    int x = *(int*)dlist_get_at(lst, 0);
    cester_assert_int_eq(1, x);

    x = *(int*)dlist_get_at(lst, 1);
    cester_assert_int_eq(2, x);

    x = *(int*)dlist_get_at(lst, 2);
    cester_assert_int_eq(3, x);

    c_ref nd = dlist_get_at(lst, -1);
    cester_assert_null(nd);

    nd = dlist_get_at(lst, 5);
    cester_assert_null(nd);
)

CESTER_TEST(converting_a_list_to_a_string, inst,
    list_ref lst = inst->arg;

    cester_assert_int_eq(3, dlist_get_size(lst));

    string lst_str = dlist_show(lst, show_int);
    cester_assert_str_eq("{1,2,3,", lst_str);
    free(lst_str);

    int x = 89;
    node_ref new_node = dlist_append(lst, &x, sizeof(x));
    lst_str = dlist_show(lst, show_int);
    cester_assert_str_eq("{1,2,3,89}", lst_str);
    free(lst_str);
)

CESTER_TEST(inserting_elements_into_a_list, inst,
    list_ref lst = inst->arg;

    cester_assert_int_eq(0, dlist_get_size(lst));

    int a[4] = {1,2,3,4};

    node_ref inserted_node = dlist_insert(lst, 0, &a[0], sizeof(int));
    cester_assert_int_eq(1, dlist_get_size(lst));

    inserted_node = dlist_insert(lst, 1, &a[1], sizeof(int));
    cester_assert_int_eq(2, dlist_get_size(lst));

    inserted_node = dlist_insert(lst, 1, &a[2], sizeof(int));
    cester_assert_int_eq(3, dlist_get_size(lst));

    inserted_node = dlist_insert(lst, 3, &a[3], sizeof(int));
    cester_assert_int_eq(4, dlist_get_size(lst));

    inserted_node = dlist_insert(lst, -1, &a[3], sizeof(int));
    cester_assert_null(inserted_node);

    inserted_node = dlist_insert(lst, 10, &a[3], sizeof(int));
    cester_assert_null(inserted_node);

    string lst_str = dlist_show(lst, show_int);
    cester_assert_str_eq("{1,3,2,4}", lst_str);
    free(lst_str);
)

CESTER_AFTER_EACH(inst, test_name, index,
    if (test_name == "destroying_a_list")
    {
        inst->arg = NULL;
    } else
    {
        inst->arg = dlist_destroy((list_ref)inst->arg);
    }
)