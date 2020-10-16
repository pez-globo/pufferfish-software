#!/bin/bash

# Sets up user and network security
# Disables SSH for root and pi user
# Allows web serving ports (443 & 80) and disables other ports
# Disables SSH service
# Installs fail2ban
# Disables Wifi
# Locks pi user and removes pi from sudoers
# Calculates hash value for OS Config, backend and frontend files

# Message colours
ERROR='\033[1;31mERROR:'
SUCCESS='\033[1;32m'
WARNING='\033[1;33mWARNING:'
NC='\033[0m'

echo -e "\n${SUCCESS}********** Setting up User & Network Security **********\n${NC}"

# Getting absolute path of config files
script_dir=$(dirname $(realpath $0))
config_dir=$script_dir/../configs

sudo apt install openssh-server nginx ufw fail2ban -y
pip3 install dirhash

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
sudo ufw --force enable

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

# Lock pi and root user
sudo passwd -l pi
sudo passwd -l root

# Remove sudo permissions from pi user
sudo deluser pi sudo
sudo mv /etc/sudoers.d/010_pi-nopasswd /etc/sudoers.d/010_pi-nopasswd.

echo -e "\n${SUCCESS}User and Network Security setup complete\n${NC}"