# MultiTouch

<p align="center">
<img src="https://raw.githubusercontent.com/Russell-Newton/MultiTouch/main/librarydemo/public/click-2384-black.svg#gh-light-mode-only">
<img src="https://raw.githubusercontent.com/Russell-Newton/MultiTouch/main/librarydemo/public/click-2384-white.svg#gh-dark-mode-only">
</p>

[![Built with CMake](https://img.shields.io/badge/Built%20with-CMake-blue?style=flat-square&logo=cmake&color=064F8C)](#installation)
[![Demo Powered by Emscripten](https://img.shields.io/badge/Demo%20powered%20by-Emscripten-black?style=flat-square&logo=pytorch-lightning&color=b666d2)](https://russell-newton.github.io/MultiTouch/)
[![Docs generated by Doxygen](https://img.shields.io/badge/Docs%20generated%20with-Doxygen-black?style=flat-square&logo=read-the-docs&color=3d4ba2&logoColor=white)](https://russell-newton.github.io/MultiTouch/docs)

[![Deploy Artifacts to GitHub Pages](https://img.shields.io/github/deployments/Russell-Newton/MultiTouch/github-pages?label=Pages%20Deploy&logo=github&style=flat-square)](https://russell-newton.github.io/MultiTouch/)
[![Checks: pre-commit](https://img.shields.io/github/workflow/status/Russell-Newton/MultiTouch/Checks:%20pre-commit?label=pre-commit&logo=github&style=flat-square)](https://github.com/Russell-Newton/MultiTouch/actions/workflows/pre-commit.yml)
[![Unit Tests](https://img.shields.io/github/workflow/status/Russell-Newton/MultiTouch/Unit%20Tests?label=Unit%20Tests&logo=github&style=flat-square)](https://github.com/Russell-Newton/MultiTouch/actions/workflows/run_tests.yml)

<!-- ALL-CONTRIBUTORS-BADGE:START - Do not remove or modify this section -->
[![All Contributors](https://img.shields.io/badge/all_contributors-13-orange?style=flat-square)](#contributors)
<!-- ALL-CONTRIBUTORS-BADGE:END -->

A lightweight touch gesture recognition library created in C as a part of Georgia Tech's
Spring-Fall 2022 Junior Design program.

[**See the Demo!**](https://russell-newton.github.io/MultiTouch/)

---

## Contents

* [Installation](#installation)
* [Usage](#usage)
* [Design](#design)
  * [Touch Preprocessing](#touch-preprocessing)
  * [Recognizers](#recognizers)
* [Release Notes](#release-notes)

---

## Installation

1. Clone the repo.

```shell
git clone https://github.com/Russell-Newton/MultiTouch.git
```

2. Build the CMake project.

```shell
cd MultiTouch
cmake -S gesturelibrary -B build -D SKIP_TESTS=true
```

3. Compile the library with `make`.

```shell
cd build
make
```

4. Include the library when compiling your program:

    * Add `-I...pathto/MultiTouch/gesturelibrary/include` to your compile command.
    * Add `...pathto/MultiTouch/build/libGestureLibrary.a` to your compile targets.

---

## Usage

1. Configure any gesture recognition parameters before [building with CMake](#installation) by modifying
   [gestureparams.h](gesturelibrary/include/gestureparams.h). A full list of configurable parameters can be found in
   [the documentation for `gestureparams.h`](https://russell-newton.github.io/MultiTouch/docs/gestureparams_8h.html).
   <br>
   Example configuration:
2. Create an adapter for your touch input device. Adapters transform device input data into `touch_event_t`s.
3. Include `<gesturelib.h>`.
4. At program startup, run `init_gesturelib()`.
5. Whenever a touch is received, create a `touch_event_t` with your adapter and send it to `process_touch_event()`.
6. Processed gesture data can either be collected via the output of `process_touch_event()` or through gesture event
   listeners (See [listeners section](#listeners))
7. Create custom listeners or enable/disable built-in listeners with the provided utility functions:
    * `add_recognizer()`
    * `remove_recognizer()`
    * `enable_recognizer()`
    * `disable_recognizer()`

### Listeners

Listeners are single functions that accept gesture-specific data and have a void return type. They are called whenever a
recognizer's state machine updates its internal state. A listener should be registered after calling
`init_gesturelib()`.

Example:
```c
// main.c
#include <stdio.h>
#include <gesturelib.h>
#include <tap.h>

void tap_listener(tap_t event) {
    if (event.type == RECOGNIZER_STATE_COMPLETED) {
        printf("Tap received at (%.3f, %.3f)!", event.x, event.y);
    }
}

int main(int argc, char *argv[]) {
    init_gesturelib();
    
    // register the new listener
    set_on_tap(tap_listener);
    
    // rest of program
}
```

---

## Design

### Touch Preprocessing

After touch data has been transformed into a `touch_event_t` and sent to our library, the library will perform some
additional preprocessing. If the event has its group set to `TOUCH_ID_UNDEFINED`, the library will determine which touch
group it belongs to. If the device provides a touch group, the library will not assign one.

The touch group represents the finger a touch event was made by. That is, touch group 0 corresponds
to events created by the first finger pressed, 1 to the second, 2 to the third, and so on.

Touch group assignment is determined by event type:

* If the event is a down event, attempt to assign it to the first unused group. Track this event as the most recent
event in the group it was assigned to, marking the group as active. If there are no unassigned groups, leave the group
as unassigned.
* If the event is a move event, find the active group this event is closest to. Assign it to that group and track this
event as the most recent in the group. If there are no active groups, leave it unassigned.
* If the event is an up event, perform the same logic as with a move event. This time when a group is assigned, the
group is marked as inactive.

> ℹ️ Group assignment ensures that fingers generate the same group as long as they're in contact with the touch device.

After the preprocessing has finished, a touch event is sent to every enabled recognizer in the order in which they were
added to the library.

### Recognizers

Gesture recognizers are built like state machines. They receive touch events and update their state. When the state is
updated, they call on the registered event listener, if applicable.

Builtin single-finger gesture recognizers save data about every possible touch group that could be performing the
gesture they recognize.

Builtin multi-finger recognizers are more complicated and store data about every possible group for every possible user
id. User id is set by the data adapter and could be determined by factors like which device received the touch or where
on the screen the touch was received. 
> ⚠️ All touch events with the same uid will be considered as part of the same multi-finger gesture for recognition purposes.

### Gestures

Gesture recognition starts with a base gesture: stroke. Any other gestures can be recognized by composing and performing
additional processing on strokes and other composite gestures.

#### Stroke

This section to be filled in later.

---

## Release Notes

### Version 0.4.0

<details>

<summary>Expand for full details</summary>

#### New Features

* Zoom and Rotate split into their own gestures
* Removed swipe gesture
* Finished implementing gestures: tap, double tap, hold, hold and drag
* Demo page updates:
    * Links back to home page
    * Communicates with library using new listener structure
    * GestureCanvas component now sets display text within Demo component
    * Folder structure overhauled

#### Bug Fixes

* Zoom and rotate gestures work with more than 2 fingers

#### Known Issues

* Zoom and rotate gesture occasionally marked as complete on the demo page when a single drag has been performed
* Multi-finger double tap tests failing for unknown reason

</details>

### Version 0.3.0

<details>

<summary>Expand for full details</summary>

#### New Features

* Functioning swipe and drag gestures
* Minimally functioning zoom and rotate gesture
* Gesture library compiles into .js and .wasm with emscripten
    * Functions exposed by Module object to pack and unpack library structs without needing heap DMA

#### Bug Fixes

* Faulty unit tests removed

#### Known Issues

* Zoom and rotate gesture only works with 2 fingers
* 3+ finger zoom and rotate is planned for next sprint

</details>

### Version 0.2.1

<details>

<summary>Expand for full details</summary>

#### New Features

* Included Flutter project to collect example gesture `.csv` data.
* Pages deploy workflow modified to deploy web artifacts
    * Demo app: deployed to [root endpoint](https://russell-newton.github.io/MultiTouch/)
    * Documentation: deployed to [/docs endpoint](https://russell-newton.github.io/MultiTouch/docs)
    * Data collection: deployed
      to [/data-collection endpoint](https://russell-newton.github.io/MultiTouch/data-collection)

#### Known Issues

* Some unit tests SEGFAULT. These have been commented so the unit test workflow passes.

</details>

### Version 0.2.0

<details>

<summary>Expand for full details</summary>

#### New Features

* Framework for recognizer files (header and c files) created
* File organization updated
* Doxygen document generator linked to library
* Vue project environment set up
* Demo webapp front landing page created
* GitHub Actions workflow created to generate and deploy Doxygen documentation from [doxygen-config](doxygen-config)
* Created prebuild and predev npm scripts that compile C code with Emscripten, allowing for use in the webapp
* Created `build:run` npm script that runs `npm run build` and `live-server`

#### Bug Fixes

N/A

</details>

### Version 0.1.0

<details>

<summary>Expand for full details</summary>

#### New Features

* Sprint 1 limited to research, no features created
* Project is buildable

#### Bug Fixes

N/A

#### Known Issues

N/A

#### Research Done

* Specified input/output format for data
* Specified library and architecture structure

</details>

## Contributors

<!-- ALL-CONTRIBUTORS-LIST:START - Do not remove or modify this section -->
<!-- prettier-ignore-start -->
<!-- markdownlint-disable -->

<!-- markdownlint-restore -->
<!-- prettier-ignore-end -->

<!-- ALL-CONTRIBUTORS-LIST:END -->
