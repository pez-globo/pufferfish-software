#!/bin/bash

# Message colours
ERROR='\033[1;31mERROR:'
SUCCESS='\033[1;32m'
WARNING='\033[1;33mWARNING:'
NC='\033[0m'

echo -e "\n${SUCCESS}********** Installing pigpio **********\n${NC}"

cd ~/

wget https://github.com/joan2937/pigpio/archive/master.zip

if [ 1 -eq $( ls | grep -c "master.zip" ) ]
then
    unzip master.zip
    cd pigpio-master
    make
    sudo make install
    cd ..
    rm master.zip
else
    echo -e "${ERROR} pigpio zip download failed"
    exit 1
fi

echo -e "\n${SUCCESS}Pigpio setup complete\n"