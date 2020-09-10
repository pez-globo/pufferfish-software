#!/bin/bash

# Setup pyenv
./scripts/pyenv.sh

# Setup poetry and install required libraries
./scripts/poetry.sh

# Setup Node
./scripts/node.sh

# Setup yarn and install required libraries
./scripts/yarn.sh

# Setup OS Boot Screen
./scripts/boot_screen.sh