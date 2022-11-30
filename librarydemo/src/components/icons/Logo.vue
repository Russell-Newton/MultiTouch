<template>
  <div id="logo">
    <picture v-bind:hidden="forceBlack || forceWhite">
      <source srcset="/click-2384-white.svg" media="(prefers-color-scheme:dark)">
      <img src="/click-2384-black.svg" v-bind:height="height">
    </picture>
    <img src="/click-2384-white.svg" v-bind:hidden="!forceWhite" v-bind:height="height">
    <img src="/click-2384-black.svg" v-bind:hidden="!forceBlack" v-bind:height="height">
  </div>
</template>

<style scoped>
#logo {
  text-align: center;
}
</style>

<script>
export default {
  name: "Logo",
  props: {
    height: Number,
    forceBlack: Boolean,
    forceWhite: Boolean,
  }
}
</script>
