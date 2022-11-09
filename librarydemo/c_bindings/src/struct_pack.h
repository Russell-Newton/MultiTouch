#pragma once

#include <emscripten.h>

#include "shared.h"

#define ARRAY_GETTER_FUNCTION                                                  \
  EMSCRIPTEN_KEEPALIVE                                                         \
  TYPE_POINTER CAT(get_, CAT(TYPE_NAME, _ptr_from_array))(                     \
      TYPE_POINTER array_ptr, int i) {                                         \
    return array_ptr + i;                                                      \
  }
  
#define ARRAY_GETTER_FUNCTION_NOPTR                                            \
  EMSCRIPTEN_KEEPALIVE                                                         \
  CAT(TYPE_NAME, _t) CAT(get_, CAT(TYPE_NAME, _ptr_from_array))(               \
      TYPE_POINTER array_ptr, int i) {                                         \
    return array_ptr[i];                                                       \
  }
  
#define UNPACKER_FUNCTION(field_type, field)                                   \
  EMSCRIPTEN_KEEPALIVE                                                         \
  field_type CAT(unpack_,                                                      \
                 CAT(TYPE_NAME, CAT(_, field)))(TYPE_POINTER ptr) {            \
    return (field_type)ptr->field;                                             \
  }
