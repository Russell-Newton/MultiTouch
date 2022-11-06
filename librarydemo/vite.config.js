import vue from '@vitejs/plugin-vue'
import {fileURLToPath, URL} from 'node:url'
import {defineConfig} from 'vite'
import wasm from "vite-plugin-wasm"

// https://vitejs.dev/config/
export default defineConfig({
  base : '',
  plugins : [ vue(), wasm() ],
  resolve : {alias : {'@' : fileURLToPath(new URL('./src', import.meta.url))}}
})
