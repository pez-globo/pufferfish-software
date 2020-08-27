# ventilator-controller-stm32

This is a STM32CubeIDE project of the embedded software which will run on the ventilator. We are testing this with the NUCLEO-H743ZI2 microcontroller board.

## Reconfiguring Pins and Peripherals with STM32CubeMX

Before you make any configuration changes to the microcontroller's pins or built-in peripherals using STM32CubeMX, you will need to rename the following files:

- Rename `Core/Src/main.cpp` to `Core/Src/main.c`
- Rename `Core/Src/stm32h7_it.cpp` to `Core/Src/stm32h7_it.c`

Then you can make configuration changes and regenerate code using STM32CubeMX. Then you'll need to rename those files back in order to recompile the project:

- Rename `Core/Src/main.c` to `Core/Src/main.cpp`
- Rename `Core/Src/stm32h7_it.c` to `Core/Src/stm32h7_it.cpp`

## Static Code Checking

To run code checks, first install `clang-tidy`, `clang-format`, `clang-tools`, and `cppcheck`.

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

## Running

### Embedded Software in Debug Mode

To run the embedded software on the STM32, select the "ventilator-controller-stm32 Debug" run target in either the run configurations menu or the run configurations manager, and then run the target.

### Running Automated Tests

To run the automated test suite using catch2 on your own laptop (not on the STM32!), select the "ventilator-controller-stm32 TestCatch2" run target in either the run configurations menu or the run configurations manager, and then run the target.  Then the console should show an output reporting the results of the automated tests.
