#!/bin/bash

echo "********** Installing yarn **********"

if ! command -v yarn &> /dev/null
then
    sudo npm i -g yarn
else
    echo "Yarn is already installed, skipping installation."
fi

cd ~/pufferfish-vent-software/frontend
yarn install
yarn build