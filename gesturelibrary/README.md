## Building and Testing
1. Create a build folder outside the repo and `cd` into it
2. Run `cmake <path to gesturelibrary folder>` to create build files
3. Run `make` to build
4. Run `ctest [--verbose]` to run tests

## Commmitting
If you don't have pre-commit installed, follow these steps:
1. `sudo apt install python3-pip`
2. `pip install pre-commit`
3. From the repo's root folder, `pre-commit install`

*Note*: If `pre-commit` doesn't work, you probably don't have `~/.local/bin` in your PATH.

To fix that, you can either add it to your PATH or run `~/.local/bin/pre-commit install` instead.

To add it to your path, follow these steps:
1. Add the following line to the end of your `~/.bashrc` file: `export PATH="${HOME}/.local/bin/:${PATH}"`
2. Run `. ~/.bashrc`

## Architecture Points
1. Touch data is sent to the length 5 array, each spots corresponds to a group (id).
2. 