#!/bin/bash

echo "********** Setting up Watchdog **********"

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
cat $config_dir/watchdog_config.txt | sudo tee -a /etc/watchdog.conf

# Starting watchdog service
sudo systemctl start watchdog
sudo systemctl daemon-reload
