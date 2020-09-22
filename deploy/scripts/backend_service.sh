#!/bin/bash

echo "********** Setting up backend service **********"

# Copying service file to systemd
cd ~/pufferfish-vent-software/deploy
sudo cp configs/pufferfish_backend.service /lib/systemd/system/
sudo chmod 644 /lib/systemd/system/pufferfish_backend.service

# Enabling service
sudo systemctl daemon-reload
sudo systemctl enable pufferfish_backend.service