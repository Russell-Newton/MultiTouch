# MultiTouch

<p align="center">
<img src="https://raw.githubusercontent.com/Russell-Newton/MultiTouch/main/images/click-2384-black.svg#gh-light-mode-only">
<img src="https://raw.githubusercontent.com/Russell-Newton/MultiTouch/main/images/click-2384-white.svg#gh-dark-mode-only">
</p>

[![Built with CMake](https://img.shields.io/badge/Built%20with-CMake-blue?style=flat-square&logo=cmake&color=064F8C)](#installation)
[![Demo Powered by Emscripten](https://img.shields.io/badge/Demo%20powered%20by-Emscripten-black?style=flat-square&logo=pytorch-lightning&color=b666d2)](https://russell-newton.github.io/MultiTouch/)
[![Docs generated by Doxygen](https://img.shields.io/badge/Docs%20generated%20with-Doxygen-black?style=flat-square&logo=read-the-docs&color=3d4ba2&logoColor=white)](https://russell-newton.github.io/MultiTouch/docs)

[![Deploy Artifacts to GitHub Pages](https://img.shields.io/github/deployments/Russell-Newton/MultiTouch/github-pages?label=Pages%20Deploy&logo=github&style=flat-square)](https://russell-newton.github.io/MultiTouch/)
[![Checks: pre-commit](https://img.shields.io/github/workflow/status/Russell-Newton/MultiTouch/Checks:%20pre-commit?label=pre-commit&logo=github&style=flat-square)](https://github.com/Russell-Newton/MultiTouch/actions/workflows/pre-commit.yml)
[![Unit Tests](https://img.shields.io/github/workflow/status/Russell-Newton/MultiTouch/Unit%20Tests?label=Unit%20Tests&logo=github&style=flat-square)](https://github.com/Russell-Newton/MultiTouch/actions/workflows/run_tests.yml)

<!-- ALL-CONTRIBUTORS-BADGE:START - Do not remove or modify this section -->
[![All Contributors](https://img.shields.io/badge/all_contributors-5-orange.svg?style=flat-square)](#contributors-)
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
    * [Listeners](#listeners)
    * [Gestures](#gestures)
* [Release Notes](#release-notes)

---

## Installation

### Prerequisites

1. Install build-essential to have access to `make` and `gcc`:

    ```shell
    sudo apt update && sudo apt install build-essential
    ```
    
2. Install CMake:
    
    ```shell
    sudo apt-get -y install cmake
    ```
    
### Option 1: Include Source in Your Project

1. Clone the repository into your project.

    ```shell
    git clone https://github.com/Russell-Newton/MultiTouch.git <Destination>
    ```
    
2. Include the source in your project
    * If you use CMake, then in a `CMakeLists.txt` of your project, add the `gesturelibrary` folder of the repository as a subdirectory using `add_subdirectory`. Delete the section of `gesturelibrary/CMakeLists.txt` in the `SKIP_TESTS` if statement.
    * If you do not use CMake, include the files in the `gesturelibrary/include` folder and add the files in the `gesturelibrary/src` folder to your executable.

### Option 2: Build Static Library and Link to Your Project

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
       
### Troubleshooting

If build errors occur, make sure you have `make` and `cmake` installed and added to your path. Ensure that you have a C compiler like `gcc`.
In Unix, `make` and `gcc` can be installed by running:

```shell
sudo apt update && sudo apt install build-essential
```

Other common build issues may be related to where the CMake build directory is located. Make sure you run `make` from within the directory created by running `cmake`.

---

## Usage

1. Include `<gesturelib.h>` and the header files for any estures you are interested in. For example, `<tap.h>` and `<drag.h>`.
1. Adjust the gesture parameters in `<gestureparams.h>` to your desired values. The variables can be set at runtime, but will require the gesture library to be reinitialized after modification.
1. Call `init_gesturelib()`.
1. Create an adapter for your touch input device. Adapters transform device input data into `touch_event_t`s.
1. Whenever a touch is received, create a `touch_event_t` with your adapter and send it to `process_touch_event()`.
    * If you want the library to determine which finger this event corresponds to, set `event.group = TOUCH_GROUP_UNDEFINED`.
1. Recognized gestures can be obtained from the library synchronously or asynchronously.

* To synchronously access recognized gestures,
    1. Call the `get_[gesture]` function of the gesture you are interested in. For example, `get_tap` and `get_drag`.
    2. This returns an array of gesture structs for the gesture you are interested in. For example, `tap_t` and `drag_t`.
    3. You can read the data from the array, but if a thread is currently executing the `process_touch_event()` function, then the data in the array may change as you are reading it.

* To asynchronously access recognized gestures,
   1. Create custom listeners or enable/disable built-in listeners with the provided utility functions:
       * `add_recognizer()`
       * `remove_recognizer()`
       * `enable_recognizer()`
       * `disable_recognizer()`
   1. Listeners accept a `const [gesture_t]*` and can read the data from the updated gesture. The gesture data will not change until the next invocation of `process_touch_event`.

### Listeners

Listeners are single functions that accept gesture-specific data and have a void return type. They are called whenever a
recognizer's state machine updates its internal state. A listener should be registered after calling
`init_gesturelib()`.

Example:

```c
// main.c
#include
<stdio.h>
#include
<gesturelib.h>
#include
<tap.h>

void tap_listener(const tap_t* event) {
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
> ⚠️ All touch events with the same uid will be considered as part of the same multi-finger gesture for recognition
> purposes.

### Gestures

Gesture recognition starts with a base gesture: stroke. Any other gestures can be recognized by composing and performing
additional processing on strokes and other composite gestures.

#### Stroke

Stroke is a simple gesture with a simple state machine:
<p align="center">
<img src="https://raw.githubusercontent.com/Russell-Newton/MultiTouch/main/images/stroke-sm-black.svg#gh-light-mode-only">
<img src="https://raw.githubusercontent.com/Russell-Newton/MultiTouch/main/images/stroke-sm-white.svg#gh-dark-mode-only">
</p>

The state updates are less important than the data that stroke collects. Stroke collects data on:

* Initial down event position and time
* Current move/up event position and time
* Move event speed (as a moving average with configurable window size)
* Touch group and user

When creating more complicated gestures, having access to this data can be incredibly useful.

#### Multistroke

Multistroke is a multi-finger counterpart to stroke. All strokes with the same user id get grouped into the same
multistroke. The first down event starts a multistroke, and the last up event for the user id ends the gesture. In
addition to the information contained in each stroke, a multistroke also tracks:

* Current centroid position
* Centroid instantaneous displacement
* Least-squares estimated rotation and zoom information

#### Tap

To perform a tap, press down and release within a short time and without moving too much.

<p align="center">
<img src="https://raw.githubusercontent.com/Russell-Newton/MultiTouch/main/images/tap-sm-black.svg#gh-light-mode-only">
<img src="https://raw.githubusercontent.com/Russell-Newton/MultiTouch/main/images/tap-sm-white.svg#gh-dark-mode-only">
</p>

Tap is a simple gesture that contains information about where and when the tap was started and released. If the time
between start and release is too long or the distance too great, the tap will fail.

#### Double-Tap

To perform a double-tap, tap twice in close succession.

<p align="center">
<img src="https://raw.githubusercontent.com/Russell-Newton/MultiTouch/main/images/dtap-sm-black.svg#gh-light-mode-only">
<img src="https://raw.githubusercontent.com/Russell-Newton/MultiTouch/main/images/dtap-sm-white.svg#gh-dark-mode-only">
</p>

Double-tap stores the same information as a tap.

#### Hold

To perform a hold, press down for a longer amount of time before releasing.

<p align="center">
<img src="https://raw.githubusercontent.com/Russell-Newton/MultiTouch/main/images/hold-sm-black.svg#gh-light-mode-only">
<img src="https://raw.githubusercontent.com/Russell-Newton/MultiTouch/main/images/hold-sm-white.svg#gh-dark-mode-only">
</p>

Hold stores the same information as a tap.

#### Drag

To perform a drag, press down and move your finger across the screen.

<p align="center">
<img src="https://raw.githubusercontent.com/Russell-Newton/MultiTouch/main/images/drag-sm-black.svg#gh-light-mode-only">
<img src="https://raw.githubusercontent.com/Russell-Newton/MultiTouch/main/images/drag-sm-white.svg#gh-dark-mode-only">
</p>

Drag tracks starting position, current position, and current velocity. Current velocity is retrieved in the same fashion
as stroke.

#### Hold and Drag

<p align="center">
<img src="https://raw.githubusercontent.com/Russell-Newton/MultiTouch/main/images/holddrag-sm-black.svg#gh-light-mode-only">
<img src="https://raw.githubusercontent.com/Russell-Newton/MultiTouch/main/images/holddrag-sm-white.svg#gh-dark-mode-only">
</p>

#### Multidrag

Like multistroke, multidrag is a multi-finger counterpart to drag. The same logic that applies to multistroke applies
to multidrag. It stores the same information as multistroke, but has a slightly different state machine and property
calculations.

Multidrag is used for processing zooms and rotates.

#### Zoom

To perform a zoom, press down with at least two fingers and move them closer together or farther apart.

<p align="center">
<img src="https://raw.githubusercontent.com/Russell-Newton/MultiTouch/main/images/zoom-sm-black.svg#gh-light-mode-only">
<img src="https://raw.githubusercontent.com/Russell-Newton/MultiTouch/main/images/zoom-sm-white.svg#gh-dark-mode-only">
</p>

Zoom tracks how many fingers are involved in the gesture and an estimated zoom factor.

#### Rotate

To perform a rotation, press down with a least two fingers and revolve them around a common center point.

<p align="center">
<img src="https://raw.githubusercontent.com/Russell-Newton/MultiTouch/main/images/rotate-sm-black.svg#gh-light-mode-only">
<img src="https://raw.githubusercontent.com/Russell-Newton/MultiTouch/main/images/rotate-sm-white.svg#gh-dark-mode-only">
</p>

Rotate tracks how many fingers are involved in the gesture and an estimated rotation amount.

---

## Release Notes

### Version 1.0.0 (Latest)

#### Features

* Created gesture recognizers:
    * Stroke (generic down/move/up gesture, no events generated)
    * Tap
    * Double tap
    * Hold
    * Drag
    * Hold and Drag
    * Zoom
    * Rotate
    * Multi-finger drag
    * Extensible to more gesture types
* Recognizers generate events if listeners are supplied with `set_on_<gesturetype>()`.
    * All saved gesture recognizer data can be accessed with `get_<gesturetype>()`.
* Gesture Library can auto-assign finger numbers to touch events if supplied group is `TOUCH_GROUP_UNDEFINED`.
* Autogenerated documentation created with Doxygen: [accessible here](https://russell-newton.github.io/MultiTouch/docs)
* Library demo created with Emscripten and Vite+Vue: [accessible here](https://russell-newton.github.io/MultiTouch)

#### Future Work

* Scale gesture parameters by DPI
* Rebase tap, double tap to repeat tap (see [branch](https://github.com/Russell-Newton/Multitouch/tree/repeat-tap))
* Implement identification of multi-finger taps, repeat tap distinctly from individual taps

#### Bug Fixes

* Fixed a bug where double-tapping would break with three fingers
* Fixed a bug where zoom and rotate gestures were marked as complete when a single drag was performed

#### Known Issues

* On the demo:
    * Opening the right-click context menu does not create an up event. This causes multi-finger gestures to misbehave
    (the library thinks a finger is down because it never received an up event)
    * Depending on the browser and OS, some gestures may try to complete as part of the browser's default handling as apposed to within our library
    * Zoom and Rotate generate a toast when each finger is released from the screen, including the last. This makes sense for releasing all fingers
    except the last finger on the screen. This last event doesn't carry any meaningful data.

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
<table>
  <tbody>
    <tr>
      <td align="center"><a href="https://www.linkedin.com/in/russellnewton01/"><img src="https://avatars.githubusercontent.com/u/18690062?v=4?s=100" width="100px;" alt="Russell Newton"/><br /><sub><b>Russell Newton</b></sub></a><br /><a href="https://github.com/Russell-Newton/MultiTouch/commits?author=Russell-Newton" title="Code">💻</a> <a href="https://github.com/Russell-Newton/MultiTouch/commits?author=Russell-Newton" title="Documentation">📖</a> <a href="#infra-Russell-Newton" title="Infrastructure (Hosting, Build-Tools, etc)">🚇</a> <a href="#maintenance-Russell-Newton" title="Maintenance">🚧</a></td>
      <td align="center"><a href="https://github.com/xorbit3024"><img src="https://avatars.githubusercontent.com/u/52808406?v=4?s=100" width="100px;" alt="Wenjun Wang"/><br /><sub><b>Wenjun Wang</b></sub></a><br /><a href="https://github.com/Russell-Newton/MultiTouch/commits?author=xorbit3024" title="Code">💻</a> <a href="https://github.com/Russell-Newton/MultiTouch/commits?author=xorbit3024" title="Documentation">📖</a> <a href="#infra-xorbit3024" title="Infrastructure (Hosting, Build-Tools, etc)">🚇</a> <a href="https://github.com/Russell-Newton/MultiTouch/commits?author=xorbit3024" title="Tests">⚠️</a></td>
      <td align="center"><a href="https://github.com/jrdike"><img src="https://avatars.githubusercontent.com/u/98774218?v=4?s=100" width="100px;" alt="jrdike"/><br /><sub><b>jrdike</b></sub></a><br /><a href="https://github.com/Russell-Newton/MultiTouch/commits?author=jrdike" title="Code">💻</a> <a href="https://github.com/Russell-Newton/MultiTouch/commits?author=jrdike" title="Documentation">📖</a> <a href="https://github.com/Russell-Newton/MultiTouch/commits?author=jrdike" title="Tests">⚠️</a></td>
      <td align="center"><a href="https://github.com/ocean-ift"><img src="https://avatars.githubusercontent.com/u/69494796?v=4?s=100" width="100px;" alt="Iftekherul Karim"/><br /><sub><b>Iftekherul Karim</b></sub></a><br /><a href="https://github.com/Russell-Newton/MultiTouch/commits?author=ocean-ift" title="Code">💻</a> <a href="https://github.com/Russell-Newton/MultiTouch/commits?author=ocean-ift" title="Documentation">📖</a> <a href="https://github.com/Russell-Newton/MultiTouch/commits?author=ocean-ift" title="Tests">⚠️</a></td>
      <td align="center"><a href="https://github.com/deborahsrcho"><img src="https://avatars.githubusercontent.com/u/84360441?v=4?s=100" width="100px;" alt="deborahsrcho"/><br /><sub><b>deborahsrcho</b></sub></a><br /><a href="https://github.com/Russell-Newton/MultiTouch/commits?author=deborahsrcho" title="Code">💻</a> <a href="#design-deborahsrcho" title="Design">🎨</a> <a href="#content-deborahsrcho" title="Content">🖋</a></td>
    </tr>
  </tbody>
</table>

<!-- markdownlint-restore -->
<!-- prettier-ignore-end -->

<!-- ALL-CONTRIBUTORS-LIST:END -->
<!-- prettier-ignore-start -->
<!-- markdownlint-disable -->

<!-- markdownlint-restore -->
<!-- prettier-ignore-end -->

<!-- ALL-CONTRIBUTORS-LIST:END -->
