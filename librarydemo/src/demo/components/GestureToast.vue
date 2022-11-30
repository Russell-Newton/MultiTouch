<template>
  <div>
    {{ `${this.eventType} ${this.gestureData[this.gestureData.length - 1].data.state}` }}
  </div>
</template>

<script>

function redBlueLerp(min, max, alpha) {
  if (max - min < 1e-2) {
    return `rgba(0, 0, 255, 0.7)`;
  }
  let red = (max - alpha) / (max - min);
  let blue = 1 - red;
  return `rgba(${255 * red}, 0, ${255 * blue}, 0.7)`;
}

export default {
  name: "GestureToast",
  data() {
    return {
      displayText: "",
      TRACE_GENERATORS: {
        "Tap": this.generateScatterTrace,
        "Hold": this.generateScatterTrace,
        "Hold & Drag": this.generateScatterTrace,
        "Double-Tap": this.generateScatterTrace,
        "Drag": this.generateScatterTrace,
        "Zoom": (gestures) => this.generateFieldTrace(gestures, "scale"),
        "Rotate": (gestures) => this.generateFieldTrace(gestures, "rotation"),
      }
    }
  },
  emits: ["showModal"],
  mounted() {
    this.$el.addEventListener("click", this.emitPlotlyPayload);
  },
  props: {
    gestureData: Array,
    eventType: String,
    plotMaxX: Number,
    plotMaxY: Number,
  },
  methods: {
    emitPlotlyPayload() {
      let payload = this.TRACE_GENERATORS[this.eventType](this.gestureData);

      this.$emit("showModal", payload);
    },
    generateScatterTrace(gestures) {
  let data = [];
  let colors = [];

  for (let gesture of gestures) {
    data.push({x: gesture.data.x, y: gesture.data.y});
    colors.push(redBlueLerp(gestures[0].t, gestures[gestures.length - 1].t, gesture.t));
  }

  return {
    data: {
      datasets: [
        {
          data: data,
          backgroundColor: colors
        }
      ]
    },
    options: {
      scales: {
        x: {
          min: 0,
          max: this.plotMaxX
        },
        y: {
          min: 0,
          max: this.plotMaxY
        }
      },
      plugins: {
        legend: {
          display: false
        }
      }
    }
  };
},
    generateFieldTrace(gestures, field) {
  let data = [];
  let colors = [];

  for (let gesture of gestures) {
    data.push({x: gesture.t, y: gesture.data[field]});
    colors.push(redBlueLerp(gestures[0].t, gestures[gestures.length - 1].t, gesture.t));
  }

  return {
    data: {
      datasets: [
        {
          data: data,
          backgroundColor: colors
        }
      ]
    },
    options: {
      plugins: {
        legend: {
          display: false
        }
      }
    }
  };
}
  },
}
</script>

<style scoped>

</style>