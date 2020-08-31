#!/bin/bash

echo "********** Installing node **********"

cd ~/
wget https://nodejs.org/dist/v12.18.3/node-v12.18.3-linux-x64.tar.gz
tar -xzf node-v12.18.3-linux-x64.tar.gz
cd node-v12.18.3-linux-x64/
sudo cp -R * /usr/local/
rm node-v12.18.3-linux-x64.tar.gz
rm -r node-v12.18.3-linux-x64/