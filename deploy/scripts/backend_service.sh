#!/bin/bash

echo "********** Setting up backend service **********"

# Getting absolute path of config files
script_dir=$(dirname $(realpath $0))
config_dir=$script_dir/../configs

if [ 1 -eq $( ls $config_dir | grep -c "pufferfish_backend.service" ) ]
then
    sudo cp $config_dir/pufferfish_backend.service /etc/systemd/system/
    sudo chmod 644 /etc/systemd/system/pufferfish_backend.service
else
    echo "The pufferfish_backend.service file doesn't exist"
    exit 1
fi

# Enabling service
sudo systemctl daemon-reload
sudo systemctl enable pufferfish_backend.service