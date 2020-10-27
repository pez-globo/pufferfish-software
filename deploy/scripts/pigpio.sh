#!/bin/bash

# Importing logging colours, absolute paths and exit function
script_dir=$(dirname $(realpath $0))
. $script_dir/helper.sh

echo -e "\n${SUCCESS}********** Installing pigpio **********\n${NC}"

sudo apt update
sudo apt install pigpio -y

sudo systemctl enable pigpiod
sudo systemctl daemon-reload

echo -e "\n${SUCCESS}Pigpio setup complete\n${NC}"