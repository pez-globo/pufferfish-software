#!/bin/bash

# Installs yarn and front-end dependencies
# Creates a build release for front-end with static files

echo "********** Installing yarn **********"

# Getting absolute path of frontend files
script_dir=$(dirname $(realpath $0))
frontend_dir=$script_dir/../../frontend

# Installing yarn
if ! command -v yarn &> /dev/null
then
    sudo npm i -g yarn
else
    echo "Yarn is already installed, skipping installation."
fi

# Static frontend build
cd $frontend_dir
yarn install
yarn build