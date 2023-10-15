#pragma once
#include "/home/mik/Coding/hpi/hpi-c-tests/dynstring/dynstring.h"
#include "/home/mik/Coding/hpi/hpi-c-tests/list/list.h"
#include "./libSAP.h"
#include <stdbool.h>

// List utility functions
int64_t __hpi_internal_list_len(ListNode * list);
void * __hpi_internal_list_index(ListNode * list, int64_t index);
bool __hpi_internal_list_contains(ListNode * list, TypeDescriptor type, void * to_check);
