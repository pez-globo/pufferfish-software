#!/bin/bash

# Message colours
ERROR='\033[1;31mERROR:'
SUCCESS='\033[1;32m'
WARNING='\033[1;33mWARNING:'
NC='\033[0m'

echo -e "\n${SUCCESS}********** Setting up kiosk service **********\n${NC}"

# Copying service file to systemd
cd ~/pufferfish-vent-software/deploy

if [ 1 -eq $( ls configs/ | grep -c "kiosk.service" ) ]
then
    sudo cp configs/kiosk.service /etc/systemd/system/
    sudo chmod 644 /etc/systemd/system/kiosk.service
else
    echo -e "${ERROR} The kiosk.service file doesn't exist${NC}"
    exit 1
fi

# Enabling service
sudo systemctl daemon-reload
sudo systemctl enable kiosk.service

echo -e "\n${SUCCESS}Kiosk setup complete\n${NC}"