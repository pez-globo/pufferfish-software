#!/bin/bash

# Message colours
ERROR='\033[1;31mERROR:'
SUCCESS='\033[1;32m'
WARNING='\033[1;33mWARNING:'
NC='\033[0m'

echo -e "\n${SUCCESS}********** Setting up nginx **********\n${NC}"

sudo apt update
sudo apt install nginx -y

# Getting absolute path of frontend files
script_dir=$(dirname $(realpath $0))
frontend_dir=$script_dir/../../frontend

if [ 0 -eq $( ls $frontend_dir | grep -c "build" ) ]
then
    echo -e "${WARNING} Build files not found${NC}"
else
    sudo cp -r $frontend_dir/build/* /var/www/html/
fi

echo -e "\n${SUCCESS}Nginx setup complete\n${NC}"