#!/bin/bash

# Generate files
nanopb_generator \
  -I "../proto" \
  -D "../firmware/ventilator-controller-stm32/Core/Inc/Pufferfish/Application" \
  -e "" \
  -Q '#include "Pufferfish/Application/%s"' \
  -L '#include "nanopb/%s"' \
  --cpp-descriptors \
  mcu_pb.proto

# Make descriptor methods constexpr, since nanopb neglects to do that
find ../firmware/ventilator-controller-stm32/Core/Inc/Pufferfish -name *_pb.h \
  | xargs sed -i 's/inline const pb_msgdesc_t/PB_INLINE_CONSTEXPR const pb_msgdesc_t/g'

# Scope all definitions inside a namespace

find ../firmware/ventilator-controller-stm32/Core/Inc/Pufferfish -name *_pb.h \
  | xargs sed -i '/\#include "nanopb\/pb.h"/a \
\
#ifdef __cplusplus\
namespace Pufferfish::Application {\
#endif'

find ../firmware/ventilator-controller-stm32/Core/Inc/Pufferfish -name *_pb.h \
  | xargs sed -i '/\/\* Message descriptors for nanopb \*\//i \
}  // namespace Pufferfish::Application\
'

find ../firmware/ventilator-controller-stm32/Core/Inc/Pufferfish -name *_pb.h \
  | xargs sed -i '/namespace nanopb {/a \
\
// using namespace is the simplest way to make the message declarations available in this section\
using namespace Pufferfish::Application;\
'

# Move source files, since STM32Cube IDE puts them in different directories
SOURCE_FILES=`find ../firmware/ventilator-controller-stm32/Core/Inc/Pufferfish -name *_pb.c`
for SOURCE in $SOURCE_FILES; do
  DEST=`echo $SOURCE | sed 's/Core\/Inc/Core\/Src/g'`
  [[ ! -e $SOURCE ]] && continue
  echo "Moving $SOURCE to $DEST"
  mv "$SOURCE" "$DEST"
done
