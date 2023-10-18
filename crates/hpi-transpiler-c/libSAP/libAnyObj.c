#include "./libAnyObj.h"
#include "reflection.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>

void anyobj_insert(AnyObject *obj, char *key, AnyValue value) {
  AnyValue *value_heap = (AnyValue *)malloc(sizeof(AnyValue));
  *value_heap = value;
  hashmap_insert(obj->fields, key, value_heap);
}

AnyObject *anyobj_new() {
  AnyObject *obj = malloc(sizeof(AnyObject));
  obj->fields = hashmap_new();
  return obj;
}

AnyValue __hpi_internal_anyobj_take(AnyObject *obj, DynString *key) {
  char *key_c = dynstring_as_cstr(key);
  MapGetResult res = hashmap_get(obj->fields, key_c);
  free(key_c);

  if (res.found) {
    return *(AnyValue *)res.value;
  } else {
    TypeDescriptor val_type = {.kind = TYPE_NONE};
    AnyValue val = {.value = NULL, .type = val_type};
    return val;
  }
}

ListNode *__hpi_internal_anyobj_keys(AnyObject *obj) {
  ListNode *raw_keys = hashmap_keys(obj->fields);

  ListNode *new_list = list_new();

  for (ssize_t i = 0; i < list_len(raw_keys); i++) {
    ListGetResult temp = list_at(raw_keys, i);
    assert(temp.found);

    DynString **str = malloc(sizeof(DynString *));
    *str = dynstring_from(temp.value);
    list_append(new_list, str);
    free(temp.value);
  }

  list_free(raw_keys);

  return new_list;
}

char *display_type(TypeDescriptor type) {
  DynString *output = dynstring_new();

  switch (type.kind) {
  case TYPE_NONE:
    dynstring_set(output, "Nichts");
    break;
  case TYPE_INT:
    dynstring_set(output, "Zahl");
    break;
  case TYPE_FLOAT:
    dynstring_set(output, "Fließkommazahl");
    break;
  case TYPE_CHAR:
    dynstring_set(output, "Zeichen");
    break;
  case TYPE_BOOL:
    dynstring_set(output, "Wahrheitswert");
    break;
  case TYPE_LIST:
    dynstring_set(output, "Liste von ");
    dynstring_push_string(output, display_type(*type.list_inner));
    break;
  case TYPE_OBJECT:
    dynstring_set(output, "Objekt {");

    ListNode *keys = hashmap_keys(type.obj_fields);
    size_t keys_len = list_len(keys);

    for (int i = 0; i < keys_len; i++) {
      ListGetResult key = list_at(keys, i);
      assert(key.found);

      MapGetResult type_res = hashmap_get(type.obj_fields, key.value);
      assert(type_res.found);

      dynstring_push_string(output,
                            display_type(*(TypeDescriptor *)type_res.value));
      dynstring_push_string(output, " ");
      dynstring_push_string(output, key.value);

      if (i + 1 < keys_len) {
        dynstring_push_string(output, ", ");
      }
    }

    dynstring_push_char(output, '}');
    break;
  case TYPE_ANY_OBJECT:
    dynstring_set(output, "Speicherbox");
    break;
  case TYPE_STRING:
    dynstring_set(output, "Zeichenkette");
    break;
  }

  char *out = dynstring_as_cstr(output);
  dynstring_free(output);
  return out;
}

void __hpi_internal_validate_runtime_cast(TypeDescriptor as_type,
                                          TypeDescriptor from_type) {
  if (as_type.kind == from_type.kind) {
    return;
  }

  if (as_type.ptr_count != from_type.ptr_count) {
    goto fail;
  }

  switch (from_type.kind) {
  case TYPE_LIST: {

    // TODO: validate inner types
    break;
  case TYPE_OBJECT:
    // TODO: validate inner types
    break;
  }
  }

  return;

fail:
  printf("Runtime error: Unsupported cast: Cannot cast value of type `%s` to "
         "`%s`\n",
         display_type(from_type), display_type(as_type));
  exit(-1);
}
