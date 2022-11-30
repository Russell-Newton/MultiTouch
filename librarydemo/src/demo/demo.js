import 'mdb-vue-ui-kit/css/mdb.min.css';
import '../assets/main.css';
import "vue-toastification/dist/index.css";

import {createApp} from 'vue';
import Toast from 'vue-toastification';

import Demo from './Demo.vue';

const app = createApp(Demo);

const toastOptions = {
  position : "top-right",
  timeout : 5000,
  closeOnClick : false,
  pauseOnFocusLoss : true,
  pauseOnHover : true,
  draggable : true,
  draggablePercent : 0.33,
  showCloseButtonOnHover : true,
  hideProgressBar : false,
  closeButton : "button",
  icon : true,
  rtl : false,
  transition : "Vue-Toastification__slideBlurred",
  maxToasts : 20,
  newestOnTop : true
};

app.use(Toast, toastOptions);

app.mount('#demo');
