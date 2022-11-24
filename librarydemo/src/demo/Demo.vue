<script setup>
import GestureCanvas from './components/GestureCanvas.vue'
import { MDBBtn, MDBCol, MDBRow, MDBContainer } from "mdb-vue-ui-kit";
import {test, testStruct} from "./assets/api_wrapper";
</script>

<template>
    <h1 class="text-center">MultiTouch Demo</h1>
    <MDBContainer xxl style="height: 90vh; max-height: 90vh; overflow-y: auto; vertical-align: center">
        <MDBRow>
            <MDBCol md="3" class="d-none d-md-table-cell">
                <div class="p-3">
                    <a href="../index.html">
                        <MDBBtn color="light" size="sm" rounded>Home</MDBBtn>
                    </a>
                    <h3 style="white-space: pre-wrap">
                      {{ simpleText }}
                    </h3>
                </div>
            </MDBCol>
            <MDBCol md="6" class="text-center">
                <GestureCanvas :setParentText="setText" :setDumpData="setDumpData" :width="300" :height="300"/>
                <br />
              <MDBBtn @click="runDumpData" color="light" size="sm" rounded>Dump Data</MDBBtn>
            </MDBCol>
            <MDBCol md="3" class="d-md-none">
              <div class="p-3">
                <a href="../index.html">
                  <MDBBtn color="light" size="sm" rounded>Home</MDBBtn>
                </a>
                <h3 style="white-space: pre-wrap">
                  {{ simpleText }}
                </h3>
              </div>
            </MDBCol>
            <MDBCol md="3" class="order-last">
                <div class="p-3">
                  <p style="white-space: pre-wrap">
                    {{ advancedText }}
                  </p>
                </div>
            </MDBCol>
        </MDBRow>
    </MDBContainer>
</template>

<script >
export default {
  data() {
    return {
      simpleText: "Perform a Gesture",
      advancedText: "",
      dumpData: null
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
    }
  }
}
</script>



