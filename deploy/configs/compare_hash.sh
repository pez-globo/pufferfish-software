#!/bin/bash

# Compares the original hash and generated hash to check for data tampering
error_msg="Data Tampering check failed! \nDisabled Backend and Kiosk service."

if [ 1 -eq $( ls /opt/pufferfish | grep -c "hash_value" ) ]
then
    original_value=$(cat /opt/pufferfish/hash_value)
    calculated_value=$(/usr/bin/python3 /opt/pufferfish/hash_check.py)

    if [[ "$original_value" == "$calculated_value" ]]
    then
        exit 0
    else
        zenity --error --text "$error_msg" --width=300 --height=150
        exit 1
    fi

else
    zenity --error --text "$error_msg" --width=300 --height=150
    exit 1
fi