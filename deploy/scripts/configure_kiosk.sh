#!/bin/bash

# This script adds browser autostart to lxde-pi session
# Disables screen blanking and screen savers
# Hides mouse cursor on idle

echo "Installing unclutter..."
sudo apt install unclutter -y

script_dir=$(pwd)

cd ~/
mkdir -p .config/lxsession/LXDE-pi
touch .config/lxsession/LXDE-pi/autostart

cat /etc/xdg/lxsession/LXDE-pi/autostart > .config/lxsession/LXDE-pi/autostart
cat $script_dir/configs/screen_config.txt >> .config/lxsession/LXDE-pi/autostart