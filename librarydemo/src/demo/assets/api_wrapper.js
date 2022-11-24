const GESTURE_TYPES_BACKWARD = {
  0 : "Tap",
  1 : "Hold",
  2 : "Hold & Drag",
  3 : "Double-Tap",
  4 : "Drag",
  5 : "Zoom",
  6 : "Rotate",
  7 : "KTap"
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
  6 : "rotate",
  7 : "ktap"
}

const GESTURE_DATA_FIELDS_BACKWARD =
    {
      0 : [ "state", "x0", "y0", "t0", "x", "y", "t" ],
      1 : [ "state", "x0", "y0", "t0", "x", "y", "t" ],
      2 : [ "state", "x0", "y0", "x", "y", "vx", "vy" ],
      3 : [ "state", "x", "y", "t" ],
      4 : [ "state", "group", "x0", "y0", "x", "y", "vx", "vy" ],
      5 : [ "state", "uid", "size", "scale" ],
      6 : [ "state", "uid", "size", "rotation" ],
      7 : [ "state", "x", "y", "t", "count" ]
    }

function convertStruct(pointer, struct_name, fields) {
  let out = {};
  for (let field of fields) {
    out[field] = Module[`_unpack_${struct_name}_${field}`](pointer);
  }

  return out;
}

function convertGestureData(type, data_ptr) {
  let out = convertStruct(data_ptr, GESTURE_DATA_NAMES_BACKWARD[type],
                          GESTURE_DATA_FIELDS_BACKWARD[type]);
  out.state = RECOGNIZER_STATES_BACKWARD[out.state];
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

function registerListeners() { Module._register_listeners(); }

function unpackOutData(nOutData) {
  let out = [];
  for (let i = 0; i < nOutData; i++) {
    let type = Module._get_out_type(i);
    let struct_name = GESTURE_DATA_NAMES_BACKWARD[type];
    let rawDataPtr = Module._get_out_data(i);
    let convertedDataPtr = Module[`_out_data_as_${struct_name}`](rawDataPtr);
    out.push({
      event : GESTURE_TYPES_BACKWARD[type],
      data : convertGestureData(type, convertedDataPtr)
    });
  }
  return out;
}

function processPointerEvent(event, type) {
  let {x, y, t} = event;
  let touch_ptr =
      Module._build_touch_event(TOUCH_TYPES_FORWARD[type], x, y, t, 6);

  let nOutData = Module._lib_process_touch_event(touch_ptr);
  Module._free(touch_ptr);

  return unpackOutData(nOutData);
};

function outDataToString(outData) {
  let inProgress = "Gestures in progress:\n";
  let completed = "Gestures completed:\n";
  let bullet = String.fromCodePoint(8226);
  for (let data of outData) {
    if (data.data.state == "in progress") {
      inProgress += `  ${bullet} ${data.event}
`;
    }
    if (data.data.state == "completed") {
      completed += `  ${bullet} ${data.event}
`;
    }
  }

  if (inProgress && completed) {
    return `${inProgress}\n${completed}`;
  }
  if (inProgress) {
    return `${inProgress}`;
  }
  if (completed) {
    return `${completed}`;
  }
  return "No gestures recognized."
}

function testStruct() {
  let ptr = Module._build_touch_event(2, 0.5, 0.5, 10, 6);
  let out = convertTouchEvent(ptr);
  Module._free(ptr);

  console.log(out);
};

export {
  test,
  testStruct,
  processPointerEvent,
  registerListeners,
  outDataToString
}
