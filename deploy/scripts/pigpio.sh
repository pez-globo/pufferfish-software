#!/bin/bash

# Message colours
ERROR='\033[1;31mERROR:'
SUCCESS='\033[1;32m'
WARNING='\033[1;33mWARNING:'
NC='\033[0m'

echo -e "\n${SUCCESS}********** Installing pigpio **********\n${NC}"

sudo apt update
sudo apt install pigpio -y

echo -e "\n${SUCCESS}Pigpio setup complete\n${NC}"