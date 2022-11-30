<template>
  <header>
    <MDBContainer fluid>
      <MDBNavbar position="top" dark bg="primary" container classContainer="justify-content-start">
        <Logo height="30" force-white/>
          &nbsp;
        <div class="d-none d-md-inline">
          <MDBNavbarBrand>Multitouch</MDBNavbarBrand>
        </div>
        &nbsp;
        <MDBBtn tag="a" color="dark" :href="homeRef">Home</MDBBtn>

        <MDBBtn tag="a" color="dark" :href="demoRef">Demo</MDBBtn>

        <div class="d-none d-md-inline">
          <MDBBtn tag="a" color="dark" :href="docsRef">Documentation</MDBBtn>
        </div>

        <div class="d-none d-md-inline">
          <MDBBtn tag="a" color="dark" href="https://github.com/Russell-Newton/Multitouch">
            <MDBIcon iconStyle="fab" icon="github" />
          </MDBBtn>
        </div>
      </MDBNavbar>
    </MDBContainer>
  </header>
  <MDBContainer fluid class="d-none d-md-inline">
    <MDBNavbar position="bottom" dark bg="primary" container classContainer="justify-content-start">
      <span class="navbar-text">
        Gesture Library created by Russell Newton, Leo Wang, Iftek Kharim, and Jackson Dike. Demo page created by Deborah Cho.
      </span>
    </MDBNavbar>
  </MDBContainer>
</template>

<script>
import {
  MDBBtn,
  MDBIcon,
  MDBContainer,
  MDBNavbar,
  MDBNavbarBrand,
} from 'mdb-vue-ui-kit';
import Logo from "@/components/icons/Logo.vue";
export default {
  name: "MyNavbar",
  components: {
    MDBBtn,
    MDBContainer,
    MDBIcon,
    MDBNavbar,
    MDBNavbarBrand,
    Logo,
  },
  props: {
    base: String,
  },
  data() {
    return {
      homeRef: "/index.html",
      demoRef: "/demo/index.html",
      docsRef: "/docs",
    }
  },
  mounted() {
    if (!this.base) {
      this.base = ".";
    }
    this.homeRef = this.base + this.homeRef;
    this.demoRef = this.base + this.demoRef;
    this.docsRef = this.base + this.docsRef;
  }
}
</script>

<style scoped>

</style>