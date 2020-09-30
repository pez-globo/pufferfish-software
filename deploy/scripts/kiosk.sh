#!/bin/bash

echo "********** Setting up kiosk service **********"

# Copying service file to systemd
cd ~/pufferfish-vent-software/deploy

if [ 1 -eq $( ls configs/ | grep -c "kiosk.service" ) ]
then
    sudo cp configs/kiosk.service /etc/systemd/system/
    sudo chmod 644 /etc/systemd/system/kiosk.service
else
    echo "The kiosk.service file doesn't exist"
    exit 1
fi

# Enabling service
sudo systemctl daemon-reload
sudo systemctl enable kiosk.service