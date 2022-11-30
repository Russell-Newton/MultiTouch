<script setup>
import HelloWorld from './components/HelloWorld.vue'
import TheWelcome from './components/TheWelcome.vue'
import MyNavbar from "@/components/MyNavbar.vue";
import Logo from "@/components/icons/Logo.vue";
</script>

<template>
  <header>
    <Logo height="125" class="logo"/>

    <div class="wrapper">
      <HelloWorld msg="GT Team 2135 x L3Harris Gesture Library" />
    </div>
  </header>

  <main>
    <MyNavbar />
    <TheWelcome />
  </main>
</template>

<style scoped>
header {
  line-height: 1.5;
  align-content: center;
}

.logo {
  display: block;
  margin-top: 7.5vh;
  margin-right: 2rem;
  margin-bottom: 2rem;
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
