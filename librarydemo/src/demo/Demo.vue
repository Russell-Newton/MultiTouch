<script setup>
import GestureCanvas from './components/GestureCanvas.vue'
import {MDBBtn, MDBIcon, MDBCol, MDBRow, MDBContainer} from "mdb-vue-ui-kit";
import {test, testStruct} from "./assets/api_wrapper";
import {
  MDBNavbar,
  MDBNavbarBrand,
  MDBNavbarToggler,
  MDBNavbarNav,
  MDBNavbarItem
} from 'mdb-vue-ui-kit';
</script>

<template>
  <header>
    <!-- Navbar -->
    <MDBContainer fluid>
      <MDBNavbar position="top" dark bg="primary" container classContainer="justify-content-start">
        <img
            src="https://icons-for-free.com/iconfiles/png/512/finger+gesture+hand+one+icon-1320183619867359581.png"
            height="30"
            alt=""
            loading="lazy"
        />
        &nbsp;
        <MDBNavbarBrand>Multitouch</MDBNavbarBrand>
        &nbsp;
        <a href="../index.html">
          <MDBBtn color="dark">Home</MDBBtn>
        </a>
        <a href="">
          <MDBBtn color="dark">Demo</MDBBtn>
        </a>
        <a href="../docs">
          <MDBBtn color="dark">Documentation</MDBBtn>
        </a>
        <MDBBtn tag="a" color="dark" href="https://github.com/Russell-Newton/Multitouch"
                style="background-color: rgb(51, 51, 51);">
          <MDBIcon iconStyle="fab" icon="github"></MDBIcon>
        </MDBBtn>
      </MDBNavbar>
    </MDBContainer>
    <!-- Navbar -->
  </header>
  <MDBContainer fluid style="height: 80vh; max-height: 80vh; max-width: 1600px; overflow-y: auto; vertical-align: center">
    <MDBRow>
      <MDBCol md="3" class="d-none d-md-table-cell">
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
      <MDBCol md="6" class="text-center">
        <GestureCanvas :setParentText="setText" :setDumpData="setDumpData" :width="300" :height="300"/>
        <br/>
        <MDBBtn @click="runDumpData" color="primary" size="sm">Dump Data</MDBBtn>
      </MDBCol>
      <MDBCol md="3" class="d-md-none">
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
      <MDBCol md="3" class="order-last">
        <div class="p-3 mb-2 bg-primary bg-gradient text-white">
          <p style="white-space: pre-wrap">
            {{ advancedText }}
          </p>
        </div>
      </MDBCol>
    </MDBRow>
  </MDBContainer>
  <MDBContainer fluid>
    <MDBNavbar position="bottom" dark bg="primary" container classContainer="justify-content-start">
      <span class="navbar-text"> Gesture Library created by Russell Newton, Leo Wang, Iftek Kharim, and Jackson Dike. Demo page created by Deborah Cho. </span>
    </MDBNavbar>
  </MDBContainer>
</template>

<script>
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



