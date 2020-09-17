#!/bin/bash

# This script installs yarn and front-end dependencies
# It creates a build release for front-end with static files

echo "********** Installing yarn **********"

sudo npm i -g yarn
cd ~/pufferfish-vent-software/frontend
yarn install
yarn build