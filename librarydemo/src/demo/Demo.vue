<script setup>
import GestureCanvas from './components/GestureCanvas.vue'
import { MDBBtn } from "mdb-vue-ui-kit";
import {test, testStruct} from "./assets/api_wrapper";
</script>

<template>
  <div class="container">
    <div class="row">
      <div class="col-md">
        <header>
          <a href="../index.html">
            <MDBBtn color="light" size="sm" rounded>Home</MDBBtn>
          </a>
          <MDBBtn @click="test" color="light" size="sm" rounded>Square 3 in C! (Check console output)</MDBBtn>
          <MDBBtn @click="testStruct" color="light" size="sm" rounded>C Struct test</MDBBtn>
        </header>
      </div>
    <div class="col-md">
      <main>
        <GestureCanvas width="500" height="500"/>
      </main>
    </div>
    <div class="col-md">
      Click to Change Text:
      <MDBBtn @click="setText('Text A')" color="light" size="sm" rounded>A</MDBBtn>
      <MDBBtn @click="setText('Text B')" color="light" size="sm" rounded>B</MDBBtn>
      <h3 >
        {{ myText }}
      </h3>
    </div>
  </div>
</div>
  
  

  
  
  
</template>

<script>
export default {
  data() {
    return {
      myText: "Hello World!"
    }
  },
  methods: {
    setText(newText) {
      this.myText = newText;
    }
  }
}
</script>

