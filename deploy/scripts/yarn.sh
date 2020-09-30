#!/bin/bash

# This script installs yarn and front-end dependencies
# It creates a build release for front-end with static files

echo "********** Installing yarn **********"

# Getting absolute path of frontend files
script_dir=$(dirname $(realpath $0))
frontend_dir=$script_dir/../../frontend

if ! command -v yarn &> /dev/null
then
    sudo npm i -g yarn
else
    echo "Yarn is already installed, skipping installation."
fi

cd $frontend_dir
yarn install
yarn build