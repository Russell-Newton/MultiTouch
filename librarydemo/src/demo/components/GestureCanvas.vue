<script setup>
import {convertTouchEvent, convertGestureEvent} from "../assets/api_wrapper";
</script>

<template>
  <canvas v-bind:width="width" v-bind:height="height"
          style="touch-action: none"
          @pointerdown.prevent.stop="pointerdown" @pointerup="pointerup" @pointermove="pointermove">
  </canvas>
</template>

<script>
export default {
    data() {
      return {
          dragging: {}
      }  
    },
    props: {
        width: Number,
        height: Number,
    },
    mounted() {
      let canvas = this.$el;
      let ctx = canvas.getContext("2d");
      ctx.globalCompositeOperation = 'destination-over';
      ctx.fillStyle = "lightgrey";
      ctx.fillRect(0, 0, canvas.width, canvas.height);  
    },
    methods: {
        eventToCanvasCoordinate(event) {
            // need to use getBoundingClientRect for responsive <canvas> sizing
            // NOTE: if you use transforms, see
            // https://stackoverflow.com/a/59259174
            // to invert transform matrix
            let canvas = this.$el;
            let bounds = canvas.getBoundingClientRect();
            let x = event.x - bounds.left;
            let y = event.y - bounds.top;
            x = x / bounds.width * canvas.width;
            y = y / bounds.height * canvas.height;
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
            let {x, y} = this.eventToCanvasCoordinate(event);
            let t = performance.now() / 1000;   // in ms by default
            console.log(`Caught ${type} touch_event at time ${t}: (${x}, ${y})`);
        }
    }
}
</script>
