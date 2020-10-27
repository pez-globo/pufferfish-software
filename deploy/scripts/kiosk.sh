#!/bin/bash

# Creates a systemd service for starting browser in kiosk mode on boot
# Adds a script to reset config before starting browser

# Importing logging colours, absolute paths and exit function
script_dir=$(dirname $(realpath $0))
. $script_dir/helper.sh

echo -e "\n${SUCCESS}********** Setting up kiosk service **********\n${NC}"

# Adding script to clean kiosk failure config
if [ 1 -eq $( ls $config_dir | grep -c "clean_kiosk.sh" ) ]
then
    sudo cp $config_dir/clean_kiosk.sh /opt/
    sudo chmod +x /opt/clean_kiosk.sh
else
    echo -e "${ERROR} The clean_kiosk.sh file doesn't exist${NC}"
    exit 1
fi

if [ 1 -eq $( ls $config_dir | grep -c "kiosk.service" ) ]
then
    sudo cp $config_dir/kiosk.service /etc/systemd/system/
    sudo chmod 644 /etc/systemd/system/kiosk.service
else
    echo -e "${ERROR} The kiosk.service file doesn't exist${NC}"
    exit 1
fi

# Enabling service
sudo systemctl daemon-reload
sudo systemctl enable kiosk.service

echo -e "\n${SUCCESS}Kiosk setup complete\n${NC}"