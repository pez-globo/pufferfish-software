#!/bin/bash

echo "********** Setting up backend service **********"

# Getting absolute path of config files
script_dir=$(dirname $(realpath $0))
config_dir=$script_dir/../configs
backend_dir=$script_dir/../../backend/ventserver
backend_file=$(realpath $backend_dir)/simulation.py

if [ 1 -eq $( ls $config_dir | grep -c "pufferfish_backend.service" ) ]
then
    sudo cp $config_dir/pufferfish_backend.service /etc/systemd/system/
    sudo sed -i "s|python_file_absolute_path|$backend_file|g" /etc/systemd/system/pufferfish_backend.service
    sudo chmod 644 /etc/systemd/system/pufferfish_backend.service
else
    echo "The pufferfish_backend.service file doesn't exist"
    exit 1
fi

# Enabling service
sudo systemctl daemon-reload
sudo systemctl enable pufferfish_backend.service