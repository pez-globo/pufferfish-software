#!/bin/bash

# Compares the original hash and generated hash to check for data tampering

if [ 1 -eq $( ls /opt | grep -c "hash_value" ) ]
then
    original_value = $(cat /opt/hash_value)
    calculated_value = $(python3 /opt/hash_check.py)

    if [[ "$original_value" == "$calculated_value" ]]
    then
        echo "Check successful"
        exit
    else
        echo "Check failed"
        exit 1
    fi

else
    echo "Check failed, hash_value not found!"
    exit 1
fi