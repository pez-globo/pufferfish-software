#!/bin/bash

# Message colours
ERROR='\033[1;31mERROR:'
SUCCESS='\033[1;32m'
WARNING='\033[1;33mWARNING:'

echo -e "\n${SUCCESS}********** Installing poetry **********\n"

poetry="$HOME/.poetry/bin/poetry"
ventserver_env="$HOME/.pyenv/versions/3.7.7/envs/ventserver/bin/python"

if ! command -v pyenv &> /dev/null
then
    curl -sSL https://raw.githubusercontent.com/python-poetry/poetry/master/get-poetry.py | python3
else
    echo -e "${WARNING} Poetry is already installed, skipping installation"
fi

# Getting absolute path of backend files
script_dir=$(dirname $(realpath $0))
backend_dir=$script_dir/../../backend

cd $backend_dir
$poetry config virtualenvs.create false
$ventserver_env $poetry install

echo -e "\n${SUCCESS}Poetry setup complete\n"