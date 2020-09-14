#!/bin/bash

echo "********** Setting up custom boot screen **********"

sudo apt-get update

# Disable rainbow screen default splash
echo -e "\ndisable_splash=1" | sudo tee -a /boot/config.txt

# Copy splash image to home directory
cp configs/splash.png ~/splash.png

# Disable logs on console
existing_command=$(cat /boot/cmdline.txt)
echo $existing_command" logo.nologo consoleblank=0 loglevel=1 quiet vt.global_cursor_default=0" | sudo tee /boot/cmdline.txt

sudo systemctl disable getty@tty3

# fbi package to read the image buffer
sudo apt install fbi -y

# Create service file
sudo cp configs/splashscreen.service /etc/systemd/system/

sudo apt-get update
sudo systemctl mask plymouth-start.service
sudo systemctl enable splashscreen

echo "System will reboot in 5 seconds..."
sudo reboot