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
  MDBNavbarBrand,
  MDBNavbarToggler,
  MDBNavbarNav,
  MDBNavbarItem
} from 'mdb-vue-ui-kit';
</script>

<template>

  <WelcomeItem>
    <template #icon>
      <DocumentationIcon />
    </template>
    <template #heading>About</template>

    MultiTouch is a low-level touch gesture recognition library written in C for use on transparent displays in aircraft.

    <br />
    
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
    
<!--    <a href="./demo/index.html">-->
<!--      <MDBBtn color="primary" size="sm">Demo</MDBBtn>-->
<!--    </a>-->
    Test out different gestures in an <a href="./demo/index.html">interactive demo</a>. To accomplish this, our library has been compiled into
    WebAssembly and JavaScript with Emscripten.
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
