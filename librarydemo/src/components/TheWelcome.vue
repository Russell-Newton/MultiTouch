<script setup>
import WelcomeItem from './WelcomeItem.vue'
import DocumentationIcon from './icons/IconDocumentation.vue'
import ToolingIcon from './icons/IconTooling.vue'
import EcosystemIcon from './icons/IconEcosystem.vue'
import CommunityIcon from './icons/IconCommunity.vue'
import SupportIcon from './icons/IconSupport.vue'
import { MDBBtn, MDBIcon } from "mdb-vue-ui-kit";
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
      <a href="">
        <MDBBtn color="dark">Home</MDBBtn>
      </a>
      <a href="./demo/index.html">
        <MDBBtn color="dark">Demo</MDBBtn>
      </a>
      <a href="./docs">
        <MDBBtn color="dark">Documentation</MDBBtn>
      </a>
      <MDBBtn tag="a" color="dark" href="https://github.com/Russell-Newton/Multitouch" style="background-color: rgb(51, 51, 51);">
      <MDBIcon iconStyle="fab" icon="github"></MDBIcon>
    </MDBBtn>
  </MDBNavbar>
</MDBContainer>
    <!-- Navbar -->
  </header>

  <WelcomeItem>
    <template #icon>
      <DocumentationIcon />
    </template>
    <template #heading>About</template>
    
    Our gesture library features a variety of common gestures with up to 5-finger capabilities. Each of these gestures are completely customizable
    using the library and can be incorporated into any application.

    <br />

    More instructions are available in <code>README.md</code>.
    
  </WelcomeItem>

  <WelcomeItem>
    <template #icon>
      <ToolingIcon />
    </template>
    <template #heading>Github</template>
    Link to our 
    <a href="https://github.com/Russell-Newton/Team-2135-Graphics-Applications-for-Transparent-Electronic-Displays" target="_blank" rel="noopener">Github</a>
    page.
    
  </WelcomeItem>

  <WelcomeItem>
    <template #icon>
      <EcosystemIcon />
    </template>
    
    <a href="./demo/index.html">
      <MDBBtn color="primary" size="sm">Demo</MDBBtn>
    </a>
    Test out different gestures.
  </WelcomeItem>

  <WelcomeItem>
    <template #icon>
      <CommunityIcon />
    </template>
    <template #heading>Documentation</template>
    Link to  <a href="./docs">Documentation.</a>
    
  </WelcomeItem>

  <WelcomeItem>
    <template #icon>
      <SupportIcon />
    </template>
    <template #heading>Contact</template>

    Contact Team 2135 for any questions or comments:
    <br />
    dcho71@gatech.edu
  </WelcomeItem>
</template>
