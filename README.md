# MultiTouch
<!-- ALL-CONTRIBUTORS-BADGE:START - Do not remove or modify this section -->
[![All Contributors](https://img.shields.io/badge/all_contributors-1-orange.svg?style=flat-square)](#contributors-)
<!-- ALL-CONTRIBUTORS-BADGE:END -->
[![Deploy Artifacts to GitHub Pages](https://github.com/Russell-Newton/MultiTouch/actions/workflows/pages_deploy.yml/badge.svg)](https://russell-newton.github.io/MultiTouch/)
[![Checks: pre-commit](https://github.com/Russell-Newton/MultiTouch/actions/workflows/pre-commit.yml/badge.svg)](https://github.com/Russell-Newton/MultiTouch/actions/workflows/pre-commit.yml)
[![Unit Tests](https://github.com/Russell-Newton/MultiTouch/actions/workflows/run_tests.yml/badge.svg)](https://github.com/Russell-Newton/MultiTouch/actions/workflows/run_tests.yml)

A lightweight, robust touch gesture recognition library created in C for use within aircraft.

# Demo and Documentation
Demo web app automatically deployed to [GitHub Pages](https://russell-newton.github.io/MultiTouch/).

Documentation automatically deployed to [GitHub Pages](https://russell-newton.github.io/MultiTouch/docs).

Data collection web app automatically deployed to [GitHub Pages](https://russell-newton.github.io/MultiTouch/data-collection)

# Release Notes

## Version 0.4.0
### New Features
* Zoom and Rotate split into their own gestures
* Removed swipe gesture
* Finished implementing gestures: tap, double tap, hold, hold and drag
* Demo page updates:
  * Links back to home page
  * Communicates with library using new listener structure
  * GestureCanvas component now sets display text within Demo component
  * Folder structure overhauled

### Bug Fixes
* Zoom and rotate gestures work with more than 2 fingers

### Known Issues
* Zoom and rotate gesture occasionally marked as complete on the demo page when a single drag has been performed
* Multi-finger double tap tests failing for unknown reason

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
  * Demo app: deployed to [root endpoint](https://russell-newton.github.io/MultiTouch/)
  * Documentation: deployed to [/docs endpoint](https://russell-newton.github.io/MultiTouch/docs)
  * Data collection: deployed to [/data-collection endpoint](https://russell-newton.github.io/MultiTouch/data-collection)
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

## Contributors ✨

Thanks goes to these wonderful people ([emoji key](https://allcontributors.org/docs/en/emoji-key)):

<!-- ALL-CONTRIBUTORS-LIST:START - Do not remove or modify this section -->
<!-- prettier-ignore-start -->
<!-- markdownlint-disable -->
<table>
  <tbody>
    <tr>
      <td align="center"><a href="https://www.linkedin.com/in/russellnewton01/"><img src="https://avatars.githubusercontent.com/u/18690062?v=4?s=100" width="100px;" alt="Russell Newton"/><br /><sub><b>Russell Newton</b></sub></a><br /><a href="https://github.com/Russell-Newton/MultiTouch/commits?author=Russell-Newton" title="Code">💻</a> <a href="https://github.com/Russell-Newton/MultiTouch/commits?author=Russell-Newton" title="Documentation">📖</a> <a href="#infra-Russell-Newton" title="Infrastructure (Hosting, Build-Tools, etc)">🚇</a> <a href="#maintenance-Russell-Newton" title="Maintenance">🚧</a></td>
    </tr>
  </tbody>
</table>

<!-- markdownlint-restore -->
<!-- prettier-ignore-end -->

<!-- ALL-CONTRIBUTORS-LIST:END -->

This project follows the [all-contributors](https://github.com/all-contributors/all-contributors) specification. Contributions of any kind welcome!