<template>
  <picture>
    <source v-bind:srcset="whiteSrc" media="(prefers-color-scheme:dark)">
    <img v-bind:src="blackSrc" v-bind:height="height">
  </picture>
</template>

<style scoped>
picture {
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
  },
  data() {
    return {
      blackSrc: "/click-2384-black.svg",
      whiteSrc: "/click-2384-white.svg",
    }
  },
  mounted() {
    if (this.forceBlack) {
      this.whiteSrc = this.blackSrc;
    } else if (this.forceWhite) {
      this.blackSrc = this.whiteSrc;
    }
  }
}
</script>
