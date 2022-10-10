# Library demo

[Vue Quick Start Guide](https://vuejs.org/guide/quick-start.html#creating-a-vue-application)

## Prereqs:

1. Install node.js ver 16+
2. In cmd or terminal within the `librarydemo` directory:
```sh
npm install
```

## Emscripten:
In order for emscripten to work (required for `dev` and `build` scripts), you may have to follow some additional setup steps:

1. Install python
2. Provide a location for the EMSDK (emscripten software development kit):

On Windows
```sh
npm config set emsdk %UserProfile%\.emsdk
```
On Linux
```sh
npm config set emsdk /usr/local/emsdk
```

## Development:

In order to see changes made live, run
```sh
npm run dev
```

In order to see a built version of the page, run
```sh
npm run build:run
```
This will run a build of the website, creating HTML, JS, and CSS from the Vue project, and will serve it with live-server.


**Both of these commands also build the C code living in `c_bindings`. The build code is put into `public/scripts`.**

You may need to specify a path to install emsdk (the emscripten SDK). If this is the case, it should suggest some commands to fix the problem.
