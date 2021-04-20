#!/bin/bash

# Getting absolute path of script
script_path=$(dirname $(realpath $0))

# Message colours
ERROR='\033[1;31mERROR:'
NC='\033[0m'

# Function to exit script on failure
function exit_script {
  echo -e "${ERROR} $1 ${NC}"
  exit 1
}

# Enable Overlayfs and change /boot partition to read-only
$script_path/scripts/overlayfs.sh || exit_script "Read-only setup failed"

# Disable unnecessary background services
$script_path/scripts/disable_services.sh || exit_script "Disabling background services failed"

# Setup network security protocols
$script_path/scripts/security_network.sh || exit_script "Network security setup failed"

# Setup tampering security protocols
$script_path/scripts/security_tampering.sh || exit_script "Tampering security setup failed"

# Setup user security protocols
$script_path/scripts/security_user.sh || exit_script "User security setup failed"

if [[ "$@" != "pipeline" ]]
then
    echo -n "Reboot required for changes to take effect. Do you want to reboot now? [y/n]: "
    read answer
    if echo "$answer" | grep -iq "^y"
    then
        sudo reboot
    fi
fi
