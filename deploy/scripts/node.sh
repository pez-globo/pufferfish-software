#!/bin/bash

echo "********** Installing node **********"

cd ~/
wget https://nodejs.org/dist/v12.18.3/node-v12.18.3-linux-armv7l.tar.gz

if [ 1 -eq $( ls | grep -c "node-v12.18.3-linux-armv7l.tar.gz" ) ]
then
    tar -xzf node-v12.18.3-linux-armv7l.tar.gz
    cd node-v12.18.3-linux-armv7l/
    sudo cp -R * /usr/local/
    cd ..
    rm node-v12.18.3-linux-armv7l.tar.gz
    rm -r node-v12.18.3-linux-armv7l/
else
    echo "Node tar download failed"
    exit 1
fi