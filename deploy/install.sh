#!/bin/bash

# Getting absolute path of script
script_path=$(dirname $(realpath $0))

# Setup pyenv
$script_path/scripts/pyenv.sh

# Setup poetry and install required libraries
$script_path/scripts/poetry.sh

# Setup pigpio dependency for backend
$script_path/scripts/pigpio.sh

# Setup Node
$script_path/scripts/node.sh

# Install Roboto Fonts
$script_path/scripts/install_fonts.sh

# Setup yarn and install required libraries
$script_path/scripts/yarn.sh

if [[ "$@" != "deploy" ]]
then
    echo -n "Reboot required for changes to take effect. Do you want to reboot now? [y/N]: "
    read answer
    if echo "$answer" | grep -iq "^y"
    then
        sudo reboot
    fi
fi
