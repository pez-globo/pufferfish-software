#!/bin/bash

# Compares the original hash and generated hash to check for data tampering

if [ 1 -eq $( ls /opt | grep -c "hash_value" ) ]
then
    original_value=$(cat /opt/hash_value)
    calculated_value=$(python3 /opt/hash_check.py)

    if [[ "$original_value" == "$calculated_value" ]]
    then
        exit
    else
        zenity --error --text "Data Tampering check failed!" &
        exit 1
    fi

else
    zenity --error --text "Data Tampering check failed!" &
    exit 1
fi