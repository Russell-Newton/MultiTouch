<script setup>
import GestureCanvas from './components/GestureCanvas.vue'
import { MDBBtn } from "mdb-vue-ui-kit";
import {test, testStruct} from "./assets/api_wrapper";
</script>

<template>
  <header>
    <MDBBtn @click="test" color="light" size="sm" rounded>Square 3 in C! (Check console output)</MDBBtn>
    <MDBBtn @click="testStruct" color="light" size="sm" rounded>C Struct test</MDBBtn>
  </header>
  
  <main>
    <GestureCanvas width="500" height="500"/>
  </main>
</template>
