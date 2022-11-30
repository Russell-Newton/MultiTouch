<script setup>
import {
  MDBBtn,
  MDBIcon,
  MDBContainer,
  MDBNavbar,
  MDBNavbarBrand,
} from 'mdb-vue-ui-kit';
import Logo from "@/components/icons/Logo.vue";
</script>

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
        <MDBBtn tag="a" color="dark" href="/index.html">Home</MDBBtn>

        <MDBBtn tag="a" color="dark" href="/demo/index.html">Demo</MDBBtn>

        <div class="d-none d-md-inline">
          <MDBBtn tag="a" color="dark" href="/docs">Documentation</MDBBtn>
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
export default {
  name: "MyNavbar"
}
</script>

<style scoped>

</style>