#!/bin/bash

echo "********** Setting up nginx **********"

sudo apt update
sudo apt install nginx -y

cd ~/pufferfish-vent-software/frontend

sudo cp -r build/* /var/www/html/