#!/bin/bash

# Calculates hash value for OS Config, backend and frontend files
# Sets up tampering service to check for tampering before starting backend and kiosk mode

# Message colours
ERROR='\033[1;31mERROR:'
SUCCESS='\033[1;32m'
WARNING='\033[1;33mWARNING:'
NC='\033[0m'

echo -e "\n${SUCCESS}********** Setting up Tampering Security **********\n${NC}"

# Copy target file to systemd directory
if [ 1 -eq $( ls $config_dir | grep -c "pufferfish.target" ) ]
then
    sudo cp $config_dir/pufferfish.target /etc/systemd/system/
    sudo chmod 644 /etc/systemd/system/pufferfish.target
else
    echo -e "${ERROR} The pufferfish.target file doesn't exist${NC}"
    exit 1
fi

# Copy service file to systemd directory
if [ 1 -eq $( ls $config_dir | grep -c "tampering.service" ) ]
then
    sudo cp $config_dir/tampering.service /etc/systemd/system/
    sudo chmod 644 /etc/systemd/system/tampering.service
else
    echo -e "${ERROR} The tampering.service file doesn't exist${NC}"
    exit 1
fi

# Enabling service
sudo systemctl daemon-reload
sudo systemctl set-default pufferfish.target
sudo systemctl enable tampering.service

if [ 1 -eq $( ls $config_dir | grep -c "hash_check.py" ) ]
then
    sudo cp $config_dir/hash_check.py /opt/
    sudo cp $config_dir/compare_hash.sh /opt/
    sudo chmod +x /opt/compare_hash.sh
    echo $(/usr/bin/python3 /opt/hash_check.py) | sudo tee /opt/hash_value
else
    echo -e "${ERROR} The hash_check.py file doesn't exist${NC}"
    exit 1
fi

echo -e "\n${SUCCESS}Tampering Security setup complete\n${NC}"