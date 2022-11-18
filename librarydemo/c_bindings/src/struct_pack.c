#include "struct_pack.h"

//////////////////////////////////////////////////////////////////////
// touch_event_t unpacking functions
//////////////////////////////////////////////////////////////////////
#define TYPE_NAME touch_event
UNPACKER_FUNCTION(unsigned int, type)
UNPACKER_FUNCTION(unsigned int, group)
UNPACKER_FUNCTION(float, x)
UNPACKER_FUNCTION(float, y)
UNPACKER_FUNCTION(float, t)
#undef TYPE_NAME

//////////////////////////////////////////////////////////////////////
// tap_t unpacking functions
//////////////////////////////////////////////////////////////////////
#define TYPE_NAME tap
UNPACKER_FUNCTION(state_t, state)
UNPACKER_FUNCTION(float, x0)
UNPACKER_FUNCTION(float, y0)
UNPACKER_FUNCTION(float, t0)
UNPACKER_FUNCTION(float, x)
UNPACKER_FUNCTION(float, y)
UNPACKER_FUNCTION(float, t)
#undef TYPE_NAME

//////////////////////////////////////////////////////////////////////
// hold_t unpacking functions
//////////////////////////////////////////////////////////////////////
#define TYPE_NAME hold
UNPACKER_FUNCTION(state_t, state)
UNPACKER_FUNCTION(float, x0)
UNPACKER_FUNCTION(float, y0)
UNPACKER_FUNCTION(float, t0)
UNPACKER_FUNCTION(float, x)
UNPACKER_FUNCTION(float, y)
UNPACKER_FUNCTION(float, t)
#undef TYPE_NAME

//////////////////////////////////////////////////////////////////////
// hold_and_drag_t unpacking functions
//////////////////////////////////////////////////////////////////////
#define TYPE_NAME hold_and_drag
UNPACKER_FUNCTION(state_t, state)
UNPACKER_FUNCTION(float, x0)
UNPACKER_FUNCTION(float, y0)
UNPACKER_FUNCTION(float, x)
UNPACKER_FUNCTION(float, y)
UNPACKER_FUNCTION(float, vx)
UNPACKER_FUNCTION(float, vy)
#undef TYPE_NAME

//////////////////////////////////////////////////////////////////////
// double_tap_t unpacking functions
//////////////////////////////////////////////////////////////////////
#define TYPE_NAME double_tap
UNPACKER_FUNCTION(state_t, state)
UNPACKER_FUNCTION(float, x)
UNPACKER_FUNCTION(float, y)
UNPACKER_FUNCTION(float, t)
#undef TYPE_NAME

//////////////////////////////////////////////////////////////////////
// drag_t unpacking functions
//////////////////////////////////////////////////////////////////////
#define TYPE_NAME drag
UNPACKER_FUNCTION(state_t, state)
UNPACKER_FUNCTION(int, group)
UNPACKER_FUNCTION(float, x0)
UNPACKER_FUNCTION(float, y0)
UNPACKER_FUNCTION(float, x)
UNPACKER_FUNCTION(float, y)
UNPACKER_FUNCTION(float, vx)
UNPACKER_FUNCTION(float, vy)
#undef TYPE_NAME

//////////////////////////////////////////////////////////////////////
// zoom_t unpacking functions
//////////////////////////////////////////////////////////////////////
#define TYPE_NAME zoom
UNPACKER_FUNCTION(state_t, state)
UNPACKER_FUNCTION(int, uid)
UNPACKER_FUNCTION(int, size)
UNPACKER_FUNCTION(float, scale)
#undef TYPE_NAME

//////////////////////////////////////////////////////////////////////
// rotate_t unpacking functions
//////////////////////////////////////////////////////////////////////
#define TYPE_NAME rotate
UNPACKER_FUNCTION(state_t, state)
UNPACKER_FUNCTION(int, uid)
UNPACKER_FUNCTION(int, size)
UNPACKER_FUNCTION(float, rotation)
#undef TYPE_NAME

//////////////////////////////////////////////////////////////////////
// out_data unpacking functions
//////////////////////////////////////////////////////////////////////
#define TYPE_NAME out_data
ARRAY_GETTER_FUNCTION
#undef TYPE_NAME

EMSCRIPTEN_KEEPALIVE
out_data_t* get_out_data(int i) {
    return get_out_data_ptr_from_array(out_data, i);
}

#define TYPE_NAME out_type
ARRAY_GETTER_FUNCTION_NOPTR
#undef TYPE_NAME

EMSCRIPTEN_KEEPALIVE
out_type_t get_out_type(int i) {
    return get_out_type_from_array(out_types, i);
}
