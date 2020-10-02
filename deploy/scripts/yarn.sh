#!/bin/bash

# Message colours
ERROR='\033[1;31mERROR:'
SUCCESS='\033[1;32m'
WARNING='\033[1;33mWARNING:'
NC='\033[0m'

echo -e "\n${SUCCESS}********** Installing yarn **********\n${NC}"

# Getting absolute path of frontend files
script_dir=$(dirname $(realpath $0))
frontend_dir=$script_dir/../../frontend

if ! command -v yarn &> /dev/null
then
    sudo npm i -g yarn
else
    echo -e "${WARNING} Yarn is already installed, skipping installation${NC}"
fi

cd $frontend_dir
yarn install
yarn build

echo -e "\n${SUCCESS}Yarn setup complete\n${NC}"