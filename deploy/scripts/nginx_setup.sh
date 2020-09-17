#!/bin/bash

# This script installs nginx and adds front-end build files to 
# web serving directory

echo "********** Setting up nginx **********"

sudo apt update
sudo apt install nginx -y

# Symbolic Link to nginx for read-only filesystem
sudo ln -s /tmp /var/log/nginx

cd ~/pufferfish-vent-software/frontend

sudo cp -r build/* /var/www/html/