#!/bin/bash

echo "Installing unclutter..."
sudo apt install unclutter -y

# Getting absolute path of config files
script_dir=$(dirname $(realpath $0))
config_dir=$script_dir/../configs

# Set up autostart
mkdir -p $HOME/.config/lxsession/LXDE-pi

if [ 0 -eq $( ls $HOME/.config/lxsession/LXDE-pi/ | grep -c "autostart" ) ]
then
    touch $HOME/.config/lxsession/LXDE-pi/autostart
    cat /etc/xdg/lxsession/LXDE-pi/autostart > $HOME/.config/lxsession/LXDE-pi/autostart
fi

if [ 1 -eq $( ls $config_dir | grep -c "screen_config.txt" ) ]
then
    cat $config_dir/screen_config.txt >> $HOME/.config/lxsession/LXDE-pi/autostart
else
    echo "Configuration file (screen_config.txt) not found!"
    exit 1
fi

# Disable Alt+Tab (causes buggy behavior in Chromium with touchscreen events)
mkdir -p $HOME/.config/openbox

if [ 1 -eq $( ls $config_dir | grep -c "lxde-pi-rc.xml" ) ]
then
    touch $HOME/.config/openbox/lxde-pi-rc.xml
    cat $config_dir/lxde-pi-rc.xml > $HOME/.config/openbox/lxde-pi-rc.xml
else
    echo "Configuration file (lxde-pi-rc.xml) not found!"
    exit 1
fi