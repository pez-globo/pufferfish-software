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
sudo systemctl mask bluealsa.service
sudo systemctl mask bthelper@.service
sudo systemctl mask apt-daily-upgrade.service
sudo systemctl disable apt-daily-upgrade.timer
sudo systemctl mask apt-daily.service
sudo systemctl disable apt-daily.timer
sudo systemctl mask bluetooth.target
sudo systemctl mask printer.target
sudo systemctl mask smartcard.target

# Remove bluetooth dependencies
sudo apt-get purge piwiz -y
sudo apt-get autoremove --purge -y

# Disable default ssh password warning
if [ 1 -eq $( ls /etc/xdg/lxsession/LXDE-pi/ | grep -c "sshpwd.sh" ) ]
then
    sudo rm /etc/xdg/lxsession/LXDE-pi/sshpwd.sh
fi

sudo systemctl daemon-reload

echo -e "\n${SUCCESS}Unnecessary Services disabled\n${NC}"