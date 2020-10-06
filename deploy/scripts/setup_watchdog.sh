#!/bin/bash

# Message colours
ERROR='\033[1;31mERROR:'
SUCCESS='\033[1;32m'
WARNING='\033[1;33mWARNING:'

echo -e "\n${SUCCESS}********** Setting up Watchdog **********\n"

# Getting absolute path of config files
script_dir=$(dirname $(realpath $0))
config_dir=$script_dir/../configs

# Loading internal watchdog kernel module
sudo modprobe bcm2835_wdt
if [ 0 -eq $( grep -c "^bcm2835_wdt" /etc/modules ) ]
then
    echo -e "\nbcm2835_wdt" | sudo tee -a /etc/modules
fi

# Installing watchdog
sudo apt-get install watchdog -y

# Adding watchdog config
if [ 1 -eq $( ls $config_dir | grep -c "watchdog_config.txt" ) ]
then
    cat $config_dir/watchdog_config.txt | sudo tee -a /etc/watchdog.conf
else
    echo -e "${ERROR} Configuration file (watchdog_config.txt) not found!"
    exit 1
fi

# Starting watchdog service
sudo systemctl start watchdog
sudo systemctl daemon-reload

echo -e "\n${SUCCESS}Watchdog setup complete\n"