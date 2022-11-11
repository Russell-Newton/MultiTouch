# MultiTouch
[![Deploy Artifacts to GitHub Pages](https://github.com/Russell-Newton/MultiTouch/actions/workflows/pages_deploy.yml/badge.svg)](https://russell-newton.github.io/Team-2135-Graphics-Applications-for-Transparent-Electronic-Displays/)
[![Checks: pre-commit](https://github.com/Russell-Newton/MultiTouch/actions/workflows/pre-commit.yml/badge.svg)](https://github.com/Russell-Newton/MultiTouch/actions/workflows/pre-commit.yml)
[![Unit Tests](https://github.com/Russell-Newton/MultiTouch/actions/workflows/run_tests.yml/badge.svg)](https://github.com/Russell-Newton/MultiTouch/actions/workflows/run_tests.yml)

A lightweight, robust touch gesture recognition library created in C for use within aircraft.

# Demo and Documentation
Demo web app automatically deployed to [GitHub Pages](https://russell-newton.github.io/Team-2135-Graphics-Applications-for-Transparent-Electronic-Displays/).

Documentation automatically deployed to [GitHub Pages](https://russell-newton.github.io/Team-2135-Graphics-Applications-for-Transparent-Electronic-Displays/docs).

Data collection web app automatically deployed to [GitHub Pages](https://russell-newton.github.io/Team-2135-Graphics-Applications-for-Transparent-Electronic-Displays/data-collection)

# Release Notes

## Version 0.3.0
### New Features
* Functioning swipe and drag gestures
* Minimally functioning zoom and rotate gesture
* Gesture library compiles into .js and .wasm with emscripten
  * Functions exposed by Module object to pack and unpack library structs without needing heap DMA

### Bug Fixes
* Faulty unit tests removed

### Known Issues
* Zoom and rotate gesture only works with 2 fingers
* 3+ finger zoom and rotate is planned for next sprint

## Version 0.2.1
### New Features
* Included Flutter project to collect example gesture `.csv` data.
* Pages deploy workflow modified to deploy web artifacts
  * Demo app: deployed to [root endpoint](https://russell-newton.github.io/Team-2135-Graphics-Applications-for-Transparent-Electronic-Displays/)
  * Documentation: deployed to [/docs endpoint](https://russell-newton.github.io/Team-2135-Graphics-Applications-for-Transparent-Electronic-Displays/docs)
  * Data collection: deployed to [/data-collection endpoint](https://russell-newton.github.io/Team-2135-Graphics-Applications-for-Transparent-Electronic-Displays/data-collection)
### Known Issues
* Some unit tests SEGFAULT. These have been commented so the unit test workflow passes.

## Version 0.2.0
### New Features
* Framework for recognizer files (header and c files) created
* File organization updated
* Doxygen document generator linked to library
* Vue project environment set up
* Demo webapp front landing page created
* GitHub Actions workflow created to generate and deploy Doxygen documentation from [doxygen-config](doxygen-config)
* Created prebuild and predev npm scripts that compile C code with Emscripten, allowing for use in the webapp
* Created `build:run` npm script that runs `npm run build` and `live-server`
### Bug Fixes
N/A

## Version 0.1.0
### New Features
* Sprint 1 limited to research, no features created
* Project is buildable
### Bug Fixes
N/A
### Known Issues
N/A

### Research Done
* Specified input/output format for data
* Specified library and architecture structure
