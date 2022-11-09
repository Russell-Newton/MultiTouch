const GESTURE_TYPES_BACKWARD = {
  0 : "tap",
  1 : "hold",
  2 : "hold_and_drag",
  3 : "double_tap",
  4 : "drag",
  5 : "zoom",
  6 : "rotate"
};

const TOUCH_TYPES_FORWARD = {
  down : 0,
  move : 1,
  up : 2
};

const RECOGNIZER_STATES_BACKWARD = {
  0 : "null",
  1 : "possible",
  2 : "in progress",
  3 : "failed",
  4 : "completed"
}

const GESTURE_DATA_NAMES_BACKWARD = {
  0 : "tap",
  1 : "hold",
  2 : "hold_and_drag",
  3 : "double_tap",
  4 : "drag",
  5 : "zoom",
  6 : "rotate"
}

const GESTURE_DATA_FIELDS_BACKWARD = {
  0 : [ "state", "x0", "y0", "t0", "x", "y", "t" ],
  1 : [ "state", "x0", "y0", "t0", "x", "y", "t" ],
  2 : [ "state", "x0", "y0", "x", "y", "vx", "vy" ],
  3 : [ "state", "x", "y", "t" ],
  4 : [ "state", "group", "x0", "y0", "x", "y", "vx", "vy" ],
  5 : [ "state", "uid", "size", "scale" ],
  6 : [ "state", "uid", "size", "rotation" ]
}

function convertStruct(pointer, struct_name, fields) {
  let out = {};
  for (let field of fields) {
    out[field] = eval(`Module._unpack_${struct_name}_${field}(pointer)`);
  }

  return out;
}

function convertGestureData(type, data_ptr) {
  let out = convertStruct(data_ptr, GESTURE_DATA_NAMES_BACKWARD[type],
                          GESTURE_DATA_FIELDS_BACKWARD[type]);
  out.state = RECOGNIZER_STATES_BACKWARD[out.state];
  if (type == 5) {
    out.drag1 = convertGestureData(4, out.drag1);
    out.drag2 = convertGestureData(4, out.drag2);
  }
  return out;
}

function test() {
  let square = Module.cwrap('square', 'number', [ 'number' ]);
  console.log(square(3));
};

function convertTouchEvent(pointer) {
  const fields = [ "type", "x", "y", "t", "group" ];
  return convertStruct(pointer, "touch_event", fields);
};

function convertGestureEvent(pointer) {
  const fields = [ "type", "index", "get_data" ];
  let out = convertStruct(pointer, "gesture_event", fields);
  out.type = GESTURE_TYPES_BACKWARD[out.type];
  return out;
};

function registerListeners() {
  Module._register_listeners();
}

// TODO - Unpack Gesture data
function processPointerEvent(event, type) {
  let touch_ptr = Module._build_touch_event(TOUCH_TYPES_FORWARD[type], event.x,
                                            event.y, event.timeStamp, 6);

  let out_pass_ptr = Module._lib_process_touch_event(touch_ptr);
  Module._free(touch_ptr);

  let num_out_gestures = Module._unpack_num_out_events(out_pass_ptr);
  let out_gestures_ptr = Module._unpack_out_events(out_pass_ptr);
  Module._free(out_pass_ptr);

  let out = [];
  for (let i = 0; i < num_out_gestures; i++) {
    let this_gesture_ptr =
        Module._get_gesture_event_ptr_from_array(out_gestures_ptr, i);
    out.push(convertGestureEvent(this_gesture_ptr));
  }
  Module._free(out_gestures_ptr);

  return out;
};

function testStruct() {
  let ptr = Module._build_touch_event(2, 0.5, 0.5, 10, 6);
  let out = convertTouchEvent(ptr);
  Module._free(ptr);

  console.log(out);
};

export {test, testStruct, processPointerEvent, registerListeners}
