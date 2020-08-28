# ventilator-controller-stm32

This is a STM32CubeIDE project of the embedded software which will run on the ventilator. We are testing this with the NUCLEO-H743ZI2 microcontroller board.


## Reconfiguring Pins and Peripherals with STM32CubeMX

Before you make any configuration changes to the microcontroller's pins or built-in peripherals using STM32CubeMX, you will need to rename the following files:

- Rename `Core/Src/main.cpp` to `Core/Src/main.c`
- Rename `Core/Src/stm32h7_it.cpp` to `Core/Src/stm32h7_it.c`

Then you can make configuration changes and regenerate code using STM32CubeMX. Then you'll need to rename those files back in order to recompile the project:

- Rename `Core/Src/main.c` to `Core/Src/main.cpp`
- Rename `Core/Src/stm32h7_it.c` to `Core/Src/stm32h7_it.cpp`


## Auto-Formatting

To automatically format all code using clang-format, first install `clang-format`.

Then, from this directory, run the `clang-format-all.sh` script with the usual options for clang-format
(though note that this script will use the configuration defined in the `.clang-format` file).
For example, to do a dry run identifying all necessary formatting changes, run:

```
./clang-format-all.sh --dry-run
```

And to automatically apply all necessary formatting changes directly to the files, run:

```
./clang-format-all.sh -i
```

If you want to run a command which sets the shel process return code to be an error
if at least one formatting change is necessary, run:

```
! ./clang-format-all.sh -dry-run 2>&1 | grep ''
```

## Static Code Checking

To run code checks, first install `cppcheck`, `cmake`, `clang-tidy`, and `clang-tools`.

### Cppcheck

First install `cppcheck`; if you'd like to use the GUI, also install `cppcheck-gui`.

You can run the cppcheck GUI by starting `cppcheck-gui` and opening the
`application.cppcheck` project file (to check the code to be uploaded to the STM32)
or the `testcatch2.cppcheck` project file (to check the test code). Th GUI also
makes it easy to edit the project files.

You can run cppcheck checks for the main application from the command-line with:

```
mkdir application-cppcheck-build-dir
cppcheck --project=application.cppcheck --inline-suppr --enable=all --error-exitcode=1
```

### Clang-tidy


## CMake Builds

To use CMake to build the main firmware application, first find the path where the
gcc arm-none-eabi toolchain is available (if you have not already installed it into
somewhere accessible from the shell), and add it to your shell's path. For example,
you might have this toolchain provided by the STM32Cube IDE at
`/opt/st/stm32cubeide_1.3.0/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.7-2018-q2-update.linux64_1.0.0.201904181610/tools/bin/`
in which case you could add it to your path as follows:
```
PATH=""/opt/st/stm32cubeide_1.3.0/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.7-2018-q2-update.linux64_1.0.0.201904181610/tools/bin/:$PATH"
```

Then make a CMake build directory and build the project. To build it in debug mode
with four build threads:
```
mkdir cmake-build-debug  # run from the firmware/ventilator-controller-stm32 directory
cd cmake-build-debug
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j4
```

To build it in release mode with two build threads:
```
mkdir cmake-build-release  # run from the firmware/ventilator-controller-stm32 directory
cd cmake-build-release
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j2
```


## Running

### Embedded Software in Debug Mode

To run the embedded software on the STM32, select the "ventilator-controller-stm32 Debug" run target in either the run configurations menu or the run configurations manager, and then run the target.

### Running Automated Tests

To run the automated test suite using catch2 on your own laptop (not on the STM32!), select the "ventilator-controller-stm32 TestCatch2" run target in either the run configurations menu or the run configurations manager, and then run the target.  Then the console should show an output reporting the results of the automated tests.
