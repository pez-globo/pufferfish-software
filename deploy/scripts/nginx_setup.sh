#!/bin/bash

# This script installs nginx and adds front-end build files to 
# web serving directory

echo "********** Setting up nginx **********"

sudo apt update
sudo apt install nginx -y

# Symbolic Link to nginx for read-only filesystem
sudo ln -s /tmp /var/log/nginx

# Getting absolute path of frontend files
script_dir=$(dirname $(realpath $0))
frontend_dir=$script_dir/../../frontend

if [ 0 -eq $( ls $frontend_dir | grep -c "build" ) ]
then
    echo "Build files not found"
else
    sudo cp -r $frontend_dir/build/* /var/www/html/
fi