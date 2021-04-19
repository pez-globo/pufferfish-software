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

# Install required components
$script_path/install.sh "pipeline" || exit_script "Development components installation failed"

# Deploy services
$script_path/deploy.sh "pipeline" || exit_script "Development of services failed"

# Deploy lock services
$script_path/lock.sh || exit_script "Development of lock services failed"

echo -e "Reboot required for changes to take effect. Please restart!"
