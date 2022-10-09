.PHONY: all
all: prep main

prep:
	if not exist "..\public\c_bindings" mkdir "..\public\c_bindings"

main:
	emcc src/test.c -o ../public/c_bindings/test_api.js -sEXPORTED_RUNTIME_METHODS=ccall,cwrap
