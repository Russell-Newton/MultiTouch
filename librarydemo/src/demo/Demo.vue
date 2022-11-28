<script setup>
import GestureCanvas from './components/GestureCanvas.vue'
import { MDBBtn, MDBIcon, MDBCol, MDBRow, MDBContainer } from "mdb-vue-ui-kit";
import {test, testStruct} from "./assets/api_wrapper";
import {
  MDBNavbar,
  MDBNavbarToggler,
  MDBNavbarNav,
  MDBNavbarItem
} from 'mdb-vue-ui-kit';
</script>

<template>
   <header>
    <!-- Navbar -->
    <MDBContainer fluid> 
      <MDBNavbar position="top" light bg="primary" container classContainer="justify-content-start">
        <img
        src="https://icons-for-free.com/iconfiles/png/512/finger+gesture+hand+one+icon-1320183619867359581.png"
        height="30"
        alt=""
        loading="lazy"
      />
      &nbsp;
      <h3>
      <span class="navbar-text"> Multitouch </span>
      &nbsp;
    </h3>
        <a href="../index.html">
          <MDBBtn color="dark">Home</MDBBtn>
        </a>
        <a href="">
          <MDBBtn color="dark">Demo</MDBBtn>
        </a>
        <a href="../docs">
          <MDBBtn color="dark">Documentation</MDBBtn>
        </a>
        <MDBBtn tag="a" color="dark" href="https://github.com/Russell-Newton/Multitouch" style="background-color: rgb(51, 51, 51);">
        <MDBIcon iconStyle="fab" icon="github"></MDBIcon>
      </MDBBtn>
      </MDBNavbar>
    </MDBContainer>
    <!-- Navbar -->
  </header>
    <MDBContainer xxl>
        <MDBRow>
            <MDBCol md="3">
              <div class="p-3 mb-2 bg-primary bg-gradient text-white">
              <h3>
                Multitouch Gesture Library Demo
              </h3>
              Perform any gesture from our library on the canvas and export gesture data with the button below.
            </div>
            <div class="p-3 mb-2 bg-primary bg-gradient text-white">
              Gestures in our Library:
              <li> Gesture </li>
              <li> Gesture </li>
              <li> Gesture </li>
            </div>
            </MDBCol>
            <MDBCol md="6" class="text-center">
                <GestureCanvas :setParentText="setText" :setDumpData="setDumpData"/>
                <br />
              <MDBBtn @click="runDumpData" color="primary" size="sm">Dump Data</MDBBtn>
            </MDBCol>
            <MDBCol md="3">
                <div class="p-3 mb-2 bg-primary bg-gradient text-white">
                    <h3 style="white-space: pre-wrap">
                        {{ myText }}
                    </h3>
                </div>
            </MDBCol>
        </MDBRow>
    </MDBContainer>
    <MDBContainer fluid> 
      <MDBNavbar position="bottom" light bg="primary" container classContainer="justify-content-start">
        <span class="navbar-text"> Gesture Library created by Russell Newton, Leo Wang, Iftek Kharim, and Jackson Dike. Demo page created by Deborah Cho. </span>
      </MDBNavbar>
    </MDBContainer>
</template>

<script >
export default {
  data() {
    return {
      myText: "Perform a Gesture",
      dumpData: null
    }
  },
  methods: {
    setText(newText) {
      this.myText = newText;
    },
    setTextSquare3() {
      this.setText(Module._square(3));
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



