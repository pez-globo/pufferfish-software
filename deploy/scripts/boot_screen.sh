#!/bin/bash

echo "********** Setting up custom boot screen **********"

cd ~/pufferfish-vent-software/deploy
sudo mkdir -p /etc/plymouth/themes
sudo cp plymouth/plymouthd.conf /etc/plymouth/plymouthd.conf
sudo cp -r plymouth/themes/pufferfish /etc/plymouth/themes/