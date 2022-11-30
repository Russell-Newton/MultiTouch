<template>
  <canvas v-bind:width="width" v-bind:height="height"
          style="touch-action: none; width: 100%; max-height: 70vh; aspect-ratio: 1;"
          @pointerdown.prevent.stop="pointerdown"
          @pointerup.prevent.stop="pointerup"
          @pointermove.prevent.stop="pointermove"
          @pointercancel.prevent.stop="pointerup"
  >
  </canvas>
</template>

<script>
import {processPointerEvent, registerListeners, outDataToString} from "../assets/api_wrapper";

export default {
  data() {
    return {
      dragging: {},
      initialized: false,
      capturedEvents: [],
    }
  },
  props: {
    width: Number,
    height: Number,
    setParentText: Function,
    setDumpData: Function,
  },
  created() {
    document.addEventListener("emscriptenready", this.initLib);
  },
  destroyed() {
    document.removeEventListener("emscriptenready", this.initLib);
  },
  mounted() {
    let canvas = this.$el;
    let ctx = canvas.getContext("2d");
    ctx.globalCompositeOperation = 'destination-over';
    ctx.fillStyle = "lightgrey";
    ctx.fillRect(0, 0, canvas.width, canvas.height);

    this.setDumpData(this.dumpCaptured);
    this.initCapture();
  },
  methods: {
    initLib() {
      if (this.initialized) {
        return;
      }

      Module._init_gesturelib();
      registerListeners();
      console.log("Library ready!");
      this.initialized = true;
    },
    initCapture() {
      this.capturedEvents = [
        "type,position.dx,position.dy,timeStamp,width,height,pressure"
      ]
    },
    eventToCanvasCoordinate(event) {
      // need to use getBoundingClientRect for responsive <canvas> sizing
      // NOTE: if you use transforms, see
      // https://stackoverflow.com/a/59259174
      // to invert transform matrix
      let canvas = this.$el;
      let style = canvas.currentStyle || window.getComputedStyle(canvas, null)
      let borderLeftWidth = parseInt(style['borderLeftWidth'], 10);
      let borderTopWidth = parseInt(style['borderTopWidth'], 10);
      let bounds = canvas.getBoundingClientRect();
      let x = event.x - borderLeftWidth - bounds.left;
      let y = event.y - borderTopWidth - bounds.top;
      x = x * canvas.width / canvas.clientWidth;
      y = y * canvas.height / canvas.clientHeight;
      y = canvas.height - y;
      return {x, y};
    },
    checkDragging(group) {
      if (!(group in this.dragging)) {
        this.dragging[group] = false;
      }
      return this.dragging[group];
    },
    pointermove(event) {
      if (!this.checkDragging(event.pointerId)) {
        return;
      }
      this.logEvent(event, "move");
    },
    pointerdown(event) {
      if (this.checkDragging(event.pointerId)) {
        return;
      }
      this.dragging[event.pointerId] = true;
      // we want all the events until the pointer is released
      event.target.setPointerCapture(event.pointerId);
      this.logEvent(event, "down");
    },
    pointerup(event) {
      if (!this.checkDragging(event.pointerId)) {
        return;
      }
      delete this.dragging[event.pointerId];
      this.logEvent(event, "up");
    },
    logEvent(event, type) {
      this.captureEvent(event, type);
      let {x, y} = this.eventToCanvasCoordinate(event);
      let t = event.timeStamp / 1000;   // in ms by default
      let processed_gestures = processPointerEvent({x, y, t}, type);
      if (processed_gestures.length > 0) {
        this.setParentText(outDataToString(processed_gestures));
        this.$emit("toast", processed_gestures);
      } else {
        this.setParentText("No gestures caught from this event.", "");
      }
    },
    captureEvent(event, type) {
      let {x, y} = this.eventToCanvasCoordinate(event);
      this.capturedEvents.push(
          `${type},${x},${y},0:00:${(event.timeStamp / 1000).toFixed(6)},${event.width},${event.height},${event.pressure}`
      )
    },
    dumpCaptured() {
      let filename = "capture.csv";

      let contents = `${this.capturedEvents.join("\n")}\n`;

      let element = document.createElement('a');
      element.setAttribute('href', 'data:text/plain;charset=utf-8,' + encodeURIComponent(contents));
      element.setAttribute('download', filename);

      element.style.display = 'none';
      document.body.appendChild(element);

      element.click();

      document.body.removeChild(element);
      this.initCapture();
    },
  }
}
</script>
