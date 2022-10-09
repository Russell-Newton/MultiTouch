#include <stdio.h>

#include <emscripten.h>

EMSCRIPTEN_KEEPALIVE
void say_hi() { printf("Hello, World!"); }

EMSCRIPTEN_KEEPALIVE
float square(float x) { return x * x; }
