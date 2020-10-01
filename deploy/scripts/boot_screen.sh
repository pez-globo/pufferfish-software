#!/bin/bash

# Message colours
ERROR='\033[1;31mERROR:'
SUCCESS='\033[1;32m'
WARNING='\033[1;33mWARNING:'

echo -e "${SUCCESS}********** Setting up custom boot screen **********"

sudo apt-get update

# Getting absolute path of config files
script_dir=$(dirname $(realpath $0))
config_dir=$script_dir/../configs

# Copy splash image to home directory
cp $config_dir/splash.png ~/splash.png

# Disable logs on console
existing_command=$(cat /boot/cmdline.txt)
if [ 0 -eq $( cat /boot/cmdline.txt | grep -c "consoleblank=0 loglevel=1" ) ]
then
    echo $existing_command" consoleblank=0 loglevel=1 quiet vt.global_cursor_default=0" | sudo tee /boot/cmdline.txt
else
    echo -e "${WARNING} Logs are already disabled"
fi

sudo systemctl mask getty@tty1

# fim package to read the image buffer
sudo apt install fim -y

# Create service file
if [ 1 -eq $( ls $config_dir | grep -c "splashscreen.service" ) ]
then
    sudo cp $config_dir/splashscreen.service /etc/systemd/system/
else
    echo -e "${ERROR} The splashscreen.service file doesn't exist"
    exit 1
fi

# Configure lightdm
if [ 1 -eq $( ls $config_dir | grep -c "lightdm.conf" ) ]
then
    sudo cp $config_dir/lightdm.conf /etc/lightdm/lightdm.conf
else
    echo -e "${ERROR} Lightdm configuration file doesn't exist"
    exit 1
fi

sudo apt-get update
sudo systemctl mask plymouth-start.service
sudo systemctl enable splashscreen

echo -e "${SUCCESS}Custom Bootscreen setup complete"