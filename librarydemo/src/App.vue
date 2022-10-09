<script setup>
import HelloWorld from './components/HelloWorld.vue'
import TheWelcome from './components/TheWelcome.vue'
import { MDBBtn } from "mdb-vue-ui-kit";
</script>

<template>
  <header>
  <img alt="Vue logo" class="logo" src="./assets/gesture.png" width="125" height="125" />

    <div class="wrapper">
      <HelloWorld msg="GT Team 2135 x L3Harris Gesture Library" />
    </div>
  </header>

  <main>
    <TheWelcome />
    <MDBBtn @click="test()" color="light" size="sm" rounded>Square 3 in C! (Check console output)</MDBBtn>
  </main>
</template>

<style scoped>
header {
  line-height: 1.5;
}

.logo {
  display: block;
  margin: 0 auto 2rem;
}

@media (min-width: 1024px) {
  header {
    display: flex;
    place-items: center;
    padding-right: calc(var(--section-gap) / 2);
  }

  .logo {
    margin: 0 2rem 0 0;
  }

  header .wrapper {
    display: flex;
    place-items: flex-start;
    flex-wrap: wrap;
  }
}
#app {
  font-family: Roboto, Helvetica, Arial, sans-serif;
}
</style>

<script>
  export default {
    methods: {
      test: function() {
        let square = Module.cwrap('square', 'number', ['number']);
        console.log(square(3));
      }
    }
  }
</script>
