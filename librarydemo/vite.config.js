import vue from "@vitejs/plugin-vue"
import glob from "glob"
import {fileURLToPath, URL} from "node:url"
import path from "path"
import {defineConfig} from "vite"
import wasm from "vite-plugin-wasm"

// https://vitejs.dev/config/
export default defineConfig({
  base : "./",
  root : path.join(__dirname, "src"),
  publicDir : path.join(__dirname, "public"),
  plugins : [ vue(), wasm() ],
  build : {
    outDir : path.join(__dirname, "dist"),
    rollupOptions :
        {input : glob.sync(path.resolve(__dirname, "src/**/", "*.html"))}
  },
  resolve : {alias : {'@' : fileURLToPath(new URL('./src', import.meta.url))}}
})
