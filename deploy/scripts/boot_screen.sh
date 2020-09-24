#!/bin/bash

echo "********** Setting up custom boot screen **********"

sudo apt-get update
script_dir=$(pwd)

# Copy splash image to home directory
cp $script_dir/configs/splash.png ~/splash.png

# Disable logs on console
existing_command=$(cat /boot/cmdline.txt)
if [ 0 -eq $( cat /boot/cmdline.txt | grep -c "consoleblank=0 loglevel=1" ) ]
then
    echo $existing_command" consoleblank=0 loglevel=1 quiet vt.global_cursor_default=0" | sudo tee /boot/cmdline.txt
else
    echo "Logs are already disabled"
fi

sudo systemctl mask getty@tty1

# fim package to read the image buffer
sudo apt install fim -y

# Create service file
if [ 1 -eq $( ls $script_dir/configs/ | grep -c "splashscreen.service" ) ]
then
    sudo cp $script_dir/configs/splashscreen.service /etc/systemd/system/
else
    echo "The splashscreen.service file doesn't exist"
    exit 1
fi

# Configure lightdm
if [ 1 -eq $( ls $script_dir/configs/ | grep -c "lightdm.conf" ) ]
then
    sudo cp $script_dir/configs/lightdm.conf /etc/lightdm/lightdm.conf
else
    echo "Lightdm configuration file doesn't exist"
    exit 1
fi

sudo apt-get update
sudo systemctl mask plymouth-start.service
sudo systemctl enable splashscreen
