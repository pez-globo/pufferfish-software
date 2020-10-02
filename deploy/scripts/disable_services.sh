#!/bin/bash

# Message colours
ERROR='\033[1;31mERROR:'
SUCCESS='\033[1;32m'
WARNING='\033[1;33mWARNING:'

echo -e "\n${SUCCESS}********** Disabling unnecessary services **********\n"

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
    echo -e "${WARNING} Bluetooth is already disabled"
fi

echo -e "\n${SUCCESS}Unnecessary Services disabled\n"
