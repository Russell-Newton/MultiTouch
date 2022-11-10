<script setup>
import GestureCanvas from './components/GestureCanvas.vue'
import { MDBBtn, MDBCol, MDBRow, MDBContainer } from "mdb-vue-ui-kit";
import {test, testStruct} from "./assets/api_wrapper";
</script>

<template>
    <MDBContainer xxl>
        <MDBRow>
            <MDBCol md="3">
                <div class="p-3">
                    <a href="../index.html">
                        <MDBBtn color="light" size="sm" rounded>Home</MDBBtn>
                    </a>
                    <br />
                    <MDBBtn @click="test" color="light" size="sm" rounded>Square 3 in C! (Check console output)</MDBBtn>
                    <br />
                    <MDBBtn @click="testStruct" color="light" size="sm" rounded>C Struct test</MDBBtn>
                </div>
            </MDBCol>
            <MDBCol md="6" class="text-center">
                <GestureCanvas :setParentText="setText" />
            </MDBCol>
            <MDBCol md="3">
                <div class="p-3">
                    Click to Change Text:
                    <MDBBtn @click="setTextSquare3()" color="light" size="sm" rounded>Square 3 in C!</MDBBtn>
                    <MDBBtn @click="setText('Gesture A')" color="light" size="sm" rounded>A</MDBBtn>
                    <MDBBtn @click="setText('Gesture B')" color="light" size="sm" rounded>B</MDBBtn>
                    <h3 style="white-space: pre-wrap">
                        {{ myText }}
                    </h3>
                </div>
            </MDBCol>
        </MDBRow>
    </MDBContainer>
</template>

<script >
export default {
  data() {
    return {
      myText: "Perform a Gesture"
    }
  },
  methods: {
    setText(newText) {
      this.myText = newText;
    },
    setTextSquare3() {
      this.setText(Module._square(3));
    }
  }
}
</script>



