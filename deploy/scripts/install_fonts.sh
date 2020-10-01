#!/bin/bash

# Message colours
ERROR='\033[1;31mERROR:'
SUCCESS='\033[1;32m'
WARNING='\033[1;33mWARNING:'

cd ~/

echo -e "\n${SUCCESS}********** Installing Roboto Font **********\n"

mkdir -p .fonts

if [ $( ls .fonts/ | grep -c "Roboto" ) -ge 2 ]
then
    echo -e "${WARNING} Roboto fonts already available."
    exit
else
    wget -O roboto.zip "https://fonts.google.com/download?family=Roboto"
fi

sudo apt install unzip -y

if [ 1 -eq $( ls | grep -c "roboto.zip" ) ]
then
    unzip roboto.zip -d .fonts/
    rm roboto.zip
else
    echo -e "${ERROR} Fonts download failed"
    exit 1
fi

echo -e "\n${SUCCESS}Roboto fonts installation complete\n"