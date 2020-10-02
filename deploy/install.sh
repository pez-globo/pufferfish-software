#!/bin/bash

# Setup pyenv
./scripts/pyenv.sh

# Setup poetry and install required libraries
./scripts/poetry.sh

# Setup Node
./scripts/node.sh

# Install Roboto Fonts
./scripts/install_fonts.sh

# Setup yarn and install required libraries
./scripts/yarn.sh

if [ "$@" != "deploy" ]
then
    echo -n "Reboot required for changes to take effect. Do you want to reboot now? [y/N]: "
    read answer
    if echo "$answer" | grep -iq "^y"
    then
        sudo reboot
    fi
fi
