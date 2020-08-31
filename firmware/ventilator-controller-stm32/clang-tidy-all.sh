#!/bin/bash

COMPILE_COMMANDS_DATABASE="-p cmake-build-debug"
STANDARD_ARGS="$COMPILE_COMMANDS_DATABASE $@"

# Set file filters for most files
IGNORE_FILES="stm32h7xx_hal_conf.h"
INCLUDE_EXTENSIONS=".h .cpp .tpp"
EXCLUDE_FILTERS=`echo $IGNORE_FILES | tr ' ' '\n' \
  | awk '{print $1 " [1,1]"}' \
  | ./clang-tidy-line-filters.sh`
INCLUDE_FILTERS=`echo $INCLUDE_EXTENSIONS | tr ' ' '\n' \
  | ./clang-tidy-line-filters.sh`
LINE_FILTERS="[$EXCLUDE_FILTERS,$INCLUDE_FILTERS]"

# Check source (and included header) files
FILES=`find Core/Src/Pufferfish -iname *.cpp | xargs`
echo clang-tidy $FILES --line-filter="$LINE_FILTERS" $STANDARD_ARGS
clang-tidy $FILES --line-filter="$LINE_FILTERS" $STANDARD_ARGS
echo

# # Check test source (and included header) files
# FILES=`find Core/Test -iname *.cpp | xargs`
# echo clang-tidy $FILES --line-filter="$LINE_FILTERS" $STANDARD_ARGS
# clang-tidy $FILES --line-filter="$LINE_FILTERS" $STANDARD_ARGS
# echo

# Check STM32CubeMX auto-generated files
MX_GENERATED="Core/Inc/main.h \
  Core/Src/main.cpp \
  Core/Inc/stm32h7xx_it.h \
  Core/Src/stm32h7xx_hal_msp.c \
  Core/Src/stm32h7xx_it.cpp"
LINE_FILTERS=$(
  for FILE in $MX_GENERATED; do
    LINES=`./grep-stm32cubemx-user.sh "$FILE" \
      | awk 'BEGIN {FS=":"}; {print "[" $1 "," $2 "]" }' \
      | paste -s -d, -`
    echo "$FILE $LINES"
  done
)
LINE_FILTERS=`echo "$LINE_FILTERS" | ./clang-tidy-line-filters.sh`
LINE_FILTERS="[$LINE_FILTERS]"
for FILE in $MX_GENERATED; do
  echo clang-tidy "$FILE" --line-filter="$LINE_FILTERS" $STANDARD_ARGS
  clang-tidy "$FILE" --line-filter="$LINE_FILTERS" $STANDARD_ARGS
  echo
done
