#!/bin/bash

script_dir=$(pwd)

cp /lib/firmware/raspberrypi/bootloader/stable/pieeprom-2020-07-31.bin pieeprom.bin

if [ 1 -eq $( ls $script_dir/configs/ | grep -c "bootconf.txt" ) ]
then
    rpi-eeprom-config --out pieeprom-new.bin --config $script_dir/configs/bootconf.txt pieeprom.bin
    sudo rpi-eeprom-update -d -f ./pieeprom-new.bin
    rm pieeprom-new.bin
    rm pieeprom.bin
else
    echo "The bootconf.txt file doesn't exist"
    exit 1
fi