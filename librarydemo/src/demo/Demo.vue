<template>
  <MyNavbar base=".."></MyNavbar>
  <MDBContainer fluid
                style="height: fit-content; max-height: fit-content; max-width: 1500px; overflow-y: hidden; position: fixed; top: 7.5vh; left: 50%; transform: translate(-50%, 0%);"
  >
    <MDBRow>
      <div class="p-3 mb-2 bg-primary bg-gradient text-white">
        <h3>
          Multitouch Gesture Library Demo
        </h3>
        Perform gestures in the box, then tap a toast to see the gesture data plotted out!
      </div>
    </MDBRow>
    <MDBRow>
      <MDBCol md="3" class="d-none d-md-table-cell" ref="leftColumn">
        <div class="d-none p-3 mb-2 bg-primary bg-gradient text-white">
          <p style="white-space: pre-wrap">
            {{ simpleText }}
          </p>
        </div>
      </MDBCol>
      <MDBCol md="6" class="text-center">
        <GestureCanvas :setParentText="setText" :setDumpData="setDumpData" :width="canvasWidth" :height="canvasHeight"
                       @toast="makeToasts"/>
        <br/>
<!--        <MDBBtn @click="runDumpData" color="primary" size="sm" style="margin-bottom: 8px">Dump Data</MDBBtn>-->
        <div style="text-align: left">
          <MDBSwitch label="Clear failures fast" v-model="clearFailures" />
        </div>
      </MDBCol>
      <MDBCol md="3" class="d-none d-md-none">
        <div class="p-3 mb-2 bg-primary bg-gradient text-white">
          <h3>
            Multitouch Gesture Library Demo
          </h3>
          Perform any gesture from our library on the canvas and export gesture data with the button below.
        </div>
        <div class="p-3 mb-2 bg-primary bg-gradient text-white">
          <p style="white-space: pre-wrap">
            {{ simpleText }}
          </p>
        </div>
      </MDBCol>
      <MDBCol md="3" class="d-none order-last">
        <div class="p-3 mb-2 bg-primary bg-gradient text-white">
          <p style="white-space: pre-wrap">
            {{ advancedText }}
          </p>
        </div>
      </MDBCol>
    </MDBRow>
  </MDBContainer>

  <MDBModal id="dataModal" tabindex="-1" v-model="dataModal" @show="freezeToasts" @hidden="unfreezeToasts">
    <MDBModalBody>
      <Scatter
          :chart-data="chartData"
          :chart-options="chartOptions"
      />
    </MDBModalBody>
  </MDBModal>
</template>

<style scoped>

</style>

<script>
import GestureCanvas from './components/GestureCanvas.vue'
import {
  MDBBtn,
  MDBCol,
  MDBRow,
  MDBContainer,
  MDBModal,
  MDBModalBody,
  MDBSwitch,
} from 'mdb-vue-ui-kit';
import {Scatter} from 'vue-chartjs';

import {
  Chart as ChartJS,
  Title,
  Tooltip,
  Legend,
  LineElement,
  CategoryScale,
  LinearScale,
  PointElement,
} from 'chart.js'
import MyNavbar from "@/components/MyNavbar.vue";
import GestureToast from "@/demo/components/GestureToast.vue";

import {ref} from "vue";
import {useToast} from "vue-toastification";

export default {
  components: {
    MDBBtn,
    MDBCol,
    MDBRow,
    MDBContainer,
    MDBModal,
    MDBModalBody,
    GestureCanvas,
    Scatter,
    MyNavbar,
    MDBSwitch,
  },
  setup() {
    ChartJS.register(
        Title,
        Tooltip,
        Legend,
        LineElement,
        CategoryScale,
        LinearScale,
        PointElement
    )

    const dataModal = ref(false);
    const chartData = ref({});
    const chartOptions = ref({});

    function updatePlotAndShow(payload) {
      chartData.value = payload.data;
      chartOptions.value = payload.options;

      dataModal.value = true;
    }

    const canvasWidth = 300;
    const canvasHeight = 300;

    const clearFailures = ref(true);

    return {dataModal, chartData, chartOptions, updatePlotAndShow, canvasWidth, canvasHeight, clearFailures};
  },
  data() {
    return {
      simpleText: "Perform a Gesture",
      advancedText: "",
      dumpData: null,
      toast: useToast(),
      trackedToasts: {},
      liveToasts: {},
      isMD: false,
    }
  },
  methods: {
    setText(newText) {
      let {simple: simple, advanced: advanced} = newText
      this.simpleText = simple;
      this.advancedText = advanced;
    },
    setDumpData(dumpData) {
      this.dumpData = dumpData;
    },
    runDumpData() {
      if (this.dumpData) {
        this.dumpData();
      }
    },
    freezeToasts() {
      for (let toast in this.liveToasts) {
        this.toast.update(toast, {
          content: this.liveToasts[toast],
          options: {
            timeout: false,
          }
        })
      }
    },
    unfreezeToasts() {
      for (let toast in this.liveToasts) {
        this.toast.update(toast, {
          content: this.liveToasts[toast],
          options: {
            timeout: this.isMD ? 2500 : 5000,
          }
        })
      }
    },
    makeToasts(processed_gestures) {
      let style = window.getComputedStyle(this.$refs.leftColumn.$el);
      if (style.display == "none" && !this.isMD) {
        this.isMD = true;
        this.toast.updateDefaults({
          maxToasts: 3,
          position: "bottom-center",
          timeout: 2500
        })
      } else if (style.display != "none" && this.isMD) {
        this.isMD = false;
        this.toast.updateDefaults({
          maxToasts: 20,
          position: "top-right",
          timeout: 5000,
        })
      }
      for (let gesture of processed_gestures) {
        if (["null", "possible", "failed"].includes(gesture.data.state) && !(gesture.ptr in this.trackedToasts)) {
          continue;
        }
        let id = -1;
        let data = [gesture];

        if (gesture.ptr in this.trackedToasts) {
          let tracked = this.trackedToasts[gesture.ptr]
          id = tracked.id;
          data = tracked.data.concat(data);
        }

        const content = {
          component: GestureToast,

          props: {
            gestureData: data,
            eventType: gesture.event,
            plotMaxX: this.canvasWidth,
            plotMaxY: this.canvasHeight,
          },

          listeners: {
            showModal: this.updatePlotAndShow
          }
        }

        let type = "info";
        switch (gesture.data.state) {
          case "completed":
            type = "success";
            break;
          case "failed":
            type = "error";
            break;
          default:
            type = "info";
        }

        if (id == -1) {
          id = this.toast(content, {
            type: type,
          });
        } else {
          this.toast.update(id, {
            content: content,
            options: {
              onClose: () => {
                delete this.trackedToasts[gesture.ptr];
                delete this.liveToasts[id]
              },
              timeout: this.isMD ? 2500 : 5000,
              type: type,
            }
          });
        }

        this.trackedToasts[gesture.ptr] = {
          id: id,
          data: data
        }
        this.liveToasts[id] = content;

        if (["completed", "failed"].includes(gesture.data.state)) {
          delete this.trackedToasts[gesture.ptr];
        }

        if (this.clearFailures && gesture.data.state == "failed") {
          this.toast.dismiss(id);
        }
      }
    },
  }
}
</script>



