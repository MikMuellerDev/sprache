#include "/home/mik/Coding/hpi/hpi-c-tests/dynstring/dynstring.h"
#include "/home/mik/Coding/hpi/hpi-c-tests/list/list.h"

ListNode * __hpi_internal_string_split(DynString * base, DynString * delim);
bool __hpi_internal_string_starts_with(DynString *base, DynString *test);
void __hpi_internal_string_replace(DynString dest, DynString * replace_src, DynString * replace_with);
