#!/bin/bash

config_path=/home/pi/.config/chromium/Default/Preferences

# Replacing config flag to denote it exited normally
if [[ -e $config_path ]]
then
    sed -i 's/"exited_cleanly":false/"exited_cleanly":true/' $config_path
    sed -i 's/"exit_type":"Crashed"/"exit_type":"Normal"/' $config_path
fi