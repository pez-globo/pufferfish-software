#!/bin/bash

# Setup pyenv
./scripts/pyenv.sh

# Setup poetry and install required libraries
./scripts/poetry.sh

# Install pigpio
./scripts/pigpio.sh

# Setup Node
./scripts/node.sh

# Install Roboto Fonts
./scripts/install_fonts.sh

# Setup yarn and install required libraries
./scripts/yarn.sh

# Setup Nginx web server for serving frontend
./scripts/nginx_setup.sh

# Setup OS Boot Screen
./scripts/boot_screen.sh

echo -n "Reboot required for changes to take effect. Do you want to reboot now? [y/N]: "
read answer
if echo "$answer" | grep -iq "^y"
then
    sudo reboot
fi
