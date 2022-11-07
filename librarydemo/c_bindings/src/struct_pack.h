#pragma once

#include <emscripten.h>

#define CAT_NOEXPAND(A, B) A##B
#define CAT(A, B) CAT_NOEXPAND(A, B)

#define STRUCT_POINTER CAT(STRUCT_NAME, _t) *

#define ARRAY_GETTER_FUNCTION                                                  \
  EMSCRIPTEN_KEEPALIVE STRUCT_POINTER CAT(                                     \
      get_, CAT(STRUCT_NAME, _ptr_from_array))(STRUCT_POINTER array_ptr,       \
                                               int i) {                        \
    return array_ptr + i;                                                      \
  }
#define UNPACKER_FUNCTION(field_type, field)                                   \
  EMSCRIPTEN_KEEPALIVE field_type CAT(                                         \
      unpack_, CAT(STRUCT_NAME, CAT(_, field)))(STRUCT_POINTER ptr) {          \
    return (field_type)ptr->field;                                             \
  }

#define MAX_OUT_EVENTS 50
