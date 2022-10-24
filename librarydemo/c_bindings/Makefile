ifeq ($(OS),Windows_NT)
	MKDIR_BUILD=if not exist "build" mkdir "build"
	MKDIR_COMPILE=if not exist "..\public\c_bindings" mkdir "..\public\c_bindings"
	RM_CLEAN_BUILD=rmdir /q /s "build"
	RM_CLEAN_COMPILE=rmdir /q /s "..\public\c_bindings"
else
	MKDIR_BUILD=mkdir -p build
	MKDIR_COMPILE=mkdir -p ../public/c_bindings
	RM_CLEAN_BUILD=rm -rf build/
	RM_CLEAN_COMPILE=rm -rf ../public/c_bindings
endif


.PHONY: all
all: compile_lib
    
compile_lib:
	$(MKDIR_BUILD)
	emconfigure cmake -S ../../gesturelibrary -B build -G "Unix Makefiles"
	cd build && emmake make

main:
	$(MKDIR_COMPILE)
	emcc src/test.c -o ../public/c_bindings/test_api.js -sEXPORTED_RUNTIME_METHODS=ccall,cwrap

clean:
	$(RM_CLEAN_BUILD)
	$(RM_CLEAN_COMPILE)