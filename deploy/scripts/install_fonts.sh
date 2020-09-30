#!/bin/bash

# This script installs all types of roboto fonts

cd ~/

echo "********** Installing Roboto Font **********"

mkdir -p .fonts

if [ $( ls .fonts/ | grep -c "Roboto" ) -ge 2 ]
then
    echo "Roboto fonts already available."
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
    echo "Fonts download failed"
    exit 1
fi