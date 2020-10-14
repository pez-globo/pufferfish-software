#!/bin/bash

# Compares the original hash and generated hash to check for data tampering
error_msg="Data Tampering check failed! \nDisabled Backend and Kiosk service."

if [ 1 -eq $( ls /opt | grep -c "hash_value" ) ]
then
    original_value=$(cat /opt/hash_value)
    calculated_value=$(python3 /opt/hash_check.py)

    if [[ "$original_value" == "$calculated_value" ]]
    then
        exit
    else
        zenity --error --text "$error_msg" --width=300 --height=150 &
        exit 1
    fi

else
    zenity --error --text "$error_msg" --width=300 --height=150 &
    exit 1
fi