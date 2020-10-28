#!/bin/bash

# Sets up user and network security
# Disables SSH for root and pi user
# Allows web serving ports (443 & 80) and disables other ports
# Disables SSH service
# Installs fail2ban
# Disables Wifi
# Locks pi user and removes pi from sudoers

# Message colours
ERROR='\033[1;31mERROR:'
SUCCESS='\033[1;32m'
WARNING='\033[1;33mWARNING:'
NC='\033[0m'

echo -e "\n${SUCCESS}********** Setting up User & Network Security **********\n${NC}"

sudo apt install openssh-server nginx ufw fail2ban -y

# Deny ssh for pi user
if [ 0 -eq $( grep -c "^DenyUsers pi" /etc/ssh/sshd_config ) ]
then
    echo -e "\nDenyUsers pi" | sudo tee -a /etc/ssh/sshd_config
fi

# Deny ssh for root user
if [ 0 -eq $( grep -c "^PermitRootLogin no" /etc/ssh/sshd_config ) ]
then
    echo -e "\nPermitRootLogin no" | sudo tee -a /etc/ssh/sshd_config
fi

# Firewall configuration
sudo ufw default deny incoming
sudo ufw deny ssh
sudo ufw allow in "Nginx Full"
sudo ufw enable

# Disabling ssh services
sudo systemctl mask ssh
sudo systemctl mask avahi-daemon.service

# local backup of fail2ban configuration
sudo cp /etc/fail2ban/jail.conf /etc/fail2ban/jail.local

sudo systemctl daemon-reload

# Add configuration to disable wifi
if [ 0 -eq $( grep -c "^dtoverlay=disable-wifi" /boot/config.txt ) ]
then
    echo -e "\ndtoverlay=disable-wifi" | sudo tee -a /boot/config.txt
else
    echo -e "${WARNING} Wifi is already disabled${NC}"
fi

# Lock pi and root user
sudo passwd -l pi
sudo passwd -l root

# Remove sudo permissions from pi user
sudo deluser pi sudo
sudo mv /etc/sudoers.d/010_pi-nopasswd /etc/sudoers.d/010_pi-nopasswd.

echo -e "\n${SUCCESS}User and Network Security setup complete\n${NC}"