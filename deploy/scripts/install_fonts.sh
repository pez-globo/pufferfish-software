#!/bin/bash

# This script installs all types of roboto fonts

cd ~/

echo "********** Installing Roboto Font **********"
mkdir -p .fonts
wget -O roboto.zip "https://fonts.google.com/download?family=Roboto"

sudo apt install unzip -y

unzip roboto.zip -d .fonts/
rm roboto.zip