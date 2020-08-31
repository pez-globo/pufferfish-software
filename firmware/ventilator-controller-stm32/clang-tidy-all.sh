#!/bin/bash

ERROR=0  # exit code

COMPILE_COMMANDS_DATABASE="-p cmake-build-debug"
STANDARD_ARGS="$COMPILE_COMMANDS_DATABASE $@"

# Set file filters
IGNORE_FILES="stm32h7xx_hal_conf.h"
STM32_USER_FILES="Core/Inc/main.h \
  Core/Src/main.cpp \
  Core/Inc/stm32h7xx_it.h \
  Core/Src/stm32h7xx_hal_msp.c \
  Core/Src/stm32h7xx_it.cpp"
INCLUDE_EXTENSIONS=".h .cpp .tpp"

# Set line filters based on file filters
EXCLUDE_FILTERS=`echo $IGNORE_FILES | tr ' ' '\n' \
  | awk '{print $1 " [1,1]"}' \
  | ./clang-tidy-line-filters.sh`
INCLUDE_FILTERS=`echo $INCLUDE_EXTENSIONS | tr ' ' '\n' \
  | ./clang-tidy-line-filters.sh`
STM32_USER_FILTERS=$(
  for FILE in $STM32_USER_FILES; do
    LINES=`./grep-stm32cubemx-user.sh "$FILE" \
      | awk 'BEGIN {FS=":"}; {print "[" $1 "," $2 "]" }' \
      | paste -s -d, -`
    echo "$FILE $LINES"
  done
)
STM32_USER_FILTERS=`echo "$STM32_USER_FILTERS" | ./clang-tidy-line-filters.sh`
echo $STM32_USER_FILTERS
LINE_FILTERS="[$EXCLUDE_FILTERS,$STM32_USER_FILTERS,$INCLUDE_FILTERS]"

# Check files
SOURCE_FILES=`find Core/Src/Pufferfish -iname *.cpp | xargs`
FILES="$SOURCE_FILES $STM32_USER_FILES"
echo clang-tidy $FILES --line-filter="$LINE_FILTERS" $STANDARD_ARGS
clang-tidy $FILES --line-filter="$LINE_FILTERS" $STANDARD_ARGS
