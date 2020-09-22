#!/bin/bash

echo "********** Setting up custom boot screen **********"

sudo apt-get update

# Disable rainbow screen default splash
if [ 0 -eq $( grep -c "^disable_splash=1" /boot/config.txt ) ]
then
    echo -e "\ndisable_splash=1" | sudo tee -a /boot/config.txt
else
    echo "Rainbow Screen is already disabled"
fi

# Copy splash image to home directory
cp configs/splash.png ~/splash.png

# Disable logs on console
existing_command=$(cat /boot/cmdline.txt)
if [ 0 -eq $( grep -c "logo.nologo" $existing_command ) ]
then
    echo $existing_command" logo.nologo consoleblank=0 loglevel=1 quiet vt.global_cursor_default=0" | sudo tee /boot/cmdline.txt
else
    echo "Logs are already disabled"
fi

sudo systemctl disable getty@tty3

# fbi package to read the image buffer
sudo apt install fbi -y

# Create service file
if [ 0 -eq $( ls configs/ | grep -c "splashscreen.service" ) ]
then
    sudo cp configs/splashscreen.service /etc/systemd/system/
else
    echo "The splashscreen.service file doesn't exist"
    exit 1
fi

sudo systemctl mask plymouth-start.service
sudo systemctl enable splashscreen