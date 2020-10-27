#!/bin/bash

# Getting absolute path of script
script_path=$(dirname $(realpath $0))
. $script_path/scripts/helper.sh

# Setup pyenv
$script_path/scripts/pyenv.sh || exit_script "Pyenv installation failed"

# Setup poetry and install required libraries
$script_path/scripts/poetry.sh || exit_script "Poetry installation failed"

# Setup pigpio dependency for backend
$script_path/scripts/pigpio.sh || exit_script "Pigpio installation failed"

# Setup Node
$script_path/scripts/node.sh || exit_script "Node installation failed"

# Install Roboto Fonts
$script_path/scripts/install_fonts.sh || exit_script "Roboto Fonts installation failed"

# Setup yarn and install required libraries
$script_path/scripts/yarn.sh || exit_script "Yarn installation failed"

if [[ "$@" != "deploy" ]]
then
    echo -n "Reboot required for changes to take effect. Do you want to reboot now? [y/N]: "
    read answer
    if echo "$answer" | grep -iq "^y"
    then
        sudo reboot
    fi
fi
