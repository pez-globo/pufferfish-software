#!/bin/bash

# Message colours
ERROR='\033[1;31mERROR:'
SUCCESS='\033[1;32m'
WARNING='\033[1;33mWARNING:'

echo -e "\n${SUCCESS}********** Setting up nginx **********\n"

sudo apt update
sudo apt install nginx -y

# Symbolic Link to nginx for read-only filesystem
sudo ln -s /tmp /var/log/nginx

# Getting absolute path of frontend files
script_dir=$(dirname $(realpath $0))
frontend_dir=$script_dir/../../frontend

nginx_conf='/etc/nginx/nginx.conf'

# Adding 404 redirection to nginx configuration
if [ 0 -eq $( grep -c 'error_page 404 /;' $nginx_conf ) ]
then
    sudo sed -i '/access.log/i\\terror_page 404 /;' $nginx_conf
fi

if [ 0 -eq $( ls $frontend_dir | grep -c "build" ) ]
then
    echo -e "${WARNING} Build files not found"
else
    sudo cp -r $frontend_dir/build/* /var/www/html/
fi

echo -e "\n${SUCCESS}Nginx setup complete\n"