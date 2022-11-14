DIR_LIB=lib

ifeq ($(OS),Windows_NT)
	MKDIR_LIB=if not exist "$(DIR_LIB)" mkdir "$(DIR_LIB)"
	MKDIR_COMPILE=if not exist "..\public\c_bindings" mkdir "..\public\c_bindings"
	RM_CLEAN_LIB=rmdir /q /s "$(DIR_LIB)"
	RM_CLEAN_COMPILE=rmdir /q /s "..\public\c_bindings"
else
	MKDIR_LIB=mkdir -p $(DIR_LIB)
	MKDIR_COMPILE=mkdir -p ../public/c_bindings
	RM_CLEAN_LIB=rm -rf $(DIR_LIB)/
	RM_CLEAN_COMPILE=rm -rf ../public/c_bindings
endif


FLAGS = -Wall -Wextra -pedantic -Werror
FLAGS += -s EXPORTED_FUNCTIONS="['_init_gesturelib']"
FLAGS += -s EXPORTED_RUNTIME_METHODS="['ccall','cwrap']"
FLAGS += --pre-js pre.js


.PHONY: all
all: build_lib compile
    
build_lib:
	$(MKDIR_LIB)
	emconfigure cmake -S ../../gesturelibrary -B $(DIR_LIB) -G "Unix Makefiles" -D SKIP_TESTS=true
	cd $(DIR_LIB) && emmake make

compile: src/*.c lib/libGestureLibrary.a
	$(MKDIR_COMPILE)
	emcc -I../../gesturelibrary/include $^ -o ../public/c_bindings/gesturelib_api.js $(FLAGS)

clean:
	$(RM_CLEAN_LIB)
	$(RM_CLEAN_COMPILE)