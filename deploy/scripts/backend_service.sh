#!/bin/bash

# Backend systemd service to run backend on boot

# Message colours
ERROR='\033[1;31mERROR:'
SUCCESS='\033[1;32m'
WARNING='\033[1;33mWARNING:'
NC='\033[0m'

echo -e "\n${SUCCESS}********** Setting up backend service **********\n${NC}"

# Getting absolute path of config files
script_dir=$(dirname $(realpath $0))
config_dir=$script_dir/../configs
backend_dir=$script_dir/../../backend/ventserver
backend_file=$(realpath $backend_dir)/simulation.py

# Copy service file to systemd directory
if [ 1 -eq $( ls $config_dir | grep -c "pufferfish_backend.service" ) ]
then
    sudo cp $config_dir/pufferfish_backend.service /etc/systemd/system/
    sudo sed -i "s|python_file_absolute_path|$backend_file|g" /etc/systemd/system/pufferfish_backend.service
    sudo chmod 644 /etc/systemd/system/pufferfish_backend.service
else
    echo -e "${ERROR} The pufferfish_backend.service file doesn't exist${NC}"
    exit 1
fi

# Enabling service
sudo systemctl daemon-reload
sudo systemctl enable pufferfish_backend.service

echo -e "\n${SUCCESS}Backend Service setup complete\n${NC}"