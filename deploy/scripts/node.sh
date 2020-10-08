#!/bin/bash

# Installs node 12.18.3

# Message colours
ERROR='\033[1;31mERROR:'
SUCCESS='\033[1;32m'
WARNING='\033[1;33mWARNING:'
NC='\033[0m'

echo -e "\n${SUCCESS}********** Installing node **********\n${NC}"

cd ~/

# Downloading node tar file
if ! command -v node &> /dev/null
then
    wget https://nodejs.org/dist/v12.18.3/node-v12.18.3-linux-armv7l.tar.gz
else
    echo -e "${WARNING} Node is already installed, skipping installation${NC}"
    exit
fi

# Installing node
if [ 1 -eq $( ls | grep -c "node-v12.18.3-linux-armv7l.tar.gz" ) ]
then
    tar -xzf node-v12.18.3-linux-armv7l.tar.gz
    cd node-v12.18.3-linux-armv7l/
    sudo cp -R * /usr/local/
    cd ..
    rm node-v12.18.3-linux-armv7l.tar.gz
    rm -r node-v12.18.3-linux-armv7l/
else
    echo -e "${ERROR} Node tar download failed${NC}"
    exit 1
fi

echo -e "\n${SUCCESS}Node setup complete\n${NC}"