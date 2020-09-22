#!/bin/bash

cd ~/

echo "********** Installing Roboto Font **********"
mkdir -p .fonts
wget -O roboto.zip "https://fonts.google.com/download?family=Roboto"

sudo apt install unzip -y

if [ 1 -eq $( ls | grep -c "roboto.zip" ) ]
then
    unzip roboto.zip -d .fonts/
    rm roboto.zip
else
    echo "Fonts download failed"
    exit 1
fi