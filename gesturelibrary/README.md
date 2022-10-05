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

## Testing
To generate test data:
1. Visit this [website](https://dartpad.dev/?id=dd735da036a2b0a468f94d5f6566b3f1)
2. Open the console
3. Perform gestures on the blue rectangle
4. Copy the console output into a csv file `test/res/<gesture>/<device>_<number>.csv` where `<number>` differentiates data of the same gesture on the same device.
