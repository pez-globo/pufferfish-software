#!/bin/bash

# Locks pi user and removes pi from sudoers

# Message colours
ERROR='\033[1;31mERROR:'
SUCCESS='\033[1;32m'
WARNING='\033[1;33mWARNING:'
NC='\033[0m'

echo -e "\n${SUCCESS}********** Setting up User Security **********\n${NC}"

# Lock pi and root user
sudo passwd -l pi
sudo passwd -l root

# Remove sudo permissions from pi user
sudo deluser pi sudo
sudo mv /etc/sudoers.d/010_pi-nopasswd /etc/sudoers.d/010_pi-nopasswd.

echo -e "\n${SUCCESS}User Security setup complete\n${NC}"