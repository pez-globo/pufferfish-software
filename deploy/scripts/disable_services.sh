#!/bin/bash

# Disables services and their dependencies
# Services include bluetooth, bluetooth helpers
# Daily update and upgrade services, printer and smartcard

# Message colours
ERROR='\033[1;31mERROR:'
SUCCESS='\033[1;32m'
WARNING='\033[1;33mWARNING:'
NC='\033[0m'

echo -e "\n${SUCCESS}********** Disabling unnecessary services **********\n${NC}"

# Masks all the unnecessary services
# Removes its dependencies

sudo systemctl mask bluetooth.service
sudo systemctl mask hciuart.service
sudo systemctl mask bluealsa.service
sudo systemctl mask bthelper@.service
sudo systemctl mask apt-daily-upgrade.service
sudo systemctl mask apt-daily.service
sudo systemctl mask bluetooth.target
sudo systemctl mask printer.target
sudo systemctl mask smartcard.target

# Remove bluetooth dependencies
sudo apt-get purge bluez -y
sudo apt-get autoremove -y

# Add configuration to disable bluetooth
if [ 0 -eq $( grep -c "^dtoverlay=disable-bt" /boot/config.txt ) ]
then
    echo -e "\ndtoverlay=disable-bt" | sudo tee -a /boot/config.txt
else
    echo -e "${WARNING} Bluetooth is already disabled${NC}"
fi

echo -e "\n${SUCCESS}Unnecessary Services disabled\n${NC}"