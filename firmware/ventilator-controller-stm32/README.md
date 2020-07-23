# ventilator-controller-stm32

This is a STM32CubeIDE project of the embedded software which will run on the ventilator. We are testing this with the NUCLEO-H743ZI2 microcontroller board.

## Reconfiguring Pins and Peripherals with STM32CubeMX

Before you make any configuration changes to the microcontroller's pins or built-in peripherals using STM32CubeMX, you will need to rename the following files:

- Rename `Core/Src/main.cpp` to `Core/Src/main.c`
- Rename `Core/Src/stm32h7_it.cpp` to `Core/Src/stm32h7_it.c`

Then you can make configuration changes and regenerate code using STM32CubeMX. Then you'll need to rename those files back in order to recompile the project:

- Rename `Core/Src/main.c` to `Core/Src/main.cpp`
- Rename `Core/Src/stm32h7_it.c` to `Core/Src/stm32h7_it.cpp`

## Running

### Embedded Software in Debug Mode

To run the embedded software on the STM32, select the "ventilator-controller-stm32 Debug" run target in either the run configurations menu or the run configurations manager, and then run the target.

### Running Automated Tests

To run the automated test suite using catch2 on your own laptop (not on the STM32!), select the "ventilator-controller-stm32 TestCatch2" run target in either the run configurations menu or the run configurations manager, and then run the target.  Then the console should show an output reporting the results of the automated tests.
