const test = () => {
  let square = Module.cwrap('square', 'number', [ 'number' ]);
  console.log(square(3));
};

const convertTouchEvent = (pointer) => {
  let type = Module._unpack_touch_type(pointer);
  let x = Module._unpack_touch_x(pointer);
  let y = Module._unpack_touch_y(pointer);
  let t = Module._unpack_touch_t(pointer);
  let group = Module._unpack_touch_group(pointer);

  let out = {type : type, x : x, y : y, t : t, group : group};

  return out;
};

const convertGestureEvent = (pointer) => {
  let type = Module._unpack_gesture_type(pointer);
  let numTouches = Module._unpack_gesture_num_touches(pointer);
  let index = Module._unpack_gesture_index(pointer);
  let pGetData = Module._unpack_gesture_get_data(pointer);

  return {
    type: type, numTouches: numTouches, index: index, pGetData: pGetData
  }
};

const testStruct = () => {
  let ptr = Module._build_touch_event(2, 0.5, 0.5, 10, 6);
  let out = convertTouchEvent(ptr);
  Module._free(ptr);

  console.log(out);
};

export {test, testStruct, convertTouchEvent, convertGestureEvent}
