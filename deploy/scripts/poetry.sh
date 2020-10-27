#!/bin/bash

# Installs poetry and the backend package dependencies

# Message colours
ERROR='\033[1;31mERROR:'
SUCCESS='\033[1;32m'
WARNING='\033[1;33mWARNING:'
NC='\033[0m'

echo -e "\n${SUCCESS}********** Installing poetry **********\n${NC}"

poetry="$HOME/.poetry/bin/poetry"
ventserver_env="$HOME/.pyenv/versions/3.7.7/envs/ventserver/bin/python"

# Installing poetry
if ! command -v poetry &> /dev/null
then
    curl -sSL https://raw.githubusercontent.com/python-poetry/poetry/master/get-poetry.py | python3
else
    echo -e "${WARNING} Poetry is already installed, skipping installation${NC}"
fi

# Getting absolute path of backend files
script_dir=$(dirname $(realpath $0))
backend_dir=$script_dir/../../backend

# Installing backend package dependencies
cd $backend_dir
$poetry config virtualenvs.create false
$ventserver_env $poetry install

echo -e "\n${SUCCESS}Poetry setup complete\n${NC}"